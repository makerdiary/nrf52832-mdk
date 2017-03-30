# 搭建 Zephyr 开发环境：Mac OSX

本节主要介绍如何在 Mac OSX 系统使用 [GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm) 快速搭建 Zephyr 开发环境，并成功编译 `Hello World` 示例。

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

## 2、安装依赖工具

编译 Zephyr 需要一些第三方工具支持，在 Mac OSX 系统，我们可以使用 [Homebrew](http://brew.sh/) 来安装这些工具：

``` sh
$ brew install gettext qemu help2man mpfr gmp coreutils wget python3

$ brew tap homebrew/dupes

$ brew install grep --with-default-names

$ pip3 install ply

```

!!! note "提示"
    可以参考 Zephyr [官方文档](https://www.zephyrproject.org/doc/getting_started/installation_mac.html)，这里我们不需要制作交叉工具链，所以无需安装 `crosstool-ng`。感兴趣的伙计可以自行尝试！

## 3、克隆 Zephyr 源码

Zephyr 源码托管在 [https://gerrit.zephyrproject.org/r/zephyr](https://gerrit.zephyrproject.org/r/zephyr)，这里为了使用方便，我在 GitHub 做了镜像，并增加 `nrf52832-mdk` 分支来支持 nRF52832-MDK 的开发。

``` sh
# 克隆 Zephyr 源码 到 zephyr-project
$ git clone https://github.com/makerdiary/zephyr.git zephyr-project
$ cd zephyr-project/

# 建立 nrf52832-mdk 分支
$ git checkout -b nrf52832-mdk

# 拉取 nrf52832-mdk 分支的最新代码
$ git pull origin nrf52832-mdk
```

## 4、配置环境变量

在进行编译前，需要配置以下环境变量：

``` sh
$ export GCCARMEMB_TOOLCHAIN_PATH="your_toolchain_path/gcc-arm-none-eabi-6-2017-q1-update"

$ export ZEPHYR_GCC_VARIANT=gccarmemb
```

也可以将以上命令添加到 `~/.bash_profile` 文件中，这样就不用每次都配置环境变量：

``` sh
# 在~/.bash_profile文件最后添加以下命令：
export GCCARMEMB_TOOLCHAIN_PATH="your_toolchain_path/gcc-arm-none-eabi-6-2017-q1-update"
export ZEPHYR_GCC_VARIANT=gccarmemb

# 使其生效
$ source .bash_profile

# 使用 echo 命令验证是否设置正确：
$ echo $GCCARMEMB_TOOLCHAIN_PATH
$ echo $ZEPHYR_GCC_VARIANT
```

## 5、编译

一切顺利的话，现在就可以编译了，以编译 `./zephyr-project/samples/hello_world` 为例：

``` sh
$ cd ./zephyr-project/
$ source zephyr-env.sh

$ cd samples/hello_world
$ make BOARD=nrf52832_mdk

```

编译成功，输出固件：

![](https://img.makerdiary.co/wiki/nrf52832mdk/compile_zephyr_helloworld.png)

## 6、下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `/outdir/nrf52832_mdk/zephyr.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./outdir/nrf52832_mdk/zephyr.hex

```

打开串口，波特率为 `115200`：

``` sh
$ screen /dev/cu.usbmodem1412 115200
```
![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_helloworld_output.png)


至此，Zephyr 开发环境已经搭建完成，Enjoy !

## 推荐资源

* [Zephyr 入门指南](https://www.zephyrproject.org/doc/getting_started/getting_started.html)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>


