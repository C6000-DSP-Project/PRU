// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      �º˿Ƽ�(����)���޹�˾
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
// ������������������������������������������������������������������������������������������������������������������������������������������������������
//
//      PRU �ڴ�ӳ�������
//
//      2022��04��25��
//
// ������������������������������������������������������������������������������������������������������������������������������������������������������
/*
 *    - ϣ����Ĭ(bin wang)
 *    - bin@corekernel.net
 *
 *    ���� corekernel.net/.org/.cn
 *    ���� fpga.net.cn
 *
 */
-cr    // RAM ģ��

MEMORY
{
    PAGE 0:
      PRUIRAM:   o = 0x00000000  l = 0x00001000  /*  4KB PRU �����ڴ� */

    PAGE 1:
      PRUDRAM:   o = 0x00000000  l = 0x00000200  /* 512B PRU �����ڴ� */
}                                              
                                               
SECTIONS                                       
{
    .text:_c_int00*		>  0x00000000  PAGE 0
    .text				>  PRUIRAM     PAGE 0
    .cinit				>  PRUIRAM     PAGE 0
    .stack				>  PRUDRAM     PAGE 1
    .bss				>  PRUDRAM     PAGE 1
    .cio				>  PRUDRAM     PAGE 1
    .const				>  PRUDRAM     PAGE 1
    .data				>  PRUDRAM     PAGE 1
    .switch				>  PRUDRAM     PAGE 1
    .sysmem				>  PRUDRAM     PAGE 1
    .rodata				>  PRUDRAM     PAGE 1
    .fardata			>  PRUDRAM     PAGE 1 ALIGN 4
    .farbss				>  PRUDRAM     PAGE 1
    .rofardata			>  PRUDRAM     PAGE 1 ALIGN 4
}
