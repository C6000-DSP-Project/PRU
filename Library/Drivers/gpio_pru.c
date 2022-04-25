// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      新核科技(广州)有限公司
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      PRU 专用输出引脚
//
//      2022年04月25日
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/*
 *    - 希望缄默(bin wang)
 *    - bin@corekernel.net
 *
 *    官网 corekernel.net/.org/.cn
 *    社区 fpga.net.cn
 *
 */
#include "gpio_pru.h"

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      写 GPO
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/**
 * 描述  向指定引脚写 1 或 0
 *
 * 参数   pinNumber  指定 GPO 引脚
 *                GPO 引脚从 0 - 31
 *
 * 参数   bitValue   指定写逻辑 0 或逻辑 1
 *                支持如下两个值
 *                1> GPO_PIN_LOW, 清除(逻辑 0)
 *                2> GPO_PIN_HIGH, 置位(逻辑 1)
 *
 * 返回值  无
 */
volatile register unsigned int __R30;

void GPOPinWrite(unsigned char pinNumber, unsigned char bitValue)
{
    if(GPIO_PIN_LOW == bitValue)
    {
        __R30 &= ~(1 << pinNumber);
    }
    else if(GPIO_PIN_HIGH == bitValue)
    {
        __R30 |= (1 << pinNumber);
    }
}

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      读 GPI
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/**
 * 描述  获取指定引脚值
 *
 * 参数   pinNumber  指定 GPO 引脚
 *                GPO 引脚从 0 - 29
 *
 * 返回值   返回指定引脚状态值
 *                支持如下两个值
 *                1> GPI_PIN_LOW, 如果引脚值为逻辑 0
 *                2> GPI_PIN_HIGH, 如果引脚值为逻辑 1
 *
 */
volatile register unsigned int __R31;

unsigned char GPIPinRead(unsigned char pinNumber)
{
    return (__R31 >> pinNumber) & 0x01;
}

