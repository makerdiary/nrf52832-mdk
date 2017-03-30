# Zephyr 入门示例：低功耗蓝牙心率计

<a href="https://github.com/makerdiary/zephyr/tree/nrf52832-mdk/samples/bluetooth/peripheral_hr"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 简介
本例将使用 Zephyr 实现一个低功耗蓝牙心率计应用，该应用会包含三个服务：[Heart Rate](https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.heart_rate.xml), [Battery Serice](https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.battery_service.xml), [Device Information](https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.device_information.xml), 你可以通过手机 App 或 Web Bluetooth 与心率计连接并获取数据。


## 源代码

``` c

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <misc/printk.h>
#include <misc/byteorder.h>
#include <zephyr.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

#include <gatt/gap.h>
#include <gatt/hrs.h>
#include <gatt/dis.h>
#include <gatt/bas.h>

#define DEVICE_NAME		CONFIG_BLUETOOTH_DEVICE_NAME
#define DEVICE_NAME_LEN		(sizeof(DEVICE_NAME) - 1)
#define HEART_RATE_APPEARANCE	0x0341

struct bt_conn *default_conn;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0x0d, 0x18, 0x0f, 0x18, 0x05, 0x18),
};

static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err %u)\n", err);
	} else {
		default_conn = bt_conn_ref(conn);
		printk("Connected\n");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);

	if (default_conn) {
		bt_conn_unref(default_conn);
		default_conn = NULL;
	}
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};

static void bt_ready(int err)
{
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	gap_init(DEVICE_NAME, HEART_RATE_APPEARANCE);
	hrs_init(0x01);
	bas_init();
	dis_init(CONFIG_SOC, "Manufacturer");

	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");
}

static void auth_cancel(struct bt_conn *conn)
{
	char addr[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

	printk("Pairing cancelled: %s\n", addr);
}

static struct bt_conn_auth_cb auth_cb_display = {
	.cancel = auth_cancel,
};

void main(void)
{
	int err;

	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	bt_conn_cb_register(&conn_callbacks);
	bt_conn_auth_cb_register(&auth_cb_display);

	/* Implement notification. At the moment there is no suitable way
	 * of starting delayed work so we do it here
	 */
	while (1) {
		k_sleep(MSEC_PER_SEC);

		/* Heartrate measurements simulation */
		hrs_notify();

		/* Battery level simulation */
		bas_notify();
	}
}

```

## 编译

``` sh

$ cd ./zephyr-project/
$ source zephyr-env.sh

$ cd ./samples/bluetooth/peripheral_hr/
$ make BOARD=nrf52832_mdk

# 编译成功，将输出以下信息：
  ...
  CC      src/main.o
  CC      src/../../gatt/gap.o
  CC      src/../../gatt/hrs.o
  CC      src/../../gatt/dis.o
  CC      src/../../gatt/bas.o
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

打开串口，波特率为 `115200bps`，查看调试信息：

``` sh
$ screen /dev/cu.usbmodem1422 115200
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_ble_heartrate.png)

打开 **nRF Connect** 应用，会出现 `Zephyr Heartrate Sensor` 设备，连接后即可获得心率数据：

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_ble_heartrate_app.png)

## Web Bluetooth API
Web Bluetooth 是 [Web Bluetooth Community Group](https://www.w3.org/community/web-bluetooth/) 提出的一种规范，旨在为浏览器提供蓝牙连接功能，目前已经得到了 Google Chrome, Mozilla 等浏览器的支持，未来该标准可能会推广开来，成为标准。

如果你的浏览器支持 Web Bluetooth，不妨体验一下：

<a href="http://wiki.makerdiary.co/WebBluetoothDemos/heart-rate-sensor/"><button data-md-color-primary="indigo">体验 Web Bluetooth</button></a>

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_web_bluetooth_hr.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/zephyr_web_bluetooth_hr1.png)

Enjoy !

## 推荐资源

* [GATT Services](https://www.bluetooth.com/specifications/gatt/services)
* [Zephyr 官方文档](https://www.zephyrproject.org/doc/index.html)
* [Web Bluetooth API](https://webbluetoothcg.github.io/web-bluetooth/)


## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>


