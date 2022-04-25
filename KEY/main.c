// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      �º˿Ƽ�(����)���޹�˾
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      GPIO KEY ����
//
//      2022��04��24��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
/*
 *    �� SW6 ���� ���İ�͵װ� LED ��˸
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

#include "interrupt.h"

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      GPIO �ܽŸ�������
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void GPIOBankPinMuxSet()
{
    // ������Ӧ�� GPIO �ڹ���Ϊ��ͨ���������
    // ���İ�
    // GPIO6[12]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & (~(SYSCFG_PINMUX13_PINMUX13_15_12))) |
                                                    ((SYSCFG_PINMUX13_PINMUX13_15_12_GPIO6_12 << SYSCFG_PINMUX13_PINMUX13_15_12_SHIFT));

    // GPIO6[13]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & (~(SYSCFG_PINMUX13_PINMUX13_11_8))) |
                                                    ((SYSCFG_PINMUX13_PINMUX13_11_8_GPIO6_13 << SYSCFG_PINMUX13_PINMUX13_11_8_SHIFT));

    // �װ� LED
    // GPIO2[15]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(05)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(05)) & (~(SYSCFG_PINMUX5_PINMUX5_3_0))) |
                                                    ((SYSCFG_PINMUX5_PINMUX5_3_0_GPIO2_15 << SYSCFG_PINMUX5_PINMUX5_3_0_SHIFT));

    // GPIO4[00]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(10)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(10)) & (~(SYSCFG_PINMUX10_PINMUX10_31_28))) |
                                                    ((SYSCFG_PINMUX10_PINMUX10_31_28_GPIO4_0 << SYSCFG_PINMUX10_PINMUX10_31_28_SHIFT));

    // ����
    // GPIO0[8]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(00)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(00)) & (~(SYSCFG_PINMUX0_PINMUX0_31_28))) |
                                                    ((SYSCFG_PINMUX0_PINMUX0_31_28_GPIO0_8 << SYSCFG_PINMUX0_PINMUX0_31_28_SHIFT));
}

// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      GPIO �ܽų�ʼ��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
static void GPIOBankPinInit()
{
    // ���İ�
    GPIODirModeSet(SOC_GPIO_0_REGS, 109, GPIO_DIR_OUTPUT);              // GPIO6[12] LED3
    GPIODirModeSet(SOC_GPIO_0_REGS, 110, GPIO_DIR_OUTPUT);              // GPIO6[13] LED2

    // �װ� LED
    GPIODirModeSet(SOC_GPIO_0_REGS, 48, GPIO_DIR_OUTPUT);               // GPIO2[15] LED4
    GPIODirModeSet(SOC_GPIO_0_REGS, 65, GPIO_DIR_OUTPUT);               // GPIO4[00] LED3

    // ����
    // �����жϴ�����ʽ
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 9, GPIO_INT_TYPE_FALLEDGE);         // SW6 �½���

    // ʹ�� GPIO BANK �ж�
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 0);                              // GPIO BANK0
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
    IntEventMap(SYS_INT1_GPIOBANK0, INT_CH0, HOST_INT0);

    // ʹ���¼�
    IntEventEnable(SYS_INT1_GPIOBANK0);
    IntSystemEventClear(SYS_INT1_GPIOBANK0);

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
    // ʹ������
    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_GPIO, PSC_POWERDOMAIN_ALWAYS_ON, PSC_MDCTL_NEXT_ENABLE);

    // �ܽŸ�������
    GPIOBankPinMuxSet();

    // GPIO �ܽų�ʼ��
    GPIOBankPinInit();

    // PRU �жϳ�ʼ��
    InterruptInit();

    for(;;)
    {
        // PRU ����û���ж������� ��Ҫ��ѯ�ж�״̬
        if(IntHostIntStatusGet(HOST_INT0))
        {
            if(GPIOPinIntStatus(SOC_GPIO_0_REGS, 9) == GPIO_INT_PEND)
            {
                // ��ʱ(�Ǿ�ȷ)
                Delay(0x00FFFFFF);
                GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_LOW);  // GPIO6[12] LED3
                GPIOPinWrite(SOC_GPIO_0_REGS, 110, GPIO_PIN_LOW);  // GPIO6[13] LED2

                GPIOPinWrite(SOC_GPIO_0_REGS, 48, GPIO_PIN_LOW);   // GPIO2[15] LED4
                GPIOPinWrite(SOC_GPIO_0_REGS, 65, GPIO_PIN_LOW);   // GPIO4[00] LED3

                // ��ʱ(�Ǿ�ȷ)
                Delay(0x00FFFFFF);
                GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_HIGH); // GPIO6[12] LED3
                GPIOPinWrite(SOC_GPIO_0_REGS, 110, GPIO_PIN_HIGH); // GPIO6[13] LED2

                GPIOPinWrite(SOC_GPIO_0_REGS, 48, GPIO_PIN_HIGH);  // GPIO2[15] LED4
                GPIOPinWrite(SOC_GPIO_0_REGS, 65, GPIO_PIN_HIGH);  // GPIO4[00] LED3

                GPIOPinIntClear(SOC_GPIO_0_REGS, 9);
            }

            IntSystemEventClear(SYS_INT1_GPIOBANK0);
        }
    }
}
