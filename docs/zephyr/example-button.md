# Zephyr 入门示例：按键事件

<a href="https://github.com/makerdiary/zephyr/tree/nrf52832-mdk/samples/basic/button"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 简介

本例是 Zephyr 的一个基础示例，演示简单的按键事件驱动，当有按键按下时，通过回调函数打印调试信息。

## 源代码

``` c
#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>
#include <misc/util.h>
#include <misc/printk.h>

/* change this to use another GPIO port */
#ifdef SW0_GPIO_NAME
#define PORT	SW0_GPIO_NAME
#else
#error SW0_GPIO_NAME needs to be set in board.h
#endif

/* change this to use another GPIO pin */
#ifdef SW0_GPIO_PIN
#define PIN     SW0_GPIO_PIN
#else
#error SW0_GPIO_PIN needs to be set in board.h
#endif

/* change to use another GPIO pin interrupt config */
#ifdef SW0_GPIO_INT_CONF
#define EDGE    SW0_GPIO_INT_CONF
#else
/*
 * If SW0_GPIO_INT_CONF not defined used default EDGE value.
 * Change this to use a different interrupt trigger
 */
#define EDGE    (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)
#endif

/* change this to enable pull-up/pull-down */
#define PULL_UP GPIO_PUD_PULL_UP

/* Sleep time */
#define SLEEP_TIME	500


void button_pressed(struct device *gpiob, struct gpio_callback *cb,
		    uint32_t pins)
{
	printk("Button pressed at %d\n", k_cycle_get_32());
}

static struct gpio_callback gpio_cb;

void main(void)
{
	struct device *gpiob;

	printk("Press the user defined button on the board\n");
	gpiob = device_get_binding(PORT);
	if (!gpiob) {
		printk("error\n");
		return;
	}

	gpio_pin_configure(gpiob, PIN,
			   GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE);

	gpio_init_callback(&gpio_cb, button_pressed, BIT(PIN));

	gpio_add_callback(gpiob, &gpio_cb);
	gpio_pin_enable_callback(gpiob, PIN);

	while (1) {
		uint32_t val = 0;

		gpio_pin_read(gpiob, PIN, &val);
		k_sleep(SLEEP_TIME);
	}
}
```

## 编译
``` sh
$ cd ./zephyr-project/
$ source zephyr-env.sh

$ cd ./samples/basic/button/
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

打开串口，波特率为 `115200bps`，查看按键事件输出：

``` sh
$ screen /dev/cu.usbmodem1422 115200
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_button_print.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr-button.jpg)

Enjoy !

## 推荐资源

* [Zephyr 官方文档](https://www.zephyrproject.org/doc/index.html)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>
