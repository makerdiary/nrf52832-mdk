# 搭建 nRF5 SDK 开发环境：Mac OSX

本小节主要介绍如何在 Mac OSX 系统上快速搭建 nRF5 SDK 开发环境。

## 1、安装 GCC 工具链
[GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) 是专门针对 ARM Cortex-M & Cortex-R 系列处理器的交叉工具链，免费开源，现已由 ARM 在维护，可在 [ARM 官网](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)下载最新 OSX 版本，这里以 `gcc-arm-none-eabi-6-2017-q1-update` 版本为例：

将下载完的压缩包解压到你自己指定的目录，并验证是否能够工作：

``` sh
# 解压到指定目录
$ cd your_toolchain_path/
$ tar -xvf gcc-arm-none-eabi-6-2017-q1-update-mac.tar.bz2

# 验证版本
$ cd your_toolchain_path/gcc-arm-none-eabi-6-2017-q1-update/bin/
$ ./arm-none-eabi-gcc --version
```
![](https://img.makerdiary.co/wiki/nrf52832mdk/checking-arm-gcc-version.png)

## 2、配置 nRF5 SDK

下载 Nordic [nRF5 SDK](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK)，SDK 版本会不断更新，可能会出现不同版本之间的兼容性问题，这里以最近发布的版本 `nRF5_SDK_13.0.0_04a0bfd` 作为示例：

将 `nRF5_SDK_13.0.0_04a0bfd.zip` 解压到指定目录，这里使用 `./nrf52832-mdk/nrf_sdks/`：

``` sh
# 解压到指定目录
$ cd ./nrf52832-mdk/nrf_sdks/
$ unzip nRF5_SDK_13.0.0_04a0bfd.zip -d ./nRF5_SDK_13.0.0_04a0bfd

```
修改 `./nRF5_SDK_13.0.0_04a0bfd/components/toolchain/gcc/Makefile.posix` 文件，配置为 GCC ARM 工具链的绝对路径，修改如下：

``` makefile
# ./nRF5_SDK_13.0.0_04a0bfd/components/toolchain/gcc/Makefile.posix

GNU_INSTALL_ROOT := your_toolchain_path/gcc-arm-none-eabi-6-2017-q1-update
GNU_VERSION := 6.3.1
GNU_PREFIX := arm-none-eabi
```

## 3、编译

尝试编译官方开发板 PCA10040 的示例，验证基本配置是否正确：

``` sh
$ cd ./nRF5_SDK_13.0.0_04a0bfd/examples/peripheral/blinky/pca10040/blank/armgcc
$ make
```

编译成功：

![](https://img.makerdiary.co/wiki/nrf52832mdk/pca10040-blinky.png)

接下来就可以编译 nRF52832-MDK 的示例，这里以 `blinky` 为例，源码路径在 `./nrf52832-mdk/examples/nrf5-sdk/blinky`:

``` sh
$ cd ./nrf52832-mdk/examples/nrf5-sdk/blinky
$ make
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-blinky-osx.png)

## 4、下载运行

生成的 `hex` 文件存放在 `_build` 目录中，将硬件连接到 PC，拷贝 `nrf52832_xxaa.hex` 到 **DAPLINK** 可移动磁盘，下载完成，复位即可运行程序。

或者直接运行以下命令：

!!! note "提醒"
    可以参考前面章节：[如何下载固件？](/getting-started/how-to-upload/)


``` sh
$ make flash
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/flashing-blinky-osx.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/blinky-demo-latest.jpg)

至此，你已经成功在 Mac OSX 上搭建了 nRF5 SDK 的开发环境，Enjoy!

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

