/****************************************************************************/
/*                                                                          */
/*    新核科技(广州)有限公司                                                */
/*                                                                          */
/*    Copyright (C) 2022 CoreKernel Technology (Guangzhou) Co., Ltd         */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*    PRU 中断控制器                                                        */
/*                                                                          */
/*    2022年03月28日                                                        */
/*                                                                          */
/****************************************************************************/
/*
 *    - 希望缄默(bin wang)
 *    - bin@corekernel.net
 *
 *    官网 corekernel.net/.org/.cn
 *    社区 fpga.net.cn
 *
 */
#include "hw_types.h"

#include "interrupt.h"

/****************************************************************************/
/*                                                                          */
/*              使能全局中断                                                */
/*                                                                          */
/****************************************************************************/
void IntGlobalEnable()
{
    HWREG(PRUINTCBase + PRUINTC_GLBLEN) = 1;
}

/****************************************************************************/
/*                                                                          */
/*              禁用全局中断                                                */
/*                                                                          */
/****************************************************************************/
void IntGlobalDisable()
{
    HWREG(PRUINTCBase + PRUINTC_GLBLEN) = 0;
}

/****************************************************************************/
/*                                                                          */
/*              使能中断事件                                                */
/*                                                                          */
/****************************************************************************/
void IntEventEnable(int event)
{
    HWREG(PRUINTCBase + PRUINTC_ENIDXSET) = (0x00000000 | event);
}

/****************************************************************************/
/*                                                                          */
/*              禁用中断事件                                                */
/*                                                                          */
/****************************************************************************/
void IntEventDisable(int event)
{
    HWREG(PRUINTCBase + PRUINTC_ENIDXCLR) = event;
}

/****************************************************************************/
/*                                                                          */
/*              使能主机中断                                                */
/*                                                                          */
/****************************************************************************/
void IntHostEnable(int host)
{
    host |= 0x00000000;
    HWREG(PRUINTCBase + PRUINTC_HSTINTENIDXSET) = host;
}

/****************************************************************************/
/*                                                                          */
/*              禁用主机中断                                                */
/*                                                                          */
/****************************************************************************/
void IntHostDisable(int host)
{
    HWREG(PRUINTCBase + PRUINTC_HSTINTENIDXCLR) = host;
}

/****************************************************************************/
/*                                                                          */
/*              系统事件映射                                                */
/*                                                                          */
/****************************************************************************/
void IntEventMap(int event, int ch, int host)
{
    volatile unsigned int CHREG;

    // 映射输入事件到中断通道
    HWREG(PRUINTCBase + PRUINTC_CHANMAP(event / 4)) = ch << (8 * (event % 4));

    // 映射中断通道到主机中断
    HWREG(PRUINTCBase + PRUINTC_HOSTMAP(ch / 4)) = host << (8 * (ch % 4));
}

/****************************************************************************/
/*                                                                          */
/*              清除系统事件标志                                            */
/*                                                                          */
/****************************************************************************/
void IntSystemEventClear(int event)
{
    HWREG(PRUINTCBase + PRUINTC_STATIDXCLR) = event;
}

/****************************************************************************/
/*                                                                          */
/*              使能统事件标志                                              */
/*                                                                          */
/****************************************************************************/
void IntSystemEventEnable(int event)
{
    HWREG(PRUINTCBase + PRUINTC_ENIDXSET) = (0x00000000 | event);
}

/****************************************************************************/
/*                                                                          */
/*              触发普通中断事件                                            */
/*                                                                          */
/****************************************************************************/
volatile register unsigned int __R31;

void IntSystemEventGenerate(int event)
{
    // 仅适用于 32-63 号普通系统事件
    __R31 |= 1 << 5;
    __R31 = event - 32;
}

/****************************************************************************/
/*                                                                          */
/*              获取主机中断 0/1 状态                                       */
/*                                                                          */
/****************************************************************************/
int IntHostIntStatusGet(int host)
{
    return (__R31 & (0x00000001<<(30 + host)));
}

void IntHostStatusClear()
{
    __R31 &= 0x0000;
}

/****************************************************************************/
/*                                                                          */
/*              选择 PRU 中断事件组                                         */
/*                                                                          */
/****************************************************************************/
void IntEventGroupSet(int group)
{
    int t = group;

    HWREG(0x01C14188) = (HWREG(0x01C14188) & ~(1 << 3)) | (t << 3);
}

/****************************************************************************/
/*                                                                          */
/*              设置优先级寄存器                                                                                                                                        */
/*                                                                          */
/****************************************************************************/
void IntPolaritySet(int group ,int event ,int value)
{
    if(group)
    {
        HWREG(PRUINTCBase + PRUINTC_POLARITY1) |= (value << event);
    }
    else
    {
        HWREG(PRUINTCBase + PRUINTC_POLARITY0) |= (value << event);
    }

    HWREG(PRUINTCBase + PRUINTC_POLARITY0) |= 0xFFFFFFFF;
}

/****************************************************************************/
/*                                                                          */
/*              设置中断类型寄存器                                                                                                                                        */
/*                                                                          */
/****************************************************************************/
void IntTypeSet(int group,int event,int value)
{
    if(group)
    {
        HWREG(PRUINTCBase + PRUINTC_TYPE1) |= (value << event);
    }
    else
    {
        HWREG(PRUINTCBase + PRUINTC_TYPE0) |= (value << event);
    }

    HWREG(PRUINTCBase + PRUINTC_TYPE0) |= 0x80;
    HWREG(PRUINTCBase + PRUINTC_TYPE1) |= 0x84;
}
