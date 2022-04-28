// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      �º˿Ƽ�(����)���޹�˾
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      AD7606 ADC
//
//      2022��04��25��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
/*
 *    ��ʱ������ ADC ת�� ת����ɺ��ȡ����
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
#include "emifa.h"
#include "timer.h"

#include "interrupt.h"

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      ȫ�ֱ���
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
// ADC ���� ÿͨ�� 4KBytes
short *ADCDataCH0 = (short *)0xC0000000;
short *ADCDataCH1 = (short *)0xC0001000;
short *ADCDataCH2 = (short *)0xC0002000;
short *ADCDataCH3 = (short *)0xC0003000;
short *ADCDataCH4 = (short *)0xC0004000;
short *ADCDataCH5 = (short *)0xC0005000;
short *ADCDataCH6 = (short *)0xC0006000;
short *ADCDataCH7 = (short *)0xC0007000;

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      GPIO �ܽŸ�������
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void GPIOBankPinMuxSet()
{
    // ������Ӧ�� GPIO �ڹ���Ϊ EMIFA �˿�
    // EMIFA D0-D15
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(9)) &
                                                   (~(SYSCFG_PINMUX9_PINMUX9_31_28  |
                                                      SYSCFG_PINMUX9_PINMUX9_27_24  |
                                                      SYSCFG_PINMUX9_PINMUX9_23_20  |
                                                      SYSCFG_PINMUX9_PINMUX9_19_16  |
                                                      SYSCFG_PINMUX9_PINMUX9_15_12  |
                                                      SYSCFG_PINMUX9_PINMUX9_11_8   |
                                                      SYSCFG_PINMUX9_PINMUX9_7_4    |
                                                      SYSCFG_PINMUX9_PINMUX9_3_0))) |
                                                    ((SYSCFG_PINMUX9_PINMUX9_31_28_EMA_D0 << SYSCFG_PINMUX9_PINMUX9_31_28_SHIFT) |
                                                     (SYSCFG_PINMUX9_PINMUX9_27_24_EMA_D1 << SYSCFG_PINMUX9_PINMUX9_27_24_SHIFT) |
                                                     (SYSCFG_PINMUX9_PINMUX9_23_20_EMA_D2 << SYSCFG_PINMUX9_PINMUX9_23_20_SHIFT) |
                                                     (SYSCFG_PINMUX9_PINMUX9_19_16_EMA_D3 << SYSCFG_PINMUX9_PINMUX9_19_16_SHIFT) |
                                                     (SYSCFG_PINMUX9_PINMUX9_15_12_EMA_D4 << SYSCFG_PINMUX9_PINMUX9_15_12_SHIFT) |
                                                     (SYSCFG_PINMUX9_PINMUX9_11_8_EMA_D5  << SYSCFG_PINMUX9_PINMUX9_11_8_SHIFT)  |
                                                     (SYSCFG_PINMUX9_PINMUX9_7_4_EMA_D6   << SYSCFG_PINMUX9_PINMUX9_7_4_SHIFT)   |
                                                     (SYSCFG_PINMUX9_PINMUX9_3_0_EMA_D7   << SYSCFG_PINMUX9_PINMUX9_3_0_SHIFT));

    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(8)) &
                                                   (~(SYSCFG_PINMUX8_PINMUX8_31_28  |
                                                      SYSCFG_PINMUX8_PINMUX8_27_24  |
                                                      SYSCFG_PINMUX8_PINMUX8_23_20  |
                                                      SYSCFG_PINMUX8_PINMUX8_19_16  |
                                                      SYSCFG_PINMUX8_PINMUX8_15_12  |
                                                      SYSCFG_PINMUX8_PINMUX8_11_8   |
                                                      SYSCFG_PINMUX8_PINMUX8_7_4    |
                                                      SYSCFG_PINMUX8_PINMUX8_3_0))) |
                                                    ((SYSCFG_PINMUX8_PINMUX8_31_28_EMA_D8  << SYSCFG_PINMUX8_PINMUX8_31_28_SHIFT) |
                                                     (SYSCFG_PINMUX8_PINMUX8_27_24_EMA_D9  << SYSCFG_PINMUX8_PINMUX8_27_24_SHIFT) |
                                                     (SYSCFG_PINMUX8_PINMUX8_23_20_EMA_D10 << SYSCFG_PINMUX8_PINMUX8_23_20_SHIFT) |
                                                     (SYSCFG_PINMUX8_PINMUX8_19_16_EMA_D11 << SYSCFG_PINMUX8_PINMUX8_19_16_SHIFT) |
                                                     (SYSCFG_PINMUX8_PINMUX8_15_12_EMA_D12 << SYSCFG_PINMUX8_PINMUX8_15_12_SHIFT) |
                                                     (SYSCFG_PINMUX8_PINMUX8_11_8_EMA_D13  << SYSCFG_PINMUX8_PINMUX8_11_8_SHIFT)  |
                                                     (SYSCFG_PINMUX8_PINMUX8_7_4_EMA_D14   << SYSCFG_PINMUX8_PINMUX8_7_4_SHIFT)   |
                                                     (SYSCFG_PINMUX8_PINMUX8_3_0_EMA_D15   << SYSCFG_PINMUX8_PINMUX8_3_0_SHIFT));

    // CS2
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(7)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(7)) &
                                                   (~(SYSCFG_PINMUX7_PINMUX7_3_0))) |
                                                    ((SYSCFG_PINMUX7_PINMUX7_3_0_NEMA_CS2 << SYSCFG_PINMUX7_PINMUX7_3_0_SHIFT));

    // ������Ӧ�� GPIO �ڹ���Ϊ��ͨ���������
    // RESET
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                                                        (~(SYSCFG_PINMUX11_PINMUX11_15_12))) |
                                                         ((SYSCFG_PINMUX11_PINMUX11_15_12_GPIO5_12 << SYSCFG_PINMUX11_PINMUX11_15_12_SHIFT));

    // CONVSTA �� CONVST
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                                                        (~(SYSCFG_PINMUX11_PINMUX11_11_8))) |
                                                         ((SYSCFG_PINMUX11_PINMUX11_11_8_GPIO5_13 << SYSCFG_PINMUX11_PINMUX11_11_8_SHIFT));

    // CONVSTB �� WE
    // AD7606 CONVSTA ת��ǰ 4 ͨ�� CONVSTB ת���� 4 ͨ��
    // AD7606B CONVST ת������ͨ�� WE ����д�ڲ�����Ĵ���
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(7)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(7)) &
                                                       (~(SYSCFG_PINMUX7_PINMUX7_19_16))) |
                                                        ((SYSCFG_PINMUX7_PINMUX7_19_16_GPIO3_11 << SYSCFG_PINMUX7_PINMUX7_19_16_SHIFT));

    // BUSY
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                                                        (~(SYSCFG_PINMUX11_PINMUX11_19_16))) |
                                                         ((SYSCFG_PINMUX11_PINMUX11_19_16_GPIO5_11 << SYSCFG_PINMUX11_PINMUX11_19_16_SHIFT));

    // FRSTDATA
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                                                        (~(SYSCFG_PINMUX11_PINMUX11_23_20))) |
                                                         ((SYSCFG_PINMUX11_PINMUX11_23_20_GPIO5_10 << SYSCFG_PINMUX11_PINMUX11_23_20_SHIFT));
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      GPIO �ܽų�ʼ��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void GPIOBankPinInit()
{
    // ���� GPIO5[11] / BUSY Ϊ����ģʽ
    GPIODirModeSet(SOC_GPIO_0_REGS, 92, GPIO_DIR_INPUT);

    // �����жϴ�����ʽ
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 92, GPIO_INT_TYPE_RISEDGE);

    // ʹ�� GPIO BANK �ж�
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 5);

    // ���� GPIO5[12] / RESET Ϊ���ģʽ
    GPIODirModeSet(SOC_GPIO_0_REGS, 93, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_0_REGS, 93, GPIO_PIN_HIGH);

    // ���� GPIO5[13] / CONVSTA Ϊ���ģʽ
    GPIODirModeSet(SOC_GPIO_0_REGS, 94, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_0_REGS, 94, GPIO_PIN_LOW);

    // ���� GPIO3[11] / CONVSTB Ϊ���ģʽ
    GPIODirModeSet(SOC_GPIO_0_REGS, 60, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_0_REGS, 60, GPIO_PIN_LOW);
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
//      ADC ��λ
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void ADCReset()
{
    GPIOPinWrite(SOC_GPIO_0_REGS, 93, GPIO_PIN_HIGH);
    Delay(0x1FFF);
    GPIOPinWrite(SOC_GPIO_0_REGS, 93, GPIO_PIN_LOW);
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      ADC ����
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void ADCStart()
{
    // CONVSTA
    GPIOPinWrite(SOC_GPIO_0_REGS, 94, GPIO_PIN_LOW);
    GPIOPinWrite(SOC_GPIO_0_REGS, 60, GPIO_PIN_LOW);

    // CONVSTB
    GPIOPinWrite(SOC_GPIO_0_REGS, 94, GPIO_PIN_HIGH);
    GPIOPinWrite(SOC_GPIO_0_REGS, 60, GPIO_PIN_HIGH);
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      EMIF ��ʼ��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
void EMIFInit()
{
    // ������������ 16bit
    EMIFAAsyncDevDataBusWidthSelect(SOC_EMIFA_0_REGS, EMIFA_CHIP_SELECT_2, EMIFA_DATA_BUSWITTH_16BIT);

    // ѡ�� Normal ģʽ
    EMIFAAsyncDevOpModeSelect(SOC_EMIFA_0_REGS, EMIFA_CHIP_SELECT_2, EMIFA_ASYNC_INTERFACE_NORMAL_MODE);

    // ��ֹWAIT����
    EMIFAExtendedWaitConfig(SOC_EMIFA_0_REGS, EMIFA_CHIP_SELECT_2, EMIFA_EXTENDED_WAIT_DISABLE);

    // ���� W_SETUP/R_SETUP W_STROBE/R_STROBE W_HOLD/R_HOLD   TA �Ȳ���
    EMIFAWaitTimingConfig(SOC_EMIFA_0_REGS, EMIFA_CHIP_SELECT_2, EMIFA_ASYNC_WAITTIME_CONFIG(1, 2, 1, 1, 2, 1, 0));
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      ��ʱ��/��������ʼ��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
void TimerInit()
{
    // ���ö�ʱ��/������ 2 Ϊ 64 λģʽ
    TimerConfigure(SOC_TMR_2_REGS, TMR_CFG_64BIT_CLK_INT);

    // �������� 228000000 / 200000 = 1140
    TimerPeriodSet(SOC_TMR_2_REGS, TMR_TIMER12, 1140);
    TimerPeriodSet(SOC_TMR_2_REGS, TMR_TIMER34, 0);

    // ʹ�ܶ�ʱ��/������ 2
    TimerEnable(SOC_TMR_2_REGS, TMR_TIMER12, TMR_ENABLE_CONT);

    // ʹ�ܶ�ʱ��/�������ж�
    TimerIntEnable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      PRU �жϳ�ʼ��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
void InterruptInit()
{
    // ����ж��¼�
    IntHostStatusClear();

    // ѡ�� PRU �����ж��¼��� 1
    IntEventGroupSet(SYS_INT_EVENTGROUP1);

    // ӳ���ж��¼���ͨ���������ж�
    // �����жϺ�ͨ��֮���������ӳ�� ���ͨ������ӳ�䵽ͬһ�����ж� ���ǲ�Ҫӳ��ͬһ��ͨ������������ж�
    // �Ƽ������жϺ�ͨ��һһӳ��
    // PRU �жϿ������� C66x ϵ�� DSP CIC �������� ���� CIC �����жϺ�ͨ���̶�һһӳ��
    IntEventMap(SYS_INT1_TIMER64P2OUT12, INT_CH0, HOST_INT0);

    // ʹ���¼�
    IntEventEnable(SYS_INT1_TIMER64P2OUT12);
    IntSystemEventClear(SYS_INT1_TIMER64P2OUT12);

    // ʹ�������ж�
    IntHostEnable(HOST_INT0);

    // ʹ��ȫ���ж�
    IntGlobalEnable();
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

    // PRU �жϳ�ʼ��
    InterruptInit();

    // EMIF ��ʼ��
    EMIFInit();

    // ��λ ADC
    ADCReset();

    // ��ʱ����ʼ��
    // 200KHz ������
    TimerInit();

    unsigned int count = 0;

    for(;;)
    {
        // PRU ����û���ж������� ��Ҫ��ѯ�ж�״̬
        if(IntHostIntStatusGet(HOST_INT0))
        {
            // ����ж�״̬
            IntSystemEventClear(SYS_INT1_TIMER64P2OUT12);

            // �����ʱ���жϱ�־
            TimerIntStatusClear(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);

            ADCDataCH0[count] = ((short *)SOC_EMIFA_CS2_ADDR)[1];
            ADCDataCH1[count] = ((short *)SOC_EMIFA_CS2_ADDR)[2];
            ADCDataCH2[count] = ((short *)SOC_EMIFA_CS2_ADDR)[3];
            ADCDataCH3[count] = ((short *)SOC_EMIFA_CS2_ADDR)[4];
            ADCDataCH4[count] = ((short *)SOC_EMIFA_CS2_ADDR)[5];
            ADCDataCH5[count] = ((short *)SOC_EMIFA_CS2_ADDR)[6];
            ADCDataCH6[count] = ((short *)SOC_EMIFA_CS2_ADDR)[7];
            ADCDataCH7[count] = ((short *)SOC_EMIFA_CS2_ADDR)[8];

            count == 2048 ? count = 0 : count++;

            // ���� ADC
            ADCStart();
        }
    }
}
