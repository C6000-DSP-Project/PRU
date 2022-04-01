/****************************************************************************/
/*                                                                          */
/*    新核科技(广州)有限公司                                                */
/*                                                                          */
/*    Copyright (C) 2022 CoreKernel Technology (Guangzhou) Co., Ltd         */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*    LED 点阵                                                              */
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
#include "hw_syscfg0_C6748.h"

#include "soc_C6748.h"

#include "gpio.h"

/****************************************************************************/
/*                                                                          */
/*              延时                                                        */
/*                                                                          */
/****************************************************************************/
void Delay(unsigned int n)
{
    unsigned int i;

    for(i = n; i > 0; i--);
}

/****************************************************************************/
/*                                                                          */
/*              WS2812B 输出                                                */
/*                                                                          */
/****************************************************************************/
#define GPIO_OUT_DATA01_REG (0x01E26000 + 0x14)

void IOSet()
{
    HWREG(GPIO_OUT_DATA01_REG) |= (1 << 5);             // 高电平
    Delay(14);

    HWREG(GPIO_OUT_DATA01_REG) &= ~(1 << 5);            // 低电平
    Delay(6);
}

void IOClear()
{
    HWREG(GPIO_OUT_DATA01_REG) |= (1 << 5);             // 高电平
    Delay(5);

    HWREG(GPIO_OUT_DATA01_REG) &= ~(1 << 5);            // 低电平
    Delay(15);
}

void IOReset()
{
    HWREG(GPIO_OUT_DATA01_REG) &= ~(1 << 5);            // 低电平
    Delay(1200);
}

void WriteRGB(unsigned char R, unsigned char G, unsigned char B)
{
    unsigned char RGBBuf[24];

    unsigned char i;

    for(i = 0; i < 8; i++)
    {
       RGBBuf[i] = (G >> i) & 0x01;
       RGBBuf[i + 8] = (R >> i) & 0x01;
       RGBBuf[i + 16] = (B >> i) & 0x01;
    }

    for(i = 0; i < 24; i++)
    {
          RGBBuf[i] ? IOSet() : IOClear();
    }

    IOReset();
}

/****************************************************************************/
/*                                                                          */
/*              LED 点阵                                                    */
/*                                                                          */
/****************************************************************************/
unsigned char LEDMatrixRGB[64][3] =
{
    // R G B
    {255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {139, 0, 255}, {255, 255, 255},
    {255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {139, 0, 255}, {255, 255, 255},
    {255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {139, 0, 255}, {255, 255, 255},
    {255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {139, 0, 255}, {255, 255, 255},
    {255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {139, 0, 255}, {255, 255, 255},
    {255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {139, 0, 255}, {255, 255, 255},
    {255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {139, 0, 255}, {255, 255, 255},
    {255, 0, 0}, {255, 165, 0}, {255, 255, 0}, {0, 255, 0}, {0, 255, 255}, {0, 0, 255}, {139, 0, 255}, {255, 255, 255}
};

unsigned char Brightness = 5;

void LEDMatrixON()
{
    unsigned char i;
    for(i = 0; i < 64; i++)
    {
        WriteRGB(255, 255, 255);
    }
}

void LEDMatrixOFF()
{
    unsigned char i;
    for(i = 0; i < 64; i++)
    {
        WriteRGB(0, 0, 0);
    }
}

void LEDMatrix()
{
    unsigned char i;
    for(i = 0; i < 64; i++)
    {
        WriteRGB(LEDMatrixRGB[i][0], LEDMatrixRGB[i][1], LEDMatrixRGB[i][2]);
    }
}

void LEDMatrixBrightness()
{
    unsigned char i;
    unsigned int SumR, SumG, SumB;
    unsigned int MeanR, MeanG, MeanB;

    for(i = 0; i < 64; i++)
    {
        SumR += LEDMatrixRGB[i][0];
        SumG += LEDMatrixRGB[i][1];
        SumB += LEDMatrixRGB[i][2];
    }

    MeanR = SumR /64;
    MeanG = SumG /64;
    MeanB = SumB /64;

    for(i = 0; i < 64; i++)
    {
        LEDMatrixRGB[i][0] = (LEDMatrixRGB[i][0] ? LEDMatrixRGB[i][0] - MeanR : 0) * (1 + (MeanR * Brightness / 10));
        LEDMatrixRGB[i][1] = (LEDMatrixRGB[i][1] ? LEDMatrixRGB[i][1] - MeanG : 0) * (1 + (MeanG * Brightness / 10));
        LEDMatrixRGB[i][2] = (LEDMatrixRGB[i][1] ? LEDMatrixRGB[i][2] - MeanB : 0) * (1 + (MeanB * Brightness / 10));
    }
}

/****************************************************************************/
/*                                                                          */
/*              主函数                                                      */
/*                                                                          */
/****************************************************************************/
int main()
{
    // 管脚复用配置 GPIO0[5]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) & (~(SYSCFG_PINMUX1_PINMUX1_11_8))) |
                                                    (SYSCFG_PINMUX1_PINMUX1_11_8_GPIO0_5 << SYSCFG_PINMUX1_PINMUX1_11_8_SHIFT);

    // 管脚初始化
    GPIODirModeSet(SOC_GPIO_0_REGS, 6, GPIO_DIR_OUTPUT);   // GPIO0[5]

    // 主循环
    for(;;)
    {
        // 调整亮度
//      LEDMatrixBrightness();

        LEDMatrix();

        IOReset();
    }
}
