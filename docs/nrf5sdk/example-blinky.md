# nRF5 SDK 入门示例：Blinky

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/nrf5-sdk/blinky"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 简介
Blinky 是一个非常简单的示例项目，通过该示例，你将会了解 nRF5 SDK 开发的基本步骤，为后面开发更复杂的应用做好准备。

## 源码结构
以下树状图展示了与 `blinky` 示例相关的源码结构，由 `Makefile` 文件统一控制项目的编译、下载，你可以以此为模板创建自己的项目：

``` sh
.
├── docs
├── examples
│   └── nrf5-sdk
│       └── blinky
│           ├── Makefile              # 编译控制文件
│           ├── _build                # 存放编译输出文件
│           ├── blinky_gcc_nrf52.ld   # 链接命令
│           ├── hex                   
│           │   └── nrf52832_xxaa.hex # 预编译hex文件
│           ├── inc                   
│           │   ├── custom_board.h    # 硬件配置文件
│           │   └── sdk_config.h      # SDK配置文件
│           ├── src                 
│           │   └── main.c            # 主程序
│           └── README.md             # 说明文档
├── nrf_sdks
│   ├── nRF5_SDK_13.0.0_04a0bfd       # 官方SDK存放位置
│   │   ├── components
│   │   ├── config
│   │   ├── documentation
│   │   ├── examples
│   │   ├── external
│   │   ├── license.txt
│   │   ├── nRF5x_MDK_8_12_0_IAR_NordicLicense.msi
│   │   ├── nRF5x_MDK_8_12_0_Keil4_NordicLicense.msi
│   │   └── svd
└── tools
    └── mergehex  # hex文件合成工具
```

## 主程序
主程序位于 `blinky/src/main.c`，这里只简单实现 RGB LED 以流水灯的方式循环闪烁，源码如下：

``` c
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    /* Configure board. */
    bsp_board_leds_init();

    /* Toggle LEDs. */
    while (true)
    {
        for (int i = 0; i < LEDS_NUMBER; i++)
        {
            bsp_board_led_invert(i);
            nrf_delay_ms(500);
        }
    }
}

/**
 *@}
 **/
```

## Makefile

Makefile 会定义程序的编译规则，一旦写好，只需要一个 `make` 命令，整个项目完全自动编译，极大地提高了软件开发的效率。在实际开发过程中，可以使用示例项目的 `Makefile` 文件作为模板，主要做几点修改即可：

``` makefile

# 修改项目的链接文件
LINKER_SCRIPT  := blinky_gcc_nrf52.ld

# C 文件路径
SRC_FILES += your_source_code_path

# 引用头文件的路径
INC_FOLDERS += your_header_file_path

# 静态库路径
LIB_FILES += your_lib_file_path

# 定义宏，如必须定义 'BOARD_CUSTOM' 宏
CFLAGS += -DBOARD_CUSTOM
ASMFLAGS += -DBOARD_CUSTOM

```

## 下载固件

Makefile 文件中定义了使用 pyOCD 下载固件的 shell 命令：

``` makefile
# 下载应用程序
flash: $(OUTPUT_DIRECTORY)/nrf52832_xxaa.hex
	@echo Flashing: $<
	pyocd-flashtool -d debug -t nrf52 -se $<

# 下载 softdevice
flash_softdevice:
	@echo Flashing: s132_nrf52_4.0.2_softdevice.hex
	pyocd-flashtool -d debug -t nrf52 -se $(SDK_ROOT)/components/softdevice/s132/hex/s132_nrf52_4.0.2_softdevice.hex

# 擦除整片 flash
erase:
	pyocd-flashtool -d debug -t nrf52 -ce

```

因此，只需要在终端直接运行相关命令即可完成擦除、下载等操作：

``` sh
# 下载应用程序
$ make flash

# 如果调用 softdevice 的 API，必须下载预编译好的 softdevice，softdevice 为 Nordic 的专利，这部分没有开放源码
$ make flash_softdevice

# 擦除整片 flash
$ make erase

```

好了，可以看看代码的实际运行效果：


![](https://img.makerdiary.co/wiki/nrf52832mdk/blinky-demo-latest.jpg)


## 推荐资源
* 一个你会爱上的文本编辑器：[Sublime Text](https://www.sublimetext.com/)
* [“跟我一起写 Makefile”](http://blog.csdn.net/haoel/article/details/2886)
* [Nordic 官方文档](http://infocenter.nordicsemi.com/index.jsp)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

