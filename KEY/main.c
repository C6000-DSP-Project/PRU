// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      新核科技(广州)有限公司
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      GPIO KEY 按键
//
//      2022年04月24日
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/*
 *    按 SW6 按键 核心板和底板 LED 闪烁
 *
 *    - 希望缄默(bin wang)
 *    - bin@corekernel.net
 *
 *    官网 corekernel.net/.org/.cn
 *    社区 fpga.net.cn
 *
 */
#include "hw_types.h"
#include "hw_syscfg0_C6748.h"

#include "soc_C6748.h"

#include "psc.h"
#include "gpio.h"

#include "interrupt.h"

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      GPIO 管脚复用配置
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
static void GPIOBankPinMuxSet()
{
    // 配置相应的 GPIO 口功能为普通输入输出口
    // 核心板
    // GPIO6[12]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & (~(SYSCFG_PINMUX13_PINMUX13_15_12))) |
                                                    ((SYSCFG_PINMUX13_PINMUX13_15_12_GPIO6_12 << SYSCFG_PINMUX13_PINMUX13_15_12_SHIFT));

    // GPIO6[13]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & (~(SYSCFG_PINMUX13_PINMUX13_11_8))) |
                                                    ((SYSCFG_PINMUX13_PINMUX13_11_8_GPIO6_13 << SYSCFG_PINMUX13_PINMUX13_11_8_SHIFT));

    // 底板 LED
    // GPIO2[15]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(05)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(05)) & (~(SYSCFG_PINMUX5_PINMUX5_3_0))) |
                                                    ((SYSCFG_PINMUX5_PINMUX5_3_0_GPIO2_15 << SYSCFG_PINMUX5_PINMUX5_3_0_SHIFT));

    // GPIO4[00]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(10)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(10)) & (~(SYSCFG_PINMUX10_PINMUX10_31_28))) |
                                                    ((SYSCFG_PINMUX10_PINMUX10_31_28_GPIO4_0 << SYSCFG_PINMUX10_PINMUX10_31_28_SHIFT));

    // 按键
    // GPIO0[8]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(00)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(00)) & (~(SYSCFG_PINMUX0_PINMUX0_31_28))) |
                                                    ((SYSCFG_PINMUX0_PINMUX0_31_28_GPIO0_8 << SYSCFG_PINMUX0_PINMUX0_31_28_SHIFT));
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      GPIO 管脚初始化
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
static void GPIOBankPinInit()
{
    // 核心板
    GPIODirModeSet(SOC_GPIO_0_REGS, 109, GPIO_DIR_OUTPUT);              // GPIO6[12] LED3
    GPIODirModeSet(SOC_GPIO_0_REGS, 110, GPIO_DIR_OUTPUT);              // GPIO6[13] LED2

    // 底板 LED
    GPIODirModeSet(SOC_GPIO_0_REGS, 48, GPIO_DIR_OUTPUT);               // GPIO2[15] LED4
    GPIODirModeSet(SOC_GPIO_0_REGS, 65, GPIO_DIR_OUTPUT);               // GPIO4[00] LED3

    // 按键
    // 配置中断触发方式
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 9, GPIO_INT_TYPE_FALLEDGE);         // SW6 下降沿

    // 使能 GPIO BANK 中断
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 0);                              // GPIO BANK0
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      延时（非精确）
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
static void Delay(volatile unsigned int delay)
{
    while(delay--);
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      PRU 中断初始化
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void InterruptInit()
{
    // 清除中断事件
    IntHostStatusClear();

    // 选择 PRU 外设中断事件组 1
    IntEventGroupSet(SYS_INT_EVENTGROUP1);

    // 映射中断事件到通道及主机中断
    // 主机中断和通道之间可以任意映射 多个通道可以映射到同一主机中断 但是不要映射同一个通道到多个主机中断
    // 推荐主机中断和通道一一映射
    // PRU 中断控制器与 C66x 系列 DSP CIC 功能类似 但是 CIC 主机中断和通道固定一一映射
    IntEventMap(SYS_INT1_GPIOBANK0, INT_CH0, HOST_INT0);

    // 使能事件
    IntEventEnable(SYS_INT1_GPIOBANK0);
    IntSystemEventClear(SYS_INT1_GPIOBANK0);

    // 使能主机中断
    IntHostEnable(HOST_INT0);

    // 使能全局中断
    IntGlobalEnable();
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      主函数
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void main()
{
    // 使能外设
    PSCModuleControl(SOC_PSC_1_REGS, HW_PSC_GPIO, PSC_POWERDOMAIN_ALWAYS_ON, PSC_MDCTL_NEXT_ENABLE);

    // 管脚复用配置
    GPIOBankPinMuxSet();

    // GPIO 管脚初始化
    GPIOBankPinInit();

    // PRU 中断初始化
    InterruptInit();

    for(;;)
    {
        // PRU 核心没有中断向量表 需要查询中断状态
        if(IntHostIntStatusGet(HOST_INT0))
        {
            if(GPIOPinIntStatus(SOC_GPIO_0_REGS, 9) == GPIO_INT_PEND)
            {
                // 延时(非精确)
                Delay(0x00FFFFFF);
                GPIOPinWrite(SOC_GPIO_0_REGS, 109, GPIO_PIN_LOW);  // GPIO6[12] LED3
                GPIOPinWrite(SOC_GPIO_0_REGS, 110, GPIO_PIN_LOW);  // GPIO6[13] LED2

                GPIOPinWrite(SOC_GPIO_0_REGS, 48, GPIO_PIN_LOW);   // GPIO2[15] LED4
                GPIOPinWrite(SOC_GPIO_0_REGS, 65, GPIO_PIN_LOW);   // GPIO4[00] LED3

                // 延时(非精确)
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
