// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      �º˿Ƽ�(����)���޹�˾
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      AD9833 DDS
//
//      2022��04��25��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
/*
 *    DDS ֱ������Ƶ������
 *
 *    - ϣ����Ĭ(bin wang)
 *    - bin@corekernel.net
 *
 *    ���� corekernel.net/.org/.cn
 *    ���� fpga.net.cn
 *
 */
#include "hw_types.h"
#include "hw_syscfg0_C6748.h"

#include "soc_C6748.h"

#include "psc.h"
#include "gpio.h"

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      ȫ�ֱ���
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
typedef struct
{
    unsigned char flag;
    unsigned char mode;
    unsigned char regsel;
    unsigned int freq;
    unsigned short phase;
} AD9833Config;

#pragma DATA_SECTION(cfg, ".SHARE")
AD9833Config cfg;

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      GPIO �ܽŸ�������
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void GPIOBankPinMuxSet()
{
    // ������Ӧ�� GPIO �ڹ���Ϊ��ͨ���������
    // GPIO3[08] SCLK
    // GPIO3[09] SDATA
    // GPIO3[10] FSYNC
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(7)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(7)) &
                                                       (~(SYSCFG_PINMUX7_PINMUX7_31_28    |
                                                          SYSCFG_PINMUX7_PINMUX7_27_24    |
                                                          SYSCFG_PINMUX7_PINMUX7_23_20))) |
                                                        ((SYSCFG_PINMUX7_PINMUX7_31_28_GPIO3_8  << SYSCFG_PINMUX7_PINMUX7_31_28_SHIFT) |
                                                         (SYSCFG_PINMUX7_PINMUX7_27_24_GPIO3_9 << SYSCFG_PINMUX7_PINMUX7_27_24_SHIFT)  |
                                                         (SYSCFG_PINMUX7_PINMUX7_23_20_GPIO3_10 << SYSCFG_PINMUX7_PINMUX7_23_20_SHIFT));
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      GPIO �ܽų�ʼ��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void GPIOBankPinInit()
{
    GPIODirModeSet(SOC_GPIO_0_REGS, 57, GPIO_DIR_OUTPUT);  // GPIO3[08] SCLK
    GPIODirModeSet(SOC_GPIO_0_REGS, 58, GPIO_DIR_OUTPUT);  // GPIO3[09] SDATA
    GPIODirModeSet(SOC_GPIO_0_REGS, 59, GPIO_DIR_OUTPUT);  // GPIO3[10] FSYNC
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      ��ʱ���Ǿ�ȷ��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void Delay(volatile unsigned int delay)
{
    while(delay--);
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      ���ģ�� SPI
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
// ���Ŷ���
#define SCLK_SET      GPIOPinWrite(SOC_GPIO_0_REGS, 57, GPIO_PIN_HIGH)     // GPIO3[08]
#define SCLK_CLR      GPIOPinWrite(SOC_GPIO_0_REGS, 57, GPIO_PIN_LOW)      // GPIO3[08]
#define SCLK_TOGGLE   {                                                        \
                            Delay(100);                                        \
                            GPIOPinWrite(SOC_GPIO_0_REGS, 57, GPIO_PIN_LOW);   \
                            Delay(100);                                        \
                            GPIOPinWrite(SOC_GPIO_0_REGS, 57, GPIO_PIN_HIGH);  \
                       }                                                   // GPIO3[08]

#define SDATA_SET     GPIOPinWrite(SOC_GPIO_0_REGS, 58, GPIO_PIN_HIGH)     // GPIO3[09]
#define SDATA_CLR     GPIOPinWrite(SOC_GPIO_0_REGS, 58, GPIO_PIN_LOW)      // GPIO3[09]

#define FSYNC_SET     GPIOPinWrite(SOC_GPIO_0_REGS, 59, GPIO_PIN_HIGH)     // GPIO3[10]
#define FSYNC_CLR     GPIOPinWrite(SOC_GPIO_0_REGS, 59, GPIO_PIN_LOW)      // GPIO3[10]

void SoftSPIWrite(unsigned short data)
{
    SCLK_SET;

    FSYNC_SET;
    FSYNC_CLR;

    unsigned char i;
    for(i = 16; i > 0; i--)
    {
        (data >> (i - 1)) & 0x01 ? SDATA_SET : SDATA_CLR;
        SCLK_TOGGLE;
    }

    FSYNC_SET;
    Delay(1000);
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      AD9833 ����
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
#define AD9833_MCLK             (25000000)
#define AD9833_CTRLB28          (1 << 13)
#define AD9833_CTRLRESET        (1 << 8)

#define AD9833_OUTREG0          (0)
#define AD9833_OUTREG1          (1)

#define AD9833_SIN              (0)  // ���Ҳ�
#define AD9833_TRI              (1)  // ���ǲ�
#define AD9833_SQUARE           (3)  // ����

void AD9833Output(unsigned char mode, unsigned char regsel, unsigned int freq, unsigned short phase)
{
    // ��λ
    SoftSPIWrite(AD9833_CTRLRESET);

    // ����Ƶ��/��λ
    // д�� 28Bits Ƶ�ʼĴ���
    SoftSPIWrite(AD9833_CTRLB28 | AD9833_CTRLRESET);

    // Ƶ�ʻ���
    float FregReg = freq * 1.0 * 10.73742;  // 2^28 / 250000000

    // ѡ��Ƶ����λ�Ĵ�����
    if(regsel == 0)
    {
        // Ƶ��
        SoftSPIWrite(((unsigned int)FregReg & 0x0003FFF) | 0x4000);
        SoftSPIWrite((((unsigned int)FregReg & 0xFFFC000) >> 14) | 0x4000);

        // ��λ
        SoftSPIWrite((phase & 0xFFF) | 0xC000);
    }
    else
    {
        // Ƶ��
        SoftSPIWrite(((unsigned int)freq & 0x0003FFF) | 0x8000);
        SoftSPIWrite((((unsigned int)freq & 0xFFFC000) >> 14) | 0x8000);

        // ��λ
        SoftSPIWrite((phase & 0xFFF) | 0xE000);
    }

    // ���ò���
    switch(mode)
    {
        case 1:     // ���ǲ�
            SoftSPIWrite(0x2002); break;

        case 2:     // ����(����ʱ�ӷ�Ƶ) DAC ���� MSB / 2
            SoftSPIWrite(0x2020); break;

        case 3:     // ����(����ʱ�ӷ�Ƶ) DAC ���� MSB
            SoftSPIWrite(0x2028); break;

        default:    // ���Ҳ�
            SoftSPIWrite(0x2000);
            break;
    }
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      ������
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
void main()
{
    // �ܽŸ�������
    GPIOBankPinMuxSet();

    // GPIO �ܽų�ʼ��
    GPIOBankPinInit();

    // �������
    cfg.flag = 0;
    cfg.mode = AD9833_SQUARE;
    cfg.regsel = AD9833_OUTREG0;
    cfg.freq = 400;
    cfg.phase = 0;
    AD9833Output(cfg.mode, cfg.regsel, cfg.freq, cfg.phase);  // ������� 400Hz ���ǲ�

    for(;;)
    {
        if(cfg.flag)
        {
            AD9833Output(cfg.mode, cfg.regsel, cfg.freq, cfg.phase);
            cfg.flag = 0;
        }
    }
}
