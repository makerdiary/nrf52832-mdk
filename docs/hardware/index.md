# 硬件介绍

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-boardbox.jpg)

## 硬件规格

* 搭载 Nordic nRF52 系列低功耗蓝牙 SoC nRF52832

    * ARM® Cortex™-M4F
    * 512kB Flash Memory + 64kB RAM
    * 支持 Bluetooth low energy, ANT 以及 2.4GHz
    * 支持 NFC™-A tag

* 集成开源仿真器 [DAPLink](https://github.com/mbedmicro/DAPLink)
  
    * 免驱动 USB HID, 即插即用，Mac OSX, Linux, Windows 全平台支持
    * 支持 [pyOCD](https://github.com/mbedmicro/pyOCD), [Keil uVision](http://www.keil.com/), [IAR](https://www.iar.com/) 等上位机调试工具
    * U盘直接“拖放式”下载程序，支持 hex、bin 文件格式
    * USB 虚拟串口，波特率高达 230400bps
    * 可更新升级固件

* 24个可用 IO 全部引出，可灵活复用 ADC, SPI, I2C, UART, PDM, PWM 等外设

* 板载复位按钮

* 板载可编程 RGB LED

* 板载 2.4GHz 陶瓷天线

* 支持 USB 供电 或 外部电源供电

* 支持 3.3V 800mA(Max.) 电源输出，可为外部模块供电

* 面包板友好连接

* 尺寸：4.8 * 1.8 cm

## 功能框图

以下为 nRF52832-MDK 的硬件功能框图：
![](https://img.makerdiary.co/wiki/nrf52832mdk/nRF52832-PINOUT.png)

## DAPLink
nRF52832-MDK 板上集成了 [DAPLink](https://github.com/mbedmicro/DAPLink)，这是 ARM mbed 开源的多功能仿真器，内部拥有三种接口：

* MSC - 可实现 U 盘直接“拖放式”下载固件
* CDC - 虚拟串口，可用于输出调试信息、与上位机通信
* HID - 兼容 CMSIS-DAP 的调试接口

对应功能框图如下：

![](https://img.makerdiary.co/wiki/nrf52832mdk/DAPLink-Block-Diagram.png)


## 硬件资源

* [nRF52832-MDK 原理图](nRF52832-MDK_SCH_V1.0.pdf)
* [nRF52832-MDK 元件位置图](nRF52832-MDK_ASSEMBLY-TOP_V1.0.pdf)
* [nRF52832-MDK PCB](nRF52832-MDK_PCB_V1.0.pdf)
* [nRF52832-MDK 3D 模型](nRF52832-MDK_3D_STEP.step)

!!! note "提示"
    原理图及 PCB 源文件因设计软件版权问题只能暂时以 pdf 格式发布，后续将使用开源软件 [KiCad](http://kicad-pcb.org/) 重新设计发布。

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

