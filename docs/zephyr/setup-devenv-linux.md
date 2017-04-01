# 搭建 Zephyr 开发环境：Linux

本节主要介绍如何在 Linux 系统使用 Zephyr SDK 快速搭建 Zephyr 开发环境，并成功编译 `Hello World` 示例。

!!! note "提醒"
    这里以 Ubuntu 16.04 LTS 64-bit 系统为例，这也是官方推荐的环境。


## 1、安装依赖工具
编译 Zephyr 需要一些第三方工具支持，在 Ubuntu 系统，使用以下命令完成安装：

``` sh
# 安装前，更新系统
$ sudo apt-get update
$ sudo apt-get upgrade

# 安装依赖工具
$ sudo apt-get install git make gcc g++ python3-ply ncurses-dev python-yaml python2 dfu-util

```

## 2、安装 Zephyr SDK
Zephyr 为 Linux 系统提供了 SDK，该 SDK 包含了所有必要的工具和交叉编译器，可以支持以下架构：

* X86
* X86 IAMCU ABI
* ARM
* ARC
* NIOS II

官网[下载](https://www.zephyrproject.org/downloads/tools)有点慢，我将其放到了百度网盘：[
Zephyr SDK v0.9](https://pan.baidu.com/s/1gfaGiXl)。

下载完后，进行安装：

``` sh
$ chmod +x zephyr-sdk-<version>-setup.run

$ sudo ./zephyr-sdk-<version>-setup.run

# 以下为成功安装的输出：
Verifying archive integrity... All good.
Uncompressing SDK for Zephyr  100%  
Enter target directory for SDK (default: /opt/zephyr-sdk/): 
Installing SDK to /opt/zephyr-sdk
Creating directory /opt/zephyr-sdk
Success
 [*] Installing x86 tools... 
 [*] Installing arm tools... 
 [*] Installing arc tools... 
 [*] Installing iamcu tools... 
 [*] Installing nios2 tools... 
 [*] Installing xtensa tools... 
 [*] Installing riscv32 tools... 
 [*] Installing additional host tools... 
Success installing SDK. SDK is ready to be used.
```

为 Zephyr SDK 配置环境变量：

``` sh
$ export ZEPHYR_GCC_VARIANT=zephyr

$ export ZEPHYR_SDK_INSTALL_DIR=<这里默认的路径是：/opt/zephyr-sdk>
```

也可以将以上命令添加到 `~/.bash_profile` 文件中，这样就不用每次都配置环境变量：

``` sh
# 在~/.bash_profile文件最后添加以下命令：
export ZEPHYR_GCC_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=<这里默认的路径是：/opt/zephyr-sdk>

# 使其生效
$ source .bash_profile

# 使用 echo 命令验证是否设置正确：
$ echo $ZEPHYR_GCC_VARIANT
$ echo $ZEPHYR_SDK_INSTALL_DIR
```


## 3、克隆 Zephyr 源码

Zephyr 源码托管在 [https://gerrit.zephyrproject.org/r/zephyr](https://gerrit.zephyrproject.org/r/zephyr)，这里为了使用方便，我在 GitHub 做了镜像，并增加 `nrf52832-mdk` 分支来支持 nRF52832-MDK 的开发。

``` sh
# 克隆 Zephyr 源码 到 zephyr-project
$ git clone https://github.com/makerdiary/zephyr.git zephyr-project
$ cd zephyr-project/

# 拉取 nrf52832-mdk 分支的最新代码
$ git pull origin nrf52832-mdk
```

## 4、编译

一切顺利的话，现在就可以编译了，以编译 `./zephyr-project/samples/hello_world` 为例：

``` sh
$ cd ./zephyr-project/
$ source zephyr-env.sh

$ cd samples/hello_world
$ make BOARD=nrf52832_mdk

```

编译成功，输出固件：

![](https://img.makerdiary.co/wiki/nrf52832mdk/compile-zephyr-helloworld-linux.png)


## 5、下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `/outdir/nrf52832_mdk/zephyr.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

!!! warning "注意"
    `pyocd-flashtool` 和 `screen` 需要权限，记得增加 `sudo` !

``` sh
$ sudo pyocd-flashtool -d debug -t nrf52 -ce ./outdir/nrf52832_mdk/zephyr.hex

```

打开串口，波特率为 `115200`：

``` sh
$ sudo screen /dev/ttyACM0 115200
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr-print-helloworld-linux.png)


至此，Zephyr 开发环境已经搭建完成，Enjoy !

## 推荐资源

* [Zephyr 入门指南](https://www.zephyrproject.org/doc/getting_started/getting_started.html)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>


