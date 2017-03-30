# 低功耗蓝牙示例：Beacon

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/mbedos5/mbed-os-example-ble/BLE_Beacon"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍
本例即将创建一个 iBeacon 设备，然后每隔 1s 广播一次数据（包括 16字节 UUID + 2字节 Major + 2字节 Minor + 1字节 Tx-Power）。

作为示例，iBeacon ID 由我的公众号「[造物邦](http://weixin.sogou.com/weixin?type=1&query=zaowubang&ie=utf8)」配置生成，可以通过微信摇一摇进入到本文档页面。iBeacon ID 如下：

| iBeacon ID | Value |
| ---------- | ----- |
| **UUID**   | FDA50693-A4E2-4FB1-AFCF-C6EB07647825 |
| **Major**  | 10100 |
| **Minor**  | 56482 |


## 源代码

``` c
#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/services/iBeacon.h"

static iBeacon* ibeaconPtr;

static EventQueue eventQueue(
    /* event count */ 4 * /* event size */ 32    
);

/**
 * This function is called when the ble initialization process has failled
 */
void onBleInitError(BLE &ble, ble_error_t error)
{
    /* Initialization error handling should go here */
}

/**
 * Callback triggered when the ble initialization process has finished
 */
void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        /* In case of error, forward the error handling to onBleInitError */
        onBleInitError(ble, error);
        return;
    }

    /* Ensure that it is the default instance of BLE */
    if(ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    /**
     * The Beacon payload has the following composition:
     * 128-Bit / 16byte UUID = FDA50693-A4E2-4FB1-AFCF-C6EB07647825
     * Major/Minor  = 10100 / 56482
     * Tx Power     = 0xC8 = 200, 2's compliment is 256-200 = (-56dB)
     *
     * Note: please remember to calibrate your beacons TX Power for more accurate results.
     */
    static const uint8_t uuid[] = {0xFD, 0xA5, 0x06, 0x93, 0xA4, 0xE2, 0x4F, 0xB1,
                                   0xAF, 0xCF, 0xC6, 0xEB, 0x07, 0x64, 0x78, 0x25};
    uint16_t majorNumber = 10100;
    uint16_t minorNumber = 56482;
    uint16_t txPower     = 0xC8;
    ibeaconPtr = new iBeacon(ble, uuid, majorNumber, minorNumber, txPower);

    ble.gap().setAdvertisingInterval(1000); /* 1000ms. */
    ble.gap().startAdvertising();
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);

    eventQueue.dispatch_forever();

    return 0;
}
```


## 编译

``` sh
$ cd ./BLE_Beacon

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_Beacon
Elf2Bin: BLE_Beacon
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   223 |     3 |   40 |
| Misc                  | 27692 |  2488 |  225 |
| drivers               |   732 |     0 |    0 |
| events                |    48 |     0 |    0 |
| events/equeue         |  1238 |     0 |   61 |
| features/FEATURE_BLE  | 16376 |     5 |  540 |
| hal                   |   506 |     0 |   16 |
| platform              |  1526 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 23587 |   140 | 2200 |
| Subtotals             | 78455 |  2664 | 7628 |
+-----------------------+-------+-------+------+
Allocated Heap: 40956 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10292 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 81119 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_Beacon.hex
```

## 下载运行
将 nRF52832-MDK 连接到 PC，拷贝 `BLE_Beacon.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_Beacon.hex

```

## 摇一摇
手机打开蓝牙，进入微信 --> 发现 --> 摇一摇，如果发现 iBeacon 设备，会出现 **周边** 图标，摇一摇就会出现该设备所绑定的页面：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-beacon-weixin-wiki.png)

Enjoy !

## 推荐资源
* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)
* [mbed-os-example-ble/BLE_Beacon](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_Beacon)
* 一家把 Beacon 技术玩到极致的公司：[Estimote](http://developer.estimote.com/)
* [Apple iBeacon for Developers](https://developer.apple.com/ibeacon/)
* [摇一摇周边官网](https://zb.weixin.qq.com/)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>


