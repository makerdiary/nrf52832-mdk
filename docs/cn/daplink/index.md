# Arm Mbed DAPLink<br><small>Arm Mbed 开源的 Cortex 芯片调试工具</small>

## 简介

DAPLink 是 Arm Mbed 开源的、针对 Cortex-M 芯片的调试工具，其目的是通过 Cortex 调试器来实现代码下载到目标设备。

DAPLink 有三个主要特性：

* HID 接口实现，基于 CMSIS-DAP 调试协议，HID 接口允许常用的工具链来对设备进行编程及调试，目前支持的工具链包括 MDK、IAR 及 pyOCD；
* 拖放编程支持，通过将 固件拖放到 USB 磁盘内即实现编程
* 实现 USB 串口通信功能。

了解更多信息，可以访问 [Arm Mbed DAPLink 官网](https://armmbed.github.io/DAPLink/)。

## 功能框图

以下是 DAPLink 的功能框图，使用 Cortex-M0 内核的 LPC11U35 实现：

![](../../daplink/images/daplink_diagram.png)

## 如何更新 DAPLink 固件？

DAPLink 会不断优化以及增加新的功能，你可以通过更新固件来获得这些新功能，按照以下步骤可完成更新：

1. 按下 RESET 按键，连接到 PC，DAPLink 将进入 Bootloader 模式，系统出现名为 `CRP DISABLD` 的可移动磁盘；
2. 删除 `CRP DISABLD` 可移动磁盘中的 `firmware.bin` 文件；
3. 下载[最新的 DAPLink 固件](https://github.com/makerdiary/nrf52832-mdk/tree/master/firmware/daplink)
4. 将固件直接拷贝到 `CRP DISABLD` 可移动磁盘；
5. 拷贝完成后，重新拔插板子，即可完成更新，新版本号从 *DAPLink* 磁盘中描述文件中获得。

## 如何编译 DAPLink?

你也可以自己编译 DAPLink，以下内容将做简单介绍：

### 安装依赖工具

提前安装好以下需要用到的第三方工具：

* [Python 2, 2.7.9+](https://www.python.org/downloads/)
* [Git](https://git-scm.com/downloads)
* [Keil MDK-ARM](https://www.keil.com/download/product/)

### 下载源码

从 Arm Mbed 官方仓库克隆 DAPLink 源码：

``` sh
$ git clone https://github.com/ARMmbed/DAPLink
$ cd DAPLink
$ pip install virtualenv
$ virtualenv venv
```

### 创建编译环境

使用 Python 虚拟环境，安装依赖工具，并生成 Keil 工程：

``` sh
$ ./venv/Scripts/activate
$ pip install -r requirements.txt
$ progen generate -t uvision
$ ./venv/Scripts/deactivate
```

### 编译 DAPLink

在 `projectfiles/uvision/<your-project>` 目录打开 Keil 工程，直接点击<kbd>编译</kbd>按钮即可。

![](../../daplink/images/building_daplink_keil.png)

## 参考资源

* [Arm Mbed DAPLink 仓库](https://github.com/ARMmbed/DAPLink)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 或 [Slack](https://join.slack.com/t/makerdiary/shared_invite/enQtMzIxNTA4MjkwMjc2LTM5MzcyNDhjYjI3YjEwOWE1YzM3YmE0YWEzNGNkNDU3NmE5M2M0MWYyM2QzZTFkNzQ2YjdmMWJlZjIwYmQwMDk) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="indigo"><i class="fa fa-github"></i> 创建 Issue</button></a>

<a href="https://join.slack.com/t/makerdiary/shared_invite/enQtMzIxNTA4MjkwMjc2LTM5MzcyNDhjYjI3YjEwOWE1YzM3YmE0YWEzNGNkNDU3NmE5M2M0MWYyM2QzZTFkNzQ2YjdmMWJlZjIwYmQwMDk"><button data-md-color-primary="red"><i class="fa fa-slack"></i> 加入 Slack</button></a>

