# nRF52832-MDK 

*An Open Source, Micro Development Kit for IoT Embedded Applications*

[开发文档](https://wiki.makerdiary.co/nrf52832-mdk)

**nRF52832-MDK** 是一款完全开源、基于 [Nordic](http://www.nordicsemi.com/eng) 新一代高性能低功耗蓝牙系统级芯片 [nRF52832](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52832)、面向物联网嵌入式应用的开发套件，集成多功能调试器，支持多种开源软件，支持跨平台开发，并且提供丰富的在线文档，希望给你带来不同于以往的开发体验。

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-boardbox.jpg)

## 完全开源

nRF52832-MDK 是一个开源项目，所有硬件设计资料、源代码、开发文档完全开源，项目托管在 [GitHub](https://github.com/makerdiary/nrf52832-mdk) 上，由社区共同维护，点击页面右上角按钮可前往源码仓库。

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-oshw-1280x976.png)

## nRF52832 SoC

[nRF52832](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52832) 是 Nordic Semiconductor 新一代高性能的低功耗蓝牙系统级芯片，ARM® Cortex™-M4F 内核，512kB Flash + 64kB RAM，支持 BLE, ANT, 2.4GHz 协议栈。

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-chip.png)

## 集成调试器

nRF52832-MDK 集成了多功能调试器 [DAPLink](https://github.com/mbedmicro/DAPLink/)，即插即用，可实现 U 盘直接“拖放”下载固件、串口调试、CMSIS-DAP 仿真调试。

![](https://img.makerdiary.co/wiki/nrf52832mdk/cp-hex-to-daplink.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/daplink-block.png)

## 玩转多种开源软件

nRF52832-MDK 可以用来玩转多种开源软件，包括 [nRF5 SDK](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK), [mbed OS 5](https://developer.mbed.org/), [Zephyr](https://www.zephyrproject.org/), [JerryScript](http://jerryscript.net/), [Espruino](http://www.espruino.com/), [Web Bluetooth](https://webbluetoothcg.github.io/web-bluetooth/), [Eddystone](https://github.com/google/eddystone), [Physical Web](https://google.github.io/physical-web/) 等等....

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-multisw.png)

## 跨平台开发

使用开源工具链，nRF52832-MDK 可实现跨平台开发，满足 Mac OSX, Linux, Windows 开发者的开发需求。

![](https://img.makerdiary.co/wiki/nrf52832mdk/multi-platform.png)

## 提供在线文档

nRF52832-MDK 提供丰富的在线文档，使用 [MkDocs](http://www.mkdocs.org/) 生成文档，托管在 [GitHub](https://github.com/makerdiary/nrf52832-mdk/)，方便开源爱好者一起维护，同时提供 CDN 加速，实现快速访问。

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-online-docs-available.png)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

## License

**MIT License**

Copyright (c) 2017 [创客日记](https://makerdiary.co)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
