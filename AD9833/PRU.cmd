// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      新核科技(广州)有限公司
//
//      Copyright (C) 2022 CoreKernel Technology Guangzhou Co., Ltd
//
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//      PRU 内存映射与分配
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
-cr    // RAM 模型

MEMORY
{
    PAGE 0:
      PRUIRAM:   o = 0x00000000  l = 0x00001000  /*  4KB PRU 程序内存 */

    PAGE 1:
      PRUDRAM:   o = 0x00000000  l = 0x00000200  /* 512B PRU 数据内存 */
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

    // DSP/PRU 共享数据
    .SHARE				>  0x00000000  PAGE 1
}
