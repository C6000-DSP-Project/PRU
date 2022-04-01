/****************************************************************************/
/*                                                                          */
/*              广州创龙电子科技有限公司                                    */
/*                                                                          */
/*              Copyright 2015 Tronlong All rights reserved                 */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*              uPP 设备抽象层函数                                          */
/*                                                                          */
/*              2015年08月19日                                              */
/*                                                                          */
/****************************************************************************/
#include "hw_upp.h"
#include "debug.h"
#include "hw_types.h"
#include "uPP.h"

#include "soc_C6748.h"
#include "hw_syscfg0_C6748.h"
#include "hw_syscfg1_C6748.h"	    // 系统配置模块寄存器

#include <stdio.h>
#include <stdlib.h>

/****************************************************************************/
/*                                                                          */
/*              宏定义                                                      */
/*                                                                          */
/****************************************************************************/
static void Delay(volatile unsigned int count);

/****************************************************************************/
/*                                                                          */
/*              管脚复用配置                                                */
/*                                                                          */
/****************************************************************************/
void uPPPinMuxSetup(unsigned char OperatingMode)
{
//	uPPGetVersion();

	volatile unsigned int savePinMux = 0;

	// 数据管脚复用配置
	switch(OperatingMode)
	{
		case uPP_CHA_8BIT:
		{
			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & ~(0xFFFF0000);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (0x44440000 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) & ~(0xFFFFFF00);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) = (0x44444400 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) & ~(0x000000FF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) = (0x00000044 | savePinMux);
			//控制引脚设置为下拉
			HWREG(SOC_SYSCFG_1_REGS + SYSCFG1_PUPD_SEL) &= ~(1<<24);
			break;
		}

		case uPP_CHA_16BIT:
		{
			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & ~(0xFFFF0000);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (0x44440000 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) & ~(0xFFFFFF00);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) = (0x44444400 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) & ~(0xFFFFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) = (0x44444444 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) & ~(0x000000FF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) = (0x00000044 | savePinMux);
			//控制引脚设置为下拉
			HWREG(SOC_SYSCFG_1_REGS + SYSCFG1_PUPD_SEL) &= ~(1<<24);
			break;
		}

		case uPP_CHA_8BIT_CHB_8BIT:
		{
			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & ~(0xFFFF0000);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (0x44440000 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) & ~(0xFFFFFF00);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) = (0x44444400 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) & ~(0xFFFFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) = (0x44444444 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) & ~(0x000000FF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) = (0x00000044 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) & ~(0x00FFFF00);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) = (0x00444400 | savePinMux);
			//控制引脚设置为下拉
			HWREG(SOC_SYSCFG_1_REGS + SYSCFG1_PUPD_SEL) &= ~(1<<24 | 1<<30);
			break;
		}

		case uPP_CHA_8BIT_CHB_16BIT:
		{
			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & ~(0xFFFF0000);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (0x44440000 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) & ~(0xFFFFFF00);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) = (0x44444400 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) & ~(0xFFFFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) = (0x44444444 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) & ~(0xFFFFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) = (0x44444444 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(17)) & ~(0x000000FF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(17)) = (0x00000044 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) & ~(0x00FFFF00);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) = (0x00444400 | savePinMux);
			//控制引脚设置为下拉
			HWREG(SOC_SYSCFG_1_REGS + SYSCFG1_PUPD_SEL) &= ~(1<<24 | 1<<30);
			break;
		}

		case uPP_CHA_16BIT_CHB_8BIT:
		{
			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & ~(0xFFFF0000);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (0x44440000 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) & ~(0xFFFFFF00);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) = (0x44444400 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) & ~(0xFFFFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) = (0x44444444 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) & ~(0x000000FF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) = (0x00000044 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(17)) & ~(0xFFFFFF00);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(17)) = (0x44444400 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) & ~(0x00FFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) = (0x00444444 | savePinMux);
			//控制引脚设置为下拉
			HWREG(SOC_SYSCFG_1_REGS + SYSCFG1_PUPD_SEL) &= ~(1<<24 | 1<<30);
			break;
		}

		case uPP_CHA_16BIT_CHB_16BIT:
		{
			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) & ~(0xFFFF0000);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(13)) = (0x44440000 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) & ~(0xFFFFFF00);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(14)) = (0x44444400 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) & ~(0xFFFFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(15)) = (0x44444444 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) & ~(0xFFFFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(16)) = (0x44444444 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(17)) & ~(0xFFFFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(17)) = (0x44444444 | savePinMux);

			savePinMux = HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) & ~(0x00FFFFFF);
			HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(18)) = (0x00444444 | savePinMux);
			//控制引脚设置为下拉
			HWREG(SOC_SYSCFG_1_REGS + SYSCFG1_PUPD_SEL) &= ~(1<<24 | 1<<30);
			break;
		}

		default:
		{
			printf("\r\nuPPPinMuxSetup: unsupported Operating Mode!\r\n");
		}
	}
}

/****************************************************************************/
/*                                                                          */
/*              时钟配置                                                    */
/*                                                                          */
/****************************************************************************/
void uPPClkConfig(unsigned int baseAddr, unsigned char channel, unsigned int Clk,
		unsigned int moduleClk, unsigned char polarity)
{
	// 时钟只在发送模式有效
	// 接收模式的最大时钟亦不能超过 CPU 时钟四分之一
    unsigned int clkDiv;
    clkDiv = moduleClk / Clk / 2 - 1;

    // 分频系数必须满足这个范围
    ASSERT(((clkDiv >= 0) && (clkDiv <= 15)));

    if(channel == uPP_CHA)
    {
    	HWREG(baseAddr + UPP_UPICR) &= ~(UPP_UPICR_CLKINVA | UPP_UPICR_CLKDIVA);
    	HWREG(baseAddr + UPP_UPICR) |= (clkDiv <<  UPP_UPICR_CLKDIVA_SHIFT) \
    									| (polarity << UPP_UPICR_CLKINVA_SHIFT);
    }
    else if(channel == uPP_CHB)
    {
    	HWREG(baseAddr + UPP_UPICR) &= ~(UPP_UPICR_CLKINVB | UPP_UPICR_CLKDIVB);
    	HWREG(baseAddr + UPP_UPICR) |= (clkDiv <<  UPP_UPICR_CLKDIVB_SHIFT) \
										| (polarity << UPP_UPICR_CLKINVB_SHIFT);
    }
}

/****************************************************************************/
/*                                                                          */
/*              外设使能                                                    */
/*                                                                          */
/****************************************************************************/
void uPPEnable(unsigned int baseAddr)
{
   	HWREG(baseAddr + UPP_UPPCR) |= ((0x00000001u) <<  UPP_UPPCR_EN_SHIFT);
}

/****************************************************************************/
/*                                                                          */
/*              外设禁用                                                    */
/*                                                                          */
/****************************************************************************/
void uPPDisable(unsigned int baseAddr)
{
   	HWREG(baseAddr + UPP_UPPCR) &= ~((0x00000001u) <<  UPP_UPPCR_EN_SHIFT);
}

/****************************************************************************/
/*                                                                          */
/*              外设配置                                                    */
/*                                                                          */
/****************************************************************************/
void uPPPeripheralConfig(unsigned int baseAddr, unsigned int config)
{
	HWREG(baseAddr + UPP_UPPCR) |= config;
}

/****************************************************************************/
/*                                                                          */
/*              数据格式配置                                                */
/*                                                                          */
/****************************************************************************/
void uPPDataFmtConfig(unsigned int baseAddr, unsigned char channel, unsigned int config)
{
	if(channel == uPP_CHA)
	{
		config <<= 16;
		HWREG(baseAddr + UPP_UPCTL) |= (config & (0x007F0000));
	}
	else if(channel == uPP_CHB)
	{
		config <<= 24;
		HWREG(baseAddr + UPP_UPCTL) |= (config & (0x7F000000));
	}
}

/****************************************************************************/
/*                                                                          */
/*              通道配置                                                    */
/*                                                                          */
/****************************************************************************/
void uPPChannelConfig(unsigned int baseAddr, unsigned int config)
{
	HWREG(baseAddr + UPP_UPCTL) |= (config & (0x0000001F));
}

/****************************************************************************/
/*                                                                          */
/*              管脚配置                                                    */
/*                                                                          */
/****************************************************************************/
void uPPPinConfig(unsigned int baseAddr, unsigned char channel, unsigned int config)
{
	if(channel == uPP_CHA)
	{
		HWREG(baseAddr + UPP_UPICR) |= (config & (0x00003038));
	}
	else if(channel == uPP_CHB)
	{
		config <<= 16;
		HWREG(baseAddr + UPP_UPICR) |= (config & (0x30380000));
	}
}

/****************************************************************************/
/*                                                                          */
/*              管脚相位配置                                                */
/*                                                                          */
/****************************************************************************/
void uPPPinPolarityConfig(unsigned int baseAddr, unsigned char channel, unsigned int config)
{
	if(channel == uPP_CHA)
	{
		HWREG(baseAddr + UPP_UPICR) |= (config & (0x00000007));
	}
	else if(channel == uPP_CHB)
	{
		config <<= 16;
		HWREG(baseAddr + UPP_UPICR) |= (config & (0x00070000));
	}
}

/****************************************************************************/
/*                                                                          */
/*              接口空闲输出配置                                            */
/*                                                                          */
/****************************************************************************/
void uPPIdleValueConfig(unsigned int baseAddr, unsigned char channel, unsigned int config)
{
    if(channel == uPP_CHA)
    {
    	HWREG(baseAddr + UPP_UPIVR) |= (config <<  UPP_UPIVR_VALA_SHIFT);
    }
    else if(channel == uPP_CHB)
    {
    	HWREG(baseAddr + UPP_UPIVR) |= (config <<  UPP_UPIVR_VALB_SHIFT);
    }
}

/****************************************************************************/
/*                                                                          */
/*              门限配置                                                    */
/*                                                                          */
/****************************************************************************/
void uPPThresholdConfig(unsigned int baseAddr, unsigned char channel, unsigned int config)
{
	switch (channel)
	{
		case uPP_CHA:
			HWREG(baseAddr + UPP_UPTCR) |= (config <<  UPP_UPTCR_TXSIZEA_SHIFT);
			break;
		case uPP_CHB:
			HWREG(baseAddr + UPP_UPTCR) |= (config <<  UPP_UPTCR_TXSIZEB_SHIFT);
			break;
		case uPP_DMA_CHI:
			HWREG(baseAddr + UPP_UPTCR) |= (config <<  UPP_UPTCR_RDSIZEI_SHIFT);
			break;
		case uPP_DMA_CHQ:
			HWREG(baseAddr + UPP_UPTCR) |= (config <<  UPP_UPTCR_RDSIZEQ_SHIFT);
			break;

	}
}

/****************************************************************************/
/*                                                                          */
/*              数字回环配置                                                */
/*                                                                          */
/****************************************************************************/
void uPPDLBConfig(unsigned int baseAddr, unsigned int config)
{
	HWREG(baseAddr + UPP_UPDLB) &= ~0x00003000u;
	HWREG(baseAddr + UPP_UPDLB) |= config;

	if(config == UPP_UPDLB_BA)
	{
		HWREG(baseAddr + UPP_UPCTL) &= ~0x00000003u;
		HWREG(baseAddr + UPP_UPCTL) |=0x00000002u;
	}
	else if(config == UPP_UPDLB_AB)
	{
		HWREG(baseAddr + UPP_UPCTL) &= ~0x00000003u;
		HWREG(baseAddr + UPP_UPCTL) |= 0x00000003u;
	}
}

/****************************************************************************/
/*                                                                          */
/*              中断使能                                                    */
/*                                                                          */
/****************************************************************************/
void uPPIntEnable(unsigned int baseAddr, unsigned char DMAchannel, unsigned int intFlags)
{
	if(DMAchannel == uPP_DMA_CHI)
	{
		HWREG(baseAddr + UPP_UPIES) = intFlags;
	}
	else if(DMAchannel == uPP_DMA_CHQ)
	{
		HWREG(baseAddr + UPP_UPIES) = (intFlags <<  8);
	}
}

/****************************************************************************/
/*                                                                          */
/*              中断禁用                                                    */
/*                                                                          */
/****************************************************************************/
void uPPIntDisable(unsigned int baseAddr, unsigned char DMAchannel, unsigned int intFlags)
{
	if(DMAchannel == uPP_DMA_CHI)
	{
		HWREG(baseAddr + UPP_UPIEC) = intFlags;
	}
	else if(DMAchannel == uPP_DMA_CHQ)
	{
		HWREG(baseAddr + UPP_UPIEC) = (intFlags << 8);
	}
}

/****************************************************************************/
/*                                                                          */
/*              中断清除                                                    */
/*                                                                          */
/****************************************************************************/
void uPPIntClear(unsigned int baseAddr, unsigned char DMAchannel, unsigned int intFlags)
{
	if(DMAchannel == uPP_DMA_CHI)
	{
		HWREG(baseAddr + UPP_UPIER) = intFlags;
	}
	else if(DMAchannel == uPP_DMA_CHQ)
	{
		HWREG(baseAddr + UPP_UPIER) = (intFlags <<  8);
	}
}

/****************************************************************************/
/*                                                                          */
/*              中断状态                                                    */
/*                                                                          */
/****************************************************************************/
unsigned int uPPIntStatus(unsigned int baseAddr, unsigned char DMAchannel)
{
	unsigned int intFlags;

	if(DMAchannel == uPP_DMA_CHI)
	{
		intFlags = HWREG(baseAddr + UPP_UPIER) & 0x0000001F;
	}
	else if(DMAchannel == uPP_DMA_CHQ)
	{
		intFlags = HWREG(baseAddr + UPP_UPIER) & 0x00001F00;
		intFlags >>=  8;
	}
	return (intFlags);
}

/****************************************************************************/
/*                                                                          */
/*              确认中断                                                    */
/*                                                                          */
/****************************************************************************/
void uPPEndOfInt(unsigned int baseAddr)
{
   	HWREG(baseAddr + UPP_UPEOI) = 0x00000000u;
}
/****************************************************************************/
/*                                                                          */
/*              DMA 传输                                                    */
/*                                                                          */
/****************************************************************************/
void uPPDMATransfer(unsigned int baseAddr, unsigned char DMAchannel, uPPDMAConfig *config)
{
	if(DMAchannel == uPP_DMA_CHI)
	{
		HWREG(baseAddr + UPP_UPID0) = (int)config->WindowAddress;
		HWREG(baseAddr + UPP_UPID1) = (config->LineCount << 16 ) | (config->ByteCount);
		HWREG(baseAddr + UPP_UPID2) = config->LineOffsetAddress;
	}
	else if(DMAchannel == uPP_DMA_CHQ)
	{
		HWREG(baseAddr + UPP_UPQD0) = (int)config->WindowAddress;
		HWREG(baseAddr + UPP_UPQD1) = (config->LineCount << 16 ) | (config->ByteCount);
		HWREG(baseAddr + UPP_UPQD2) = config->LineOffsetAddress;
	}
}

/****************************************************************************/
/*                                                                          */
/*              DMA 状态                                                    */
/*                                                                          */
/****************************************************************************/
uPPDMAStatus *uPPGetDMAStatus(unsigned int baseAddr, unsigned char channel)
{
	uPPDMAStatus * pDMAStatus;

	pDMAStatus = calloc(1, sizeof(uPPDMAStatus));

	if(channel == uPP_DMA_CHI)
	{
		pDMAStatus->WindowAddress = HWREG(baseAddr + UPP_UPIS0);
		pDMAStatus->LineCount     = (HWREG(baseAddr + UPP_UPIS1) >> 16) & 0x0000FFFF;
		pDMAStatus->ByteCount     = HWREG(baseAddr + UPP_UPIS1) & 0x0000FFFF;
		pDMAStatus->FIFO          = (HWREG(baseAddr + UPP_UPIS2) >> 4) & 0x0000000F;
		pDMAStatus->PEND          = (HWREG(baseAddr + UPP_UPIS2) >> 1) & 0x00000001;
		pDMAStatus->DMA           = HWREG(baseAddr + UPP_UPIS2) & 0x00000001;
	}
	else if(channel == uPP_DMA_CHQ)
	{
		pDMAStatus->WindowAddress = HWREG(baseAddr + UPP_UPQS0);
		pDMAStatus->LineCount     = (HWREG(baseAddr + UPP_UPQS1) >> 16) & 0x0000FFFF;
		pDMAStatus->ByteCount     = HWREG(baseAddr + UPP_UPQS1) & 0x0000FFFF;
		pDMAStatus->FIFO          = (HWREG(baseAddr + UPP_UPQS2) >> 4) & 0x0000000F;
		pDMAStatus->PEND          = (HWREG(baseAddr + UPP_UPQS2) >> 1) & 0x00000001;
		pDMAStatus->DMA           = HWREG(baseAddr + UPP_UPQS2) & 0x00000001;
	}

	return (pDMAStatus);
}

/****************************************************************************/
/*                                                                          */
/*              uPP 复位                                                    */
/*                                                                          */
/****************************************************************************/
void uPPReset(unsigned int baseAddr)
{
	// 失能 uPP
	uPPDisable(baseAddr);
	// 等待 DMA 空闲
	while(HWREG(baseAddr + UPP_UPPCR) & (1 << 7));
	// 复位
	HWREG(baseAddr + UPP_UPPCR) &= ~0x00000010u;
	HWREG(baseAddr + UPP_UPPCR) |= 0x00000001u << 0x00000004u;
	Delay(0xffff);
	HWREG(baseAddr + UPP_UPPCR) &= ~0x00000010u;
	// 使能 uPP
	uPPEnable(baseAddr);
}

/****************************************************************************/
/*                                                                          */
/*              延时                                                        */
/*                                                                          */
/****************************************************************************/
void Delay(volatile unsigned int count)
{
    while(count--);
}

/****************************************************************************/
/*                                                                          */
/*              版本                                                        */
/*                                                                          */
/****************************************************************************/
void uPPGetVersion(void)
{
    printf("\r\n Version 1.01.0814\r\n");
    printf("\r\n DSP C6748 Project\r\n");
    printf("\r\n (C) 2015 Guangzhou Tronlong Electronic Technology Co.,Ltd\r\n");
}
