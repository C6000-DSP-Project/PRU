C6748 PRU 核心例程  

CCSv7 IDE  
- 7.4 不依赖 CCS IDE 版本  
      但是工程基于 CCSv7 创建 低于该版本 CCS （但不能低于 CCSv6）需要重新创建工程  
    
编译器  
- TI PRU CGT 2.3.3  

依赖组件  
- 依赖库位于 Library 目录  

测试方法  

核心板  

EVM-CKL138PKT 底板  

点阵模块硬件连接  
- 直接将点阵模块插入 CON16 即可  

- LEDMatrix            点亮点阵模块 64 个 LED  
                       [1] 可以通过修改 unsigned char LEDMatrixRGB[64][3] 变量 RGB 值改变颜色和亮度  
					   [2] DSP/ARM 核心可以通过修改内存地址 (0x01C30000 + (n * 0x2000))（n 表示 PRU 处理器 ID）改变该变量值  
					   
程序固化  

PRU 程序需要通过 DSP/ARM 加载运行  
[1] 工程编译成功后自动生成 PRU_Data.h/PRU_Code.h 文件  
[2] 将这两个文件加入到 DSP 工程中加载运行即可  
