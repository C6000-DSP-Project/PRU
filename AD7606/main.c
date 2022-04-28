// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      新核科技(广州)有限公司
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      AD7606 ADC
//
//      2022年04月25日
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/*
 *    定时器启动 ADC 转换 转换完成后读取数据
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
#include "emifa.h"
#include "timer.h"

#include "interrupt.h"

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      全局变量
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ADC 数据 每通道 4KBytes
short *ADCDataCH0 = (short *)0xC0000000;
short *ADCDataCH1 = (short *)0xC0001000;
short *ADCDataCH2 = (short *)0xC0002000;
short *ADCDataCH3 = (short *)0xC0003000;
short *ADCDataCH4 = (short *)0xC0004000;
short *ADCDataCH5 = (short *)0xC0005000;
short *ADCDataCH6 = (short *)0xC0006000;
short *ADCDataCH7 = (short *)0xC0007000;

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      GPIO 管脚复用配置
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
static void GPIOBankPinMuxSet()
{
    // 配置相应的 GPIO 口功能为 EMIFA 端口
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

    // 配置相应的 GPIO 口功能为普通输入输出口
    // RESET
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                                                        (~(SYSCFG_PINMUX11_PINMUX11_15_12))) |
                                                         ((SYSCFG_PINMUX11_PINMUX11_15_12_GPIO5_12 << SYSCFG_PINMUX11_PINMUX11_15_12_SHIFT));

    // CONVSTA 或 CONVST
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(11)) &
                                                        (~(SYSCFG_PINMUX11_PINMUX11_11_8))) |
                                                         ((SYSCFG_PINMUX11_PINMUX11_11_8_GPIO5_13 << SYSCFG_PINMUX11_PINMUX11_11_8_SHIFT));

    // CONVSTB 或 WE
    // AD7606 CONVSTA 转换前 4 通道 CONVSTB 转换后 4 通道
    // AD7606B CONVST 转换所有通道 WE 用于写内部软件寄存器
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

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      GPIO 管脚初始化
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
static void GPIOBankPinInit()
{
    // 设置 GPIO5[11] / BUSY 为输入模式
    GPIODirModeSet(SOC_GPIO_0_REGS, 92, GPIO_DIR_INPUT);

    // 配置中断触发方式
    GPIOIntTypeSet(SOC_GPIO_0_REGS, 92, GPIO_INT_TYPE_RISEDGE);

    // 使能 GPIO BANK 中断
    GPIOBankIntEnable(SOC_GPIO_0_REGS, 5);

    // 设置 GPIO5[12] / RESET 为输出模式
    GPIODirModeSet(SOC_GPIO_0_REGS, 93, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_0_REGS, 93, GPIO_PIN_HIGH);

    // 设置 GPIO5[13] / CONVSTA 为输出模式
    GPIODirModeSet(SOC_GPIO_0_REGS, 94, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_0_REGS, 94, GPIO_PIN_LOW);

    // 设置 GPIO3[11] / CONVSTB 为输出模式
    GPIODirModeSet(SOC_GPIO_0_REGS, 60, GPIO_DIR_OUTPUT);
    GPIOPinWrite(SOC_GPIO_0_REGS, 60, GPIO_PIN_LOW);
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
//      ADC 复位
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
static void ADCReset()
{
    GPIOPinWrite(SOC_GPIO_0_REGS, 93, GPIO_PIN_HIGH);
    Delay(0x1FFF);
    GPIOPinWrite(SOC_GPIO_0_REGS, 93, GPIO_PIN_LOW);
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      ADC 启动
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
static void ADCStart()
{
    // CONVSTA
    GPIOPinWrite(SOC_GPIO_0_REGS, 94, GPIO_PIN_LOW);
    GPIOPinWrite(SOC_GPIO_0_REGS, 60, GPIO_PIN_LOW);

    // CONVSTB
    GPIOPinWrite(SOC_GPIO_0_REGS, 94, GPIO_PIN_HIGH);
    GPIOPinWrite(SOC_GPIO_0_REGS, 60, GPIO_PIN_HIGH);
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      EMIF 初始化
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void EMIFInit()
{
    // 配置数据总线 16bit
    EMIFAAsyncDevDataBusWidthSelect(SOC_EMIFA_0_REGS, EMIFA_CHIP_SELECT_2, EMIFA_DATA_BUSWITTH_16BIT);

    // 选择 Normal 模式
    EMIFAAsyncDevOpModeSelect(SOC_EMIFA_0_REGS, EMIFA_CHIP_SELECT_2, EMIFA_ASYNC_INTERFACE_NORMAL_MODE);

    // 禁止WAIT引脚
    EMIFAExtendedWaitConfig(SOC_EMIFA_0_REGS, EMIFA_CHIP_SELECT_2, EMIFA_EXTENDED_WAIT_DISABLE);

    // 配置 W_SETUP/R_SETUP W_STROBE/R_STROBE W_HOLD/R_HOLD   TA 等参数
    EMIFAWaitTimingConfig(SOC_EMIFA_0_REGS, EMIFA_CHIP_SELECT_2, EMIFA_ASYNC_WAITTIME_CONFIG(1, 2, 1, 1, 2, 1, 0));
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      定时器/计数器初始化
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void TimerInit()
{
    // 配置定时器/计数器 2 为 64 位模式
    TimerConfigure(SOC_TMR_2_REGS, TMR_CFG_64BIT_CLK_INT);

    // 设置周期 228000000 / 200000 = 1140
    TimerPeriodSet(SOC_TMR_2_REGS, TMR_TIMER12, 1140);
    TimerPeriodSet(SOC_TMR_2_REGS, TMR_TIMER34, 0);

    // 使能定时器/计数器 2
    TimerEnable(SOC_TMR_2_REGS, TMR_TIMER12, TMR_ENABLE_CONT);

    // 使能定时器/计数器中断
    TimerIntEnable(SOC_TMR_2_REGS, TMR_INT_TMR12_NON_CAPT_MODE);
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
    IntEventMap(SYS_INT1_TIMER64P2OUT12, INT_CH0, HOST_INT0);

    // 使能事件
    IntEventEnable(SYS_INT1_TIMER64P2OUT12);
    IntSystemEventClear(SYS_INT1_TIMER64P2OUT12);

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
    // 管脚复用配置
    GPIOBankPinMuxSet();

    // GPIO 管脚初始化
    GPIOBankPinInit();

    // PRU 中断初始化
    InterruptInit();

    // EMIF 初始化
    EMIFInit();

    // 复位 ADC
    ADCReset();

    // 定时器初始化
    // 200KHz 采样率
    TimerInit();

    unsigned int count = 0;

    for(;;)
    {
        // PRU 核心没有中断向量表 需要查询中断状态
        if(IntHostIntStatusGet(HOST_INT0))
        {
            // 清除中断状态
            IntSystemEventClear(SYS_INT1_TIMER64P2OUT12);

            // 清除定时器中断标志
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

            // 启动 ADC
            ADCStart();
        }
    }
}
