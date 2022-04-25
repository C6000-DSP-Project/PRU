// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      新核科技(广州)有限公司
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      PRU 中断控制器
//
//      2022年03月28日
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
#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#ifdef __cplusplus
extern "C" {
#endif

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      宏定义
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
/* 中断寄存器 */
#define PRUINTCBase               0x00004000

#define PRUINTC_REVID             0x000
#define PRUINTC_CONTROL           0x004
#define PRUINTC_GLBLEN            0x010
#define PRUINTC_GLBLNSTLVL        0x01C
#define PRUINTC_STATIDXSET        0x020
#define PRUINTC_STATIDXCLR        0x024
#define PRUINTC_ENIDXSET          0x028
#define PRUINTC_ENIDXCLR          0x02C
#define PRUINTC_HSTINTENIDXSET    0x034
#define PRUINTC_HSTINTENIDXCLR    0x038
#define PRUINTC_GLBLPRIIDX        0x080

#define PRUINTC_STATSETINT0       0x200
#define PRUINTC_STATSETINT1       0x204

#define PRUINTC_STATCLRINT0       0x280
#define PRUINTC_STATCLRINT1       0x284

#define PRUINTC_ENABLESET0        0x300
#define PRUINTC_ENABLESET1        0x304

#define PRUINTC_ENABLECLR0        0x380
#define PRUINTC_ENABLECLR1        0x384

#define PRUINTC_CHANMAP(n)       (0x400 + n * 4)
#define PRUINTC_HOSTMAP(n)       (0x800 + n * 4)

#define PRUINTC_POLARITY0         0xD00
#define PRUINTC_POLARITY1         0xD04

#define PRUINTC_POLARITY0         0xD00
#define PRUINTC_POLARITY1         0xD04

#define PRUINTC_TYPE0             0xD80
#define PRUINTC_TYPE1             0xD84

#define PRUINTC_HOSTINTNSTLVL0   (0x1100 + n * 4)

#define PRUINTC_HOSTINTEN         0x1500

/* 中断状态 */
#define PRU_INT_NOPEND            1
#define PRU_INT_PEND              0

/* 事件 */
/* 外设事件 */
#define SYS_INT_EVENTGROUP0       0
#define SYS_INT_EVENTGROUP1       1

// PRUSSEVTSEL = 0
#define SYS_INT0_Emulation        0
#define SYS_INT0_ECAP0            1
#define SYS_INT0_ECAP1            2
#define SYS_INT0_TIMER64P0OUT12   3
#define SYS_INT0_ECAP2            4
#define SYS_INT0_McASP0TXDMAReq   5
#define SYS_INT0_McASP0RXDMAReq   6
#define SYS_INT0_McBSP0TXDMAReq   7
#define SYS_INT0_McBSP0RXDMAReq   8
#define SYS_INT0_McBSP1TXDMAReq   9
#define SYS_INT0_McBSP1RXDMAReq   10
#define SYS_INT0_SPI0             11
#define SYS_INT0_SPI1             12
#define SYS_INT0_UART0            13
#define SYS_INT0_UART1            14
#define SYS_INT0_I2C0             15
#define SYS_INT0_I2C1             16
#define SYS_INT0_UART2            17
#define SYS_INT0_MMCSD0           18
#define SYS_INT0_MMCSD1           19
#define SYS_INT0_USB0             20
#define SYS_INT0_USB1             21
#define SYS_INT0_TIMER64P0OUT34   22
#define SYS_INT0_ECAP0INPUT       23
#define SYS_INT0_EPWM0            24
#define SYS_INT0_EPWM1            25
#define SYS_INT0_SATA             26
#define SYS_INT0_EDMA3_0_CC0INT2  27
#define SYS_INT0_EDMA3_0_CC0INT3  28
#define SYS_INT0_HPI              29
#define SYS_INT0_EPWM0TZ_EPWM1TZ  30
#define SYS_INT0_McASP0           31

// PRUSSEVTSEL = 1
#define SYS_INT1_Emulation        0
#define SYS_INT1_TIMER64P2CMPEVT0 1
#define SYS_INT1_TIMER64P2CMPEVT1 2
#define SYS_INT1_TIMER64P2CMPEVT2 3
#define SYS_INT1_TIMER64P2CMPEVT3 4
#define SYS_INT1_TIMER64P2CMPEVT4 5
#define SYS_INT1_TIMER64P2CMPEVT5 6
#define SYS_INT1_TIMER64P2CMPEVT6 7
#define SYS_INT1_TIMER64P2CMPEVT7 8
#define SYS_INT1_TIMER64P3CMPEVT0 9
#define SYS_INT1_TIMER64P3CMPEVT1 10
#define SYS_INT1_TIMER64P3CMPEVT2 11
#define SYS_INT1_TIMER64P3CMPEVT3 12
#define SYS_INT1_TIMER64P3CMPEVT4 13
#define SYS_INT1_TIMER64P3CMPEVT5 14
#define SYS_INT1_TIMER64P3CMPEVT6 15
#define SYS_INT1_TIMER64P3CMPEVT7 16
#define SYS_INT1_TIMER64P0CMPEVT  17
#define SYS_INT1_TIMER64P2OUT12   18
#define SYS_INT1_TIMER64P3OUT12   19
#define SYS_INT1_TIMER64P1OUT12   20
#define SYS_INT1_UART1            21
#define SYS_INT1_UART2            22
#define SYS_INT1_SPI0             23
#define SYS_INT1_EPWM0            24
#define SYS_INT1_EPWM1            25
#define SYS_INT1_SPI1             26
#define SYS_INT1_GPIOBANK0        27
#define SYS_INT1_GPIOBANK1        28
#define SYS_INT1_McBSP0TXDMAReq   29
#define SYS_INT1_McBSP0RXDMAReq   30
#define SYS_INT1_McASP0           31

/* 普通事件 */
#define SYS_INT_EVENT32           32
#define SYS_INT_EVENT33           33
#define SYS_INT_EVENT34           34
#define SYS_INT_EVENT35           35
#define SYS_INT_EVENT36           36
#define SYS_INT_EVENT37           37
#define SYS_INT_EVENT38           38
#define SYS_INT_EVENT39           39
#define SYS_INT_EVENT40           40
#define SYS_INT_EVENT41           41
#define SYS_INT_EVENT42           42
#define SYS_INT_EVENT43           43
#define SYS_INT_EVENT44           44
#define SYS_INT_EVENT45           45
#define SYS_INT_EVENT46           46
#define SYS_INT_EVENT47           47
#define SYS_INT_EVENT48           48
#define SYS_INT_EVENT49           49
#define SYS_INT_EVENT50           50
#define SYS_INT_EVENT51           51
#define SYS_INT_EVENT52           52
#define SYS_INT_EVENT53           53
#define SYS_INT_EVENT54           54
#define SYS_INT_EVENT55           55
#define SYS_INT_EVENT56           56
#define SYS_INT_EVENT57           57
#define SYS_INT_EVENT58           58
#define SYS_INT_EVENT59           59
#define SYS_INT_EVENT60           60
#define SYS_INT_EVENT61           61
#define SYS_INT_EVENT62           62
#define SYS_INT_EVENT63           63

/* 主机中断 */
#define HOST_INT0                 0
#define HOST_INT1                 1
#define HOST_INT2                 2
#define HOST_INT3                 3
#define HOST_INT4                 4
#define HOST_INT5                 5
#define HOST_INT6                 6
#define HOST_INT7                 7
#define HOST_INT8                 8
#define HOST_INT9                 9

/* 通道 */
#define INT_CH0                   0
#define INT_CH1                   1
#define INT_CH2                   2
#define INT_CH3                   3
#define INT_CH4                   4
#define INT_CH5                   5
#define INT_CH6                   6
#define INT_CH7                   7
#define INT_CH8                   8
#define INT_CH9                   9

// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      函数声明
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
void IntGlobalEnable();
void IntGlobalDisable();
void IntEventEnable(int event);
void IntEventDisable(int event);
void IntHostEnable(int host);
void IntHostDisable(int host);
void IntEventMap(int event, int ch, int host);
void IntSystemEventClear(int event);
void IntSystemEventEnable(int event);
void IntSystemEventGenerate(int event);
int IntHostIntStatusGet(int host);
void IntEventGroupSet(int group);
void IntHostStatusClear();
void IntPolaritySet(int group, int event, int value);
void IntTypeSet(int group, int event, int value);

#ifdef __cplusplus
}
#endif

#endif
