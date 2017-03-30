# 搭建 nRF5 SDK 开发环境：Windows

本小节主要介绍如何在 Windows 系统上快速搭建 nRF5 SDK 开发环境。

!!! tip "说明"
    这里以 Windows 10 作为示例，使用其他 Windows 版本遇到兼容性问题，欢迎反馈。

Windows 对开发者来说有点麻烦，不能原生支持一些开发工具，如我们将需要用到的两个命令行工具：`make` 和 `rm`。要想在 Windows 环境下使用 GNU 的开源工具，最好也是最简单的方式就是在 Windows 上安装一个 Unix 环境。

这里，我们主要基于 [MSYS2](http://www.msys2.org/) (一个专门为 Windows 开发的现代 Unix 环境) 来搭建 nRF5 SDK 的开发环境。

## 1、安装 MSYS2
从 [MSYS2 官网](http://www.msys2.org/) 下载最新版本软件，直接安装：

![](https://img.makerdiary.co/wiki/nrf52832mdk/MSYS2_install.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/MSYS2_installing.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/MSYS2_installed.png)


## 2、安装依赖工具

打开 MSYS2，安装以下工具：

``` sh
$ pacman -S git make gcc diffutils ncurses-devel python2 python3
```

!!! note "提示"
    实际上只需要 `make` 和 `git` 就好了，为了方便后面学习 Zephyr，就一起安装了。

![](https://img.makerdiary.co/wiki/nrf52832mdk/msys2_installing_package1.png)


## 3、安装 GNU ARM Embedded

[GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) 是专门针对 ARM Cortex-M & Cortex-R 系列处理器的交叉工具链，免费开源，现已由 ARM 在维护，可在 [ARM 官网](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)下载最新 Windows 版本，这里以 `gcc-arm-none-eabi-6-2017-q1-update-win32` 版本为例：

下载 `gcc-arm-none-eabi-6-2017-q1-update-win32-zip.zip` 压缩包并解压到 `c:/gccarmemb`，然后验证是否能够工作：

``` sh
# 验证版本
$ cd /c/gccarmemb/bin/
$ ./arm-none-eabi-gcc --version
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/msys2_checking_arm_gcc_version.png)


## 4、配置 nRF5 SDK

下载 Nordic [nRF5 SDK](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK)，SDK 版本会不断更新，可能会出现不同版本之间的兼容性问题，这里以最近发布的版本 `nRF5_SDK_13.0.0_04a0bfd` 作为示例：

将 `nRF5_SDK_13.0.0_04a0bfd.zip` 解压到指定目录，这里使用 `c:/nrf52832-mdk/nrf_sdks/`。

修改 `c:/nrf52832-mdk/nrf_sdks/nRF5_SDK_13.0.0_04a0bfd/components/toolchain/gcc/Makefile.posix` 文件，配置为 `/c/gccarmemb`，修改如下：

``` makefile
# ./nRF5_SDK_13.0.0_04a0bfd/components/toolchaing/gcc/Makefile.posix

GNU_INSTALL_ROOT := /c/gccarmemb
GNU_VERSION := 6.3.1
GNU_PREFIX := arm-none-eabi
```


## 5、编译

随便编译官方开发板 PCA10040 的示例，验证基本配置是否正确：

``` sh
$ cd /c/nrf52832-mdk/nrf_sdks/nRF5_SDK_13.0.0_04a0bfd/examples/peripheral/blinky/pca10040/blank/armgcc
$ make
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/msys2_building_pca10040_blinky.png)


接下来我们就可以编译 nRF52832-MDK 的示例，这里以 `blinky` 为例，源码路径在 `./nrf52832-mdk/examples/nrf5-sdk/blinky`:

``` sh
$ cd ./nrf52832-mdk/examples/nrf5-sdk/blinky
$ make
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/msys2_building_nrf52832_mdk_blinky.png)


## 6、下载运行

生成的 `hex` 文件存放在 `_build` 目录中，将 nRF52832-MDK 连接到 PC，拷贝 `nrf52832_xxaa.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

![](https://img.makerdiary.co/wiki/nrf52832mdk/daplink_disk.png)

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./_build/nrf52832_xxaa.hex

```

!!! warning "提醒"
    在 MSYS2 环境无法实现 pyOCD 下载，可以回到 Windows 环境进行下载。

![](https://img.makerdiary.co/wiki/nrf52832mdk/pyocd-flashtool-blinky-win.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/blinky-demo-latest.jpg)


至此，你已经成功在 Windows 系统下搭建 nRF5 SDK 的开发环境，Enjoy!


## 拓展阅读

在 Windows 平台上，我们还可以使用 [Keil μVision™ IDE](http://www.keil.com/) 以及 [IAR Embedded Workbench](https://www.iar.com/) 进行开发，由于他们都是商业软件，这里不便做详细介绍，后续如果有需要再考虑增加相关章节。建议大家使用开源的工具链，这些工具已经维护得非常好用了，一点都不输商业软件。

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

