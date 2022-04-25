// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      新核科技(广州)有限公司
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      KEY
//
//      2022年04月25日
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/*
 *    PRU0 专用输入引脚检测按键状态(需要加载到 PRU 核心 1 运行)
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
#include "gpio_pru.h"

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
                                                    ((SYSCFG_PINMUX13_PINMUX13_15_12_PRU1_R30_11 << SYSCFG_PINMUX13_PINMUX13_15_12_SHIFT));

    // GPIO6[13]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & (~(SYSCFG_PINMUX13_PINMUX13_11_8))) |
                                                    ((SYSCFG_PINMUX13_PINMUX13_11_8_PRU1_R30_12 << SYSCFG_PINMUX13_PINMUX13_11_8_SHIFT));

    // 按键
    // GPIO8[12] 默认值为  PRU1_R31[24]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) & (~(SYSCFG_PINMUX18_PINMUX18_23_20))) |
                                                    ((SYSCFG_PINMUX18_PINMUX18_23_20_DEFAULT << SYSCFG_PINMUX18_PINMUX18_23_20_SHIFT));

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
//      主函数
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void main()
{
    // 管脚复用配置
    GPIOBankPinMuxSet();

    for(;;)
    {
        if(!GPIPinRead(24))
        {
            // 延时(非精确)
            Delay(0x00FFFFFF);
            GPOPinWrite(11, GPIO_PIN_LOW);   // GPIO6[12] PRU1_R30_11 LED3
            GPOPinWrite(12, GPIO_PIN_LOW);   // GPIO6[13] PRU1_R30_12 LED2

            // 延时(非精确)
            Delay(0x00FFFFFF);
            GPOPinWrite(11, GPIO_PIN_HIGH);  // GPIO6[12] PRU1_R30_11 LED3
            GPOPinWrite(12, GPIO_PIN_HIGH);  // GPIO6[13] PRU1_R30_12 LED2
        }
    }
}
