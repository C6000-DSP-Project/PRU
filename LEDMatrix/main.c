/****************************************************************************/
/*                                                                          */
/*    �º˿Ƽ�(����)���޹�˾                                                */
/*                                                                          */
/*    Copyright (C) 2022 CoreKernel Technology (Guangzhou) Co., Ltd         */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*    LED ����                                                              */
/*                                                                          */
/*    2022��03��28��                                                        */
/*                                                                          */
/****************************************************************************/
/*
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

#include "gpio.h"

/****************************************************************************/
/*                                                                          */
/*              ��ʱ                                                        */
/*                                                                          */
/****************************************************************************/
void Delay(unsigned int n)
{
    unsigned int i;

    for(i = n; i > 0; i--);
}

/****************************************************************************/
/*                                                                          */
/*              WS2812B ���                                                */
/*                                                                          */
/****************************************************************************/
#define GPIO_OUT_DATA01_REG (0x01E26000 + 0x14)

void IOSet()
{
    HWREG(GPIO_OUT_DATA01_REG) |= (1 << 5);             // �ߵ�ƽ
    Delay(14);

    HWREG(GPIO_OUT_DATA01_REG) &= ~(1 << 5);            // �͵�ƽ
    Delay(6);
}

void IOClear()
{
    HWREG(GPIO_OUT_DATA01_REG) |= (1 << 5);             // �ߵ�ƽ
    Delay(5);

    HWREG(GPIO_OUT_DATA01_REG) &= ~(1 << 5);            // �͵�ƽ
    Delay(15);
}

void IOReset()
{
    HWREG(GPIO_OUT_DATA01_REG) &= ~(1 << 5);            // �͵�ƽ
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
/*              LED ����                                                    */
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
/*              ������                                                      */
/*                                                                          */
/****************************************************************************/
int main()
{
    // �ܽŸ������� GPIO0[5]
    HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) = (HWREG(SOC_SYSCFG_0_REGS + SYSCFG0_PINMUX(1)) & (~(SYSCFG_PINMUX1_PINMUX1_11_8))) |
                                                    (SYSCFG_PINMUX1_PINMUX1_11_8_GPIO0_5 << SYSCFG_PINMUX1_PINMUX1_11_8_SHIFT);

    // �ܽų�ʼ��
    GPIODirModeSet(SOC_GPIO_0_REGS, 6, GPIO_DIR_OUTPUT);   // GPIO0[5]

    // ��ѭ��
    for(;;)
    {
        // ��������
//      LEDMatrixBrightness();

        LEDMatrix();

        IOReset();
    }
}
