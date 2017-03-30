# Zephyr 入门示例：Blinky

<a href="https://github.com/makerdiary/zephyr/tree/nrf52832-mdk/samples/basic/blinky"><button data-md-color-primary="indigo">查看本例源码</button></a>

这里假设你已经成功搭建好 Zephyr 开发环境，如果没有，请参考前面的文档：

* [Mac OSX](/zephyr/setup-devenv-osx/)
* [Linux](/zephyr/setup-devenv-linux/)
* [Windows](/zephyr/setup-devenv-win/)

## 简介

本例是 Zephyr 一个非常简单的示例，演示如何使用 Zephyr 提供的 API 操作 GPIO，控制 LED 闪烁。

## 代码结构

``` sh
# ./zephyr-project/samples/basic/blinky/

$ tree -L 2
.
├── Makefile         # Makefile 项目编译控制
├── README.rst       # 示例说明文档
├── outdir           # 编译输出目录
│   └── nrf52832_mdk # nRF52832-MDK 固件
├── prj.conf         # 项目配置，定义需要那些模块
├── src             
│   ├── Makefile     # Makefile 源码编译控制
│   └── main.c       # 主程序
└── testcase.ini     # 测试案例

```

## 源代码
这里让 LED0 即板上绿色 LED，每隔 1s 改变一次状态。

``` c
#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>

/* Change this if you have an LED connected to a custom port */
#define PORT	LED0_GPIO_PORT

/* Change this if you have an LED connected to a custom pin */
#define LED	LED0_GPIO_PIN

/* 1000 msec = 1 sec */
#define SLEEP_TIME 	1000

void main(void)
{
	int cnt = 0;
	struct device *dev;

	dev = device_get_binding(PORT);
	/* Set LED pin as output */
	gpio_pin_configure(dev, LED, GPIO_DIR_OUT);

	while (1) {
		/* Set pin to HIGH/LOW every 1 second */
		gpio_pin_write(dev, LED, cnt % 2);
		cnt++;
		k_sleep(SLEEP_TIME);
	}
}
```

## 编译

``` sh

$ cd ./zephyr-project/
$ source zephyr-env.sh

$ cd ./samples/basic/blinky/
$ make BOARD=nrf52832_mdk

# 编译成功，将输出以下信息：
  ...
  CC      src/main.o
  LD      src/built-in.o
  AR      libzephyr.a
  LINK    zephyr.lnk
  IRQ     isr_tables.c
  CC      isr_tables.o
  LINK    zephyr.elf
  HEX     zephyr.hex
  BIN     zephyr.bin

```

## 下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `./outdir/nrf52832_mdk/zephyr.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./outdir/nrf52832_mdk/zephyr.hex

```

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_blinky.gif)

Enjoy !

## 推荐资源

* [Zephyr 官方文档](https://www.zephyrproject.org/doc/index.html)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>




