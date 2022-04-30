// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      �º˿Ƽ�(����)���޹�˾
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      LED �����(GPIO)
//
//      2022��04��27��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
/*
 *        A
 *      ����������
 *    F �� G ��B
 *      �ǩ�������
 *    E ��   ��C
 *      ����������. DP
 *        D
 *
 *    �������ʾ
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
//      �궨��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
#define SEGA    38
#define SEGB    37
#define SEGC    33
#define SEGD    61
#define SEGE    39
#define SEGF    40
#define SEGG    96
#define SEGDP   95

#define SEL1    91
#define SEL2    92
#define SEL3    93
#define SEL4    94

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      ȫ�ֱ���
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
#pragma DATA_SECTION(LEDNixieVal, ".Data")
unsigned char LEDNixieVal[4] = {8, 8, 8, 8};

unsigned char SEGVal[] =
{
    0xC0, 0xF9, 0xA4, 0xB0, 0x99,     // 0 1 2 3 4
    0x92, 0x82, 0xF8, 0x80, 0x90,     // 5 6 7 8 9
    0x88, 0x83, 0xC6, 0xA1, 0x86,     // A B C D E
    0x8E                              // F
};

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      GPIO �ܽŸ�������
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void GPIOBankPinMuxSet()
{
    // ������Ӧ�� GPIO �ڹ���Ϊ��ͨ���������
    // GPIO2[00] C/GPIO2[04] B/GPIO2[05] A/GPIO2[06] E/GPIO2[07] F
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(6)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(6)) &
                                                  (~(SYSCFG_PINMUX6_PINMUX6_31_28 |
                                                     SYSCFG_PINMUX6_PINMUX6_15_12 |
                                                     SYSCFG_PINMUX6_PINMUX6_11_8  |
                                                     SYSCFG_PINMUX6_PINMUX6_7_4   |
                                                     SYSCFG_PINMUX6_PINMUX6_3_0))) |
                                                   ((SYSCFG_PINMUX6_PINMUX6_31_28_GPIO2_0 << SYSCFG_PINMUX6_PINMUX6_31_28_SHIFT) |
                                                    (SYSCFG_PINMUX6_PINMUX6_15_12_GPIO2_4 << SYSCFG_PINMUX6_PINMUX6_15_12_SHIFT) |
                                                    (SYSCFG_PINMUX6_PINMUX6_11_8_GPIO2_5  << SYSCFG_PINMUX6_PINMUX6_11_8_SHIFT)  |
                                                    (SYSCFG_PINMUX6_PINMUX6_7_4_GPIO2_6   << SYSCFG_PINMUX6_PINMUX6_7_4_SHIFT)   |
                                                    (SYSCFG_PINMUX6_PINMUX6_3_0_GPIO2_7   << SYSCFG_PINMUX6_PINMUX6_3_0_SHIFT));

    // GPIO3[12] D
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(7)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(7)) & (~(SYSCFG_PINMUX7_PINMUX7_15_12))) |
                                                   (SYSCFG_PINMUX7_PINMUX7_15_12_GPIO3_12 << SYSCFG_PINMUX7_PINMUX7_15_12_SHIFT);

    // GPIO5[10] S1/GPIO5[11] S2/GPIO5[12] S3/GPIO5[13] S4/GPIO5[14] DP/GPIO5[15] G
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                                                   (~(SYSCFG_PINMUX11_PINMUX11_23_20 |
                                                      SYSCFG_PINMUX11_PINMUX11_19_16 |
                                                      SYSCFG_PINMUX11_PINMUX11_15_12 |
                                                      SYSCFG_PINMUX11_PINMUX11_11_8  |
                                                      SYSCFG_PINMUX11_PINMUX11_7_4   |
                                                      SYSCFG_PINMUX11_PINMUX11_3_0))) |
                                                    ((SYSCFG_PINMUX11_PINMUX11_23_20_GPIO5_10 << SYSCFG_PINMUX11_PINMUX11_23_20_SHIFT) |
                                                     (SYSCFG_PINMUX11_PINMUX11_19_16_GPIO5_11 << SYSCFG_PINMUX11_PINMUX11_19_16_SHIFT) |
                                                     (SYSCFG_PINMUX11_PINMUX11_15_12_GPIO5_12 << SYSCFG_PINMUX11_PINMUX11_15_12_SHIFT) |
                                                     (SYSCFG_PINMUX11_PINMUX11_11_8_GPIO5_13  << SYSCFG_PINMUX11_PINMUX11_11_8_SHIFT)  |
                                                     (SYSCFG_PINMUX11_PINMUX11_7_4_GPIO5_14   << SYSCFG_PINMUX11_PINMUX11_7_4_SHIFT)   |
                                                     (SYSCFG_PINMUX11_PINMUX11_3_0_GPIO5_15   << SYSCFG_PINMUX11_PINMUX11_3_0_SHIFT));
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      GPIO �ܽų�ʼ��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void GPIOBankPinInit()
{
    // ���� LED ��Ӧ�ܽ�Ϊ����ܽ�
    GPIODirModeSet(SOC_GPIO_0_REGS, SEGC, GPIO_DIR_OUTPUT);  // GPIO2[00] C
    GPIODirModeSet(SOC_GPIO_0_REGS, SEGB, GPIO_DIR_OUTPUT);  // GPIO2[04] B
    GPIODirModeSet(SOC_GPIO_0_REGS, SEGA, GPIO_DIR_OUTPUT);  // GPIO2[05] A
    GPIODirModeSet(SOC_GPIO_0_REGS, SEGE, GPIO_DIR_OUTPUT);  // GPIO2[06] E
    GPIODirModeSet(SOC_GPIO_0_REGS, SEGF, GPIO_DIR_OUTPUT);  // GPIO2[07] F
    GPIODirModeSet(SOC_GPIO_0_REGS, SEGD, GPIO_DIR_OUTPUT);  // GPIO3[12] D
    GPIODirModeSet(SOC_GPIO_0_REGS, SEL1, GPIO_DIR_OUTPUT);  // GPIO5[10] S1
    GPIODirModeSet(SOC_GPIO_0_REGS, SEL2, GPIO_DIR_OUTPUT);  // GPIO5[11] S2
    GPIODirModeSet(SOC_GPIO_0_REGS, SEL3, GPIO_DIR_OUTPUT);  // GPIO5[12] S3
    GPIODirModeSet(SOC_GPIO_0_REGS, SEL4, GPIO_DIR_OUTPUT);  // GPIO5[13] S4
    GPIODirModeSet(SOC_GPIO_0_REGS, SEGDP, GPIO_DIR_OUTPUT); // GPIO5[14] DP
    GPIODirModeSet(SOC_GPIO_0_REGS, SEGG, GPIO_DIR_OUTPUT);  // GPIO5[15] G
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      �������ʾ
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
void LEDNixieDisplay(unsigned char sel, unsigned char num)
{
    // ֵ�����ת��
    unsigned char val;
    val = SEGVal[num];

    // ��
    GPIOPinWrite(SOC_GPIO_0_REGS, SEGA,   val & 0x01);       // GPIO2[05] A
    GPIOPinWrite(SOC_GPIO_0_REGS, SEGB,  (val >> 1) & 0x01); // GPIO2[04] B
    GPIOPinWrite(SOC_GPIO_0_REGS, SEGC,  (val >> 2) & 0x01); // GPIO2[00] C
    GPIOPinWrite(SOC_GPIO_0_REGS, SEGD,  (val >> 3) & 0x01); // GPIO3[12] D
    GPIOPinWrite(SOC_GPIO_0_REGS, SEGE,  (val >> 4) & 0x01); // GPIO2[06] E
    GPIOPinWrite(SOC_GPIO_0_REGS, SEGF,  (val >> 5) & 0x01); // GPIO2[07] F
    GPIOPinWrite(SOC_GPIO_0_REGS, SEGG,  (val >> 6) & 0x01); // GPIO5[15] G
    GPIOPinWrite(SOC_GPIO_0_REGS, SEGDP, (val >> 7) & 0x01); // GPIO5[14] DP

    // �����������
    GPIOPinWrite(SOC_GPIO_0_REGS, SEL1, GPIO_PIN_LOW);
    GPIOPinWrite(SOC_GPIO_0_REGS, SEL2, GPIO_PIN_LOW);
    GPIOPinWrite(SOC_GPIO_0_REGS, SEL3, GPIO_PIN_LOW);
    GPIOPinWrite(SOC_GPIO_0_REGS, SEL4, GPIO_PIN_LOW);

    GPIOPinWrite(SOC_GPIO_0_REGS, sel, GPIO_PIN_HIGH);
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
//      ������
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
void main()
{
    // ʹ������
    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_GPIO, PSC_POWERDOMAIN_ALWAYS_ON, PSC_MDCTL_NEXT_ENABLE);

    // �ܽŸ�������
    GPIOBankPinMuxSet();

    // GPIO �ܽų�ʼ��
    GPIOBankPinInit();

    for(;;)
    {
        LEDNixieDisplay(SEL1, 8);
        Delay(1000);

        LEDNixieDisplay(SEL2, 8);
        Delay(1000);

        LEDNixieDisplay(SEL3, 8);
        Delay(1000);

        LEDNixieDisplay(SEL4, 8);
        Delay(1000);
    }
}
