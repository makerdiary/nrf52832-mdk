# 搭建 Zephyr 开发环境：Windows

本小节主要介绍如何在 Windows 系统上快速搭建 Zephyr 开发环境。

!!! tip "说明"
    如果你之前已经按照[搭建 nRF5 SDK 开发环境](/nrf5sdk/setup-devenv-win/)的介绍配置好了 MSYS2，以下 1、2、3 步骤可跳过。

Windows 对开发者来说有点麻烦，不能原生支持一些开发工具，如我们将需要用到的两个命令行工具：`make` 和 `rm`。要想在 Windows 环境下使用 GNU 的开源工具，最好也是最简单的方式就是在 Windows 上安装一个 Unix 环境。

这里，我们主要基于 [MSYS2](http://www.msys2.org/) (一个专门为 Windows 开发的现代 Unix 环境) 来搭建 Zephyr 的开发环境。

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

为交叉工具链配置环境变量：

``` sh
$ export ZEPHYR_GCC_VARIANT=gccarmemb
$ export GCCARMEMB_TOOLCHAIN_PATH=/c/gccarmemb
```

也可以将以上命令添加到 `~/.bash_profile` 文件中，这样就不用每次都配置环境变量：


``` sh
# 在~/.bash_profile文件最后添加以下命令：
export ZEPHYR_GCC_VARIANT=gccarmemb
export GCCARMEMB_TOOLCHAIN_PATH=/c/gccarmemb

# 使其生效
$ source .bash_profile

# 使用 echo 命令验证是否设置正确：
$ echo $ZEPHYR_GCC_VARIANT
$ echo $GCCARMEMB_TOOLCHAIN_PATH
```

## 4、克隆 Zephyr 源码

Zephyr 源码托管在 [https://gerrit.zephyrproject.org/r/zephyr](https://gerrit.zephyrproject.org/r/zephyr)，这里为了使用方便，我在 GitHub 做了镜像，并增加 `nrf52832-mdk` 分支来支持 nRF52832-MDK 的开发。

``` sh
# 克隆 Zephyr 源码 到 zephyr-project
$ git clone https://github.com/makerdiary/zephyr.git zephyr-project
$ cd zephyr-project/

# 拉取 nrf52832-mdk 分支的最新代码
$ git pull origin nrf52832-mdk

# 为 Zephyr 配置环境变量
$ unset ZEPHYR_SDK_INSTALL_DIR
$ source ~/zephyr/zephyr-env.sh
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/msys2_clone_config_zephyr.png)


## 5、编译

一切顺利的话，现在就可以编译了，以编译 `./zephyr-project/samples/hello_world` 为例：

``` sh
$ cd ./zephyr-project/samples/hello_world

$ make BOARD=nrf52832_mdk

```

成功编译，输出固件：

![](https://img.makerdiary.co/wiki/nrf52832mdk/msys2_compile_zephyr_helloworld.png)

## 6、下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `/outdir/nrf52832_mdk/zephyr.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./outdir/nrf52832_mdk/zephyr.hex

```

!!! warning "提醒"
    在 MSYS2 环境无法实现 pyOCD 下载，可以回到 Windows 环境进行下载。

![](https://img.makerdiary.co/wiki/nrf52832mdk/pyocd-flashtool-zephyr-win.png)


[PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) 打开串口，查看输出：

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_helloworld_putty_config.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_helloworld_output_win.png)


至此，你已经成功在 Windows 环境下搭建了 Zephyr 的开发环境，Enjoy !


## 推荐资源

* [Zephyr 入门指南](https://www.zephyrproject.org/doc/getting_started/getting_started.html)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>



