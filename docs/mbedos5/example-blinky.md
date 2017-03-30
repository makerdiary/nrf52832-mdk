# 从头开始创建 Blinky

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/mbedos5/mbed-os-blinky"><button data-md-color-primary="indigo">查看本例源码</button></a>

[快速入门](/mbedos5/getting-started/)中介绍了如何导入已经存在的应用，然而更多情况是：我们需要从头开始创建应用。本小节主要介绍如何从头创建 Blinky。

## 克隆 mbed-os
为了避免每次新建应用都要运行 `mbed new <project_name>`，然后再以让人无法忍受的网速去远程仓库拉取最新代码，我们可以先将 `mbed-os` 仓库克隆到本地，每次新建应用时运行 `mbed add <mbed-os-path>` 即可：

``` sh
# 该仓库是在官方代码库的基础上增加 nrf52832-mdk 分支
$ git clone https://github.com/makerdiary/mbed-os.git

# 有需要时再从远程仓库拉取最新代码：
$ cd ./mbed-os
$ git pull origin nrf52832-mdk

```

## 新建应用

在你的项目目录下新建应用，这里以 `./nrf52832-mdk/examples/mbedos5/mbed-os-blinky` 为例：

``` sh
$ cd ./nrf52832-mdk/examples/mbedos5/

# 新建 mbed-os-blinky 目录
$ mkdir mbed-os-blinky

# 添加 mbed-os 库，路径为前面克隆下来的 mbed-os 的目录
$ mbed add <your-mbed-os-path>

# 完成后，mbed-os 会被自动添加进来
$ tree -L 1
.
├── mbed-os/
├── mbed-os.lib
└── mbed_settings.py

```

## 应用程序

这里在应用目录下新建 `main.cpp`，代码如下：

``` c
// main.cpp

#include "mbed.h"

DigitalOut led2(LED2);

// main() runs in its own thread in the OS
int main() {
    while (true) {
        led2 = !led2;
        wait(0.5);
    }
}

```

## 编译

使用 `mbed compile` 命令进行编译：

``` sh
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 编译成功，将输出以下信息：

...

Compile [100.0%]: test_env.cpp
Link: mbed-os-blinky
Elf2Bin: mbed-os-blinky
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

Image: ./BUILD/NRF52832_MDK/GCC_ARM/mbed-os-blinky.hex

```

## 下载运行

将 `./BUILD/NRF52832_MDK/GCC_ARM/mbed-os-blinky.hex` 直接拷贝到 **DAPLINK** 可移动磁盘，按下复位键运行。

或者运行以下命令下载：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/mbed-os-blinky.hex

```

至此，你已成功新建 mbed os 应用，开始下一个应用吧！

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

