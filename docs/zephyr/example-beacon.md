# Zephyr 入门示例：低功耗蓝牙 Beacon

<a href="https://github.com/makerdiary/zephyr/tree/nrf52832-mdk/samples/bluetooth/beacon"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 简介
本例是 Zephyr 低功耗蓝牙的一个简单示例，演示 BLE 的广播功能，广播数据包使用 [Eddystone-URL](https://github.com/google/eddystone/tree/master/eddystone-url) 规定的格式，发出 URL `http://www.zephyrproject.org`。

更多关于 Eddystone 协议内容，请查看：[google/eddystone](https://github.com/google/eddystone)。


## 源代码

``` c
#include <stdint.h>
#include <stddef.h>
#include <misc/printk.h>
#include <misc/util.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>

#define DEVICE_NAME CONFIG_BLUETOOTH_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/*
 * Set Advertisement data. Based on the Eddystone specification:
 * https://github.com/google/eddystone/blob/master/protocol-specification.md
 * https://github.com/google/eddystone/tree/master/eddystone-url
 */
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xaa, 0xfe),
	BT_DATA_BYTES(BT_DATA_SVC_DATA16,
		      0xaa, 0xfe, /* Eddystone UUID */
		      0x10, /* Eddystone-URL frame type */
		      0x00, /* Calibrated Tx power at 0m */
		      0x00, /* URL Scheme Prefix http://www. */
		      'z', 'e', 'p', 'h', 'y', 'r',
		      'p', 'r', 'o', 'j', 'e', 'c', 't',
		      0x08) /* .org */
};

/* Set Scan Response data */
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	/* Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Beacon started\n");
}

void main(void)
{
	int err;

	printk("Starting Beacon Demo\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}
}

```

## 编译

``` sh
$ cd ./zephyr-project/
$ source zephyr-env.sh

$ cd ./samples/bluetooth/beacon/
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

打开串口，波特率为 `115200bps`，将输出蓝牙运行的调试信息：

``` sh
$ screen /dev/cu.usbmodem1422 115200
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_beacon.png)

打开 **nRF Connect** 应用，查看广播数据包的内容：

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_beacon_mobile.jpg)

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_beacon_app.png)

也可以查看 Physical Web 是否发现该设备：

!!! note "提示"
    关于 Physical Web 的使用可以参考：[Setting up Physical Web in Chrome](https://google.github.io/physical-web/try-physical-web)。

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_beacon_physicalweb.png)

Enjoy !

## 推荐资源

* [Zephyr 官方文档](https://www.zephyrproject.org/doc/index.html)
* [Eddystone 协议规范](https://github.com/google/eddystone)
* [Physical Web 官网](https://google.github.io/physical-web/)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

