# 快速入门

本小节主要介绍如何快速搭建 mbed OS 5 的开发环境，并成功运行一个简单的示例。

!!! tip ""
    mbed OS 5 支持在线 IDE，mbed CLI 以及第三方 IDE 进行开发，这里我们选择 mbed CLI，这也是 mbed 主推的一种开发方式。

## 安装依赖工具

### Python
mbed CLI 是用 Python 开发的工具，因此需要确保你的系统已经安装了 [Python](https://www.python.org/downloads/release/python-2711/)，版本建议在 2.7.11或以上，不支持 Python 3。

``` sh
$ python --version
Python 2.7.13
```

### Git & Mercurial
mbed CLI 使用 Git 和 Mercurial 作为版本控制工具，因此确保已经安装了这两个工具：

* [Git](https://git-scm.com/) - 1.9.5 或以上版本
* [Mercurial](https://www.mercurial-scm.org/) - 2.2.2 或以上版本

``` sh
$ git --version
git version 2.10.1

# Mercurial 使用的命令
$ hg --version
分布式软件配置管理工具 - 水银 (版本 3.9)
(see https://mercurial-scm.org for more information)

Copyright (C) 2005-2016 Matt Mackall and others
This is free software; see the source for copying conditions. There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## 安装 mbed CLI
完成以上步骤，便可以安装 mbed CLI，可以使用 `pip` 安装最新稳定版本：

``` sh
$ pip install mbed-cli
```

或者，克隆源码安装正在开发中的版本：

``` sh
$ git clone https://github.com/ARMmbed/mbed-cli
$ cd mbed-cli
$ python setup.py install
```

!!! warning "注意"
    在 Linux 和 Mac OSX 系统，可能需要使用 `sudo` 来安装。

``` sh
$ mbed --version
1.0.0
```

## 配置编译器
mbed OS 可以使用 GCC ARM, ARM Compiler 5, IAR 编译工具。这里使用 [GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm)，该工具链免费开源，现已由 ARM 在维护，[下载](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm)该工具链并配置 `GCC_ARM_PATH` 变量：

``` sh
$ mbed config --global GCC_ARM_PATH "your_toolchain_path/gcc-arm-none-eabi-6-2017-q1-update/bin"

# 可通过以下命令查看 mbed CLI 配置：
$ mbed config --list

```

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-cli-config.png)



## 导入示例

这里导入一个简单的 `blinky` 示例

``` sh
$ mbed import https://github.com/makerdiary/mbed-os-example-blinky

# 国内访问 GitHub 有点慢，需要等待...

$ cd ./mbed-os-example-blinky/mbed-os

# 添加 makerdiary/mbed-os 远程仓库
$ git remote add md git@github.com:makerdiary/mbed-os.git

# 新建 nrf52832-mdk 分支
$ git checkout -b nrf52832-mdk

# 获取 md 远程仓库 nrf52832-mdk 分支的最新代码
$ git pull md nrf52832-mdk

```

## 示例代码
这里我们让 LED1(即板上绿色 LED) 闪烁，间隔 500ms：

``` c
#include "mbed.h"

DigitalOut led1(LED1);

// main() runs in its own thread in the OS
int main() {
    while (true) {
        led1 = !led1;
        wait(0.5);
    }
}

```

## 编译

使用 `mbed compile` 命令进行编译：

* 指定开发板：-m <`board_name`>
* 指定编译器：-t <`GCC_ARM`, `ARM` or `IAR`>

``` sh
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若成功编译，将输出以下信息：
...
Compile [100.0%]: test_env.cpp
Link: mbed-os-example-blinky
Elf2Bin: mbed-os-example-blinky
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   119 |     4 |   24 |
| Misc                  | 26387 |  2484 |  118 |
| hal                   |   344 |     0 |   16 |
| platform              |  1187 |     4 |  264 |
| rtos                  |    38 |     4 |    4 |
| rtos/rtx              |  5841 |    20 | 4278 |
| targets/TARGET_NORDIC |  7751 |    20 |  804 |
| Subtotals             | 41667 |  2536 | 5508 |
+-----------------------+-------+-------+------+
Allocated Heap: 43204 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 8044 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 44203 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/mbed-os-example-blinky.hex

```

## 下载固件
nRF52832-MDK 连接到 PC，直接将编译生成的固件 `./BUILD/NRF52832_MDK/GCC_ARM/mbed-os-example-blinky.hex` 拷贝到 **DAPLINK** 的移动盘，完成拷贝，按下复位键即可运行：

![](https://img.makerdiary.co/wiki/nrf52832mdk/flashing-mbed-os-blinky.png)

或者直接运行以下命令自动下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/mbed-os-example-blinky.hex
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-os-blinky.gif)


至此，你已经成功在 nRF52832-MDK 上跑 mbed OS 5。Enjoy !

## 推荐资源

* 如果你不熟悉 Git，这里有很好的学习资料：
    * [Pro Git 中文版](https://git-scm.com/book/zh/v2)
    * [Learn Git Branching](http://learngitbranching.js.org/)
    * [tryGit](https://try.github.io)

* [更多关于 mbed CLI 的资源](https://docs.mbed.com/docs/mbed-os-handbook/en/latest/dev_tools/cli/)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

