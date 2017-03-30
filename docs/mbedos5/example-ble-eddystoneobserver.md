# 低功耗蓝牙示例：Eddystone 监听器

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/mbedos5/mbed-os-example-ble/BLE_EddystoneObserver"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍

Eddystone 是 Google 开源的 Beacon 协议，相比 iBeacon，Eddystone 协议规定更加灵活、开放，可以兼容 Android 和 iOS 设备。

更多关于 Eddystone 协议规范可以参考：[https://github.com/google/eddystone](https://github.com/google/eddystone).

本例将创建一个 Eddystone 监听器，用于监听其他 Eddystone 设备发出的 URL，并通过串口将这些信息打印出来。

!!! note "提示"
    本例需要额外的蓝牙设备来发送 Eddystone-URL，例如额外的 nRF52832-MDK 或者 一台支持 BLE 的笔记本(我们使用第三方工具让其发出 Eddystone-URL)。

## 工作原理
本例实际上使用了 BLE 的被动扫描功能，主要用来接收对端设备的广播数据包，从接收到的广播包中判断是否有 Eddystone-URL 的信息，有则将其打印出来。

以下为被动扫描的基本流程：

![](https://img.makerdiary.co/wiki/nrf52832mdk/passive-scaning.png)

扫描参数中，扫描间隔 (Scan interval) 和扫描窗口 (Scan Window) 是两个非常重要的参数，因为它们决定了控制器的扫描频率和扫描时间的长短：

* 扫描间隔 —— 控制器间隔多长时间扫描一次；
* 扫描窗口 —— 每一次扫描的持续时间。

例如，本例中扫描间隔为 1800 ms，扫描窗口为 1000ms，控制器的扫描占空比为 56%。理论上可捕获到定向广播数据包的最低占空比为 0.4%，扫描间隔越长、占空比越低，越不容易捕获到广播数据包。


## 源代码

``` c
#include <events/mbed_events.h>
#include "mbed.h"
#include "ble/BLE.h"

static const int URI_MAX_LENGTH = 18;             // Maximum size of service data in ADV packets

static EventQueue eventQueue(
    /* event count */ 16 * /* event size */ 32
);

DigitalOut led1(LED1, 1);

void periodicCallback(void)
{
    led1 = !led1; /* Do blinky on LED1 while we're waiting for BLE events */
}

void decodeURI(const uint8_t* uriData, const size_t uriLen)
{
    const char *prefixes[] = {
        "http://www.",
        "https://www.",
        "http://",
        "https://",
        "urn:uuid:"
    };
    const size_t NUM_PREFIXES = sizeof(prefixes) / sizeof(char *);
    const char *suffixes[] = {
        ".com/",
        ".org/",
        ".edu/",
        ".net/",
        ".info/",
        ".biz/",
        ".gov/",
        ".com",
        ".org",
        ".edu",
        ".net",
        ".info",
        ".biz",
        ".gov"
    };
    const size_t NUM_SUFFIXES = sizeof(suffixes) / sizeof(char *);

    size_t index = 0;

    /* First byte is the URL Scheme. */
    if (uriData[index] < NUM_PREFIXES) {
        printf("%s", prefixes[uriData[index]]);
        index++;
    } else {
        printf("URL Scheme was not encoded!");
        return;
    }

    /* From second byte onwards we can have a character or a suffix */
    while(index < uriLen) {
        if (uriData[index] < NUM_SUFFIXES) {
            printf("%s", suffixes[uriData[index]]);
        } else {
            printf("%c", uriData[index]);
        }
        index++;
    }

    printf("\n\r");
}

/*
 * This function is called every time we scan an advertisement.
 */
void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params)
{
    struct AdvertisingData_t {
        uint8_t                        length; /* doesn't include itself */
        GapAdvertisingData::DataType_t dataType;
        uint8_t                        data[1];
    } AdvDataPacket;

    struct ApplicationData_t {
        uint8_t applicationSpecificId[2];
        uint8_t frameType;
        uint8_t advPowerLevels;
        uint8_t uriData[URI_MAX_LENGTH];
    } AppDataPacket;

    const uint8_t BEACON_UUID[sizeof(UUID::ShortUUIDBytes_t)] = {0xAA, 0xFE};
    const uint8_t FRAME_TYPE_URL                              = 0x10;
    const uint8_t APPLICATION_DATA_OFFSET                     = sizeof(ApplicationData_t) + sizeof(AdvDataPacket.dataType) - sizeof(AppDataPacket.uriData);

    AdvertisingData_t *pAdvData;
    size_t index = 0;
    while(index < params->advertisingDataLen) {
        pAdvData = (AdvertisingData_t *)&params->advertisingData[index];
        if (pAdvData->dataType == GapAdvertisingData::SERVICE_DATA) {
            ApplicationData_t *pAppData = (ApplicationData_t *) pAdvData->data;
            if (!memcmp(pAppData->applicationSpecificId, BEACON_UUID, sizeof(BEACON_UUID)) && (pAppData->frameType == FRAME_TYPE_URL)) {
                decodeURI(pAppData->uriData, pAdvData->length - APPLICATION_DATA_OFFSET);
                break;
            }
        }
        index += (pAdvData->length + 1);
    }
}

void onBleInitError(BLE &ble, ble_error_t error)
{
   /* Initialization error handling should go here */
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        onBleInitError(ble, error);
        return;
    }

    if (ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    ble.gap().setScanParams(1800 /* scan interval */, 1500 /* scan window */);
    ble.gap().startScan(advertisementCallback);
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
    eventQueue.call_every(500, periodicCallback);

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);

    eventQueue.dispatch_forever();

    return 0;
}
```

## 编译

``` sh
$ cd ./BLE_EddystoneObserver

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_EddystoneObserver
Elf2Bin: BLE_EddystoneObserver
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   165 |     3 |   47 |
| Misc                  | 33600 |  2488 |  226 |
| drivers               |   732 |     0 |    0 |
| events                |    48 |     0 |    0 |
| events/equeue         |  1250 |     0 |   61 |
| features/FEATURE_BLE  | 16458 |     5 |  540 |
| hal                   |   506 |     0 |   16 |
| platform              |  1526 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 23679 |   140 | 2200 |
| Subtotals             | 84491 |  2664 | 7636 |
+-----------------------+-------+-------+------+
Allocated Heap: 40948 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10300 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 87155 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_EddystoneObserver.hex

```

## 下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `BLE_EddystoneObserver.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_EddystoneObserver.hex

```

## 创建 Eddystone Beacon

如果你手上有多余的 nRF52832-MDK 板子，可以参照 [低功耗蓝牙示例：Eddystone Beacon](/mbedos5/example-ble-eddystoneservice/) 创建一个或多个 Eddystone Beacon 设备，用来广播 Eddystone-URL 信息：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-eddystone-observer.jpg)

或者，你可以找一台支持 BLE 的笔记本，将其作为一个 Eddystone Beacon 来广播 Eddystone-URL 信息：

``` sh
# 安装基于 node.js 的 eddystone-beacon
$ npm install eddystone-beacon
```

源代码位于 `./BLE_EddystoneObserver/eddystone-url.js`：

``` javascript
// eddystone-url.js

var eddystoneBeacon = require('eddystone-beacon');

eddystoneBeacon.advertiseUrl('https://www.google.com');

```

node 运行 eddystone-url.js 开始广播：

``` sh
$ cd ./BLE_EddystoneObserver/

$ node eddystone-url.js
```

## 监听
现在，我们可以开始监听周围有哪些 Eddystone Beacon 在广播数据。串口的使用请参考 [首次使用](/getting-started/hello-world/) 中的介绍，这里以 Mac OSX 系统为例：

``` sh
# 打开串口，mbed os 默认的串口波特率为 9600bps
$ screen /dev/cu.usbmodem1422 9600

```
可以发现有两个 URL 正在被广播出来：`https://google.com` 和 `https://mbed.om`.

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-eddystone-observer.png)

Enjoy !

## 推荐资源

* [Eddystone 协议规范](https://github.com/google/eddystone)
* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)
* [mbed-os-example-ble/BLE_EddystoneObserver](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_EddystoneObserver)
* [Bluetooth Specifications](https://www.bluetooth.com/specifications)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

