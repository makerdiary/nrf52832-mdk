# 低功耗蓝牙示例：Eddystone Beacon

<a href="https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_EddystoneService"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍
<img align="left" src="https://img.makerdiary.co/wiki/nrf52832mdk/eddystone_logo.png" hspace="15" style="float: left">Eddystone 是 Google 发布的一款跨平台的开源蓝牙低功耗（Bluetooth Low Energy，BLE）Beacon 标准，其目标在于试图创建一个鲁棒、可扩展的 Beacon 标准，并试图把商人、开发者和制造商集合在一起，建立起一个完善的 Beacon 生态系统。

Eddystone 支持通用唯一识别码（Universally Unique Identifier，UUID）、URL链接、临时标识（Ephemeral Identifiers，EID）以及遥测数据等四种框架类型：

* [Eddystone-UID](https://github.com/google/eddystone/tree/master/eddystone-uid): 广播 16 字节的 Beacon ID，与 iBeacon 类似；

* [Eddystone-URL](https://github.com/google/eddystone/tree/master/eddystone-url): 广播使用压缩编码格式的 URL，比 UID 更加简单、直接；

* [Eddystone-EID](https://github.com/google/eddystone/tree/master/eddystone-eid)：广播加密过的临时标识，只有经过授权的设备才能读取信息；

* [Eddystone-TLM](https://github.com/google/eddystone/tree/master/eddystone-tlm): 广播遥测数据，用于管理员掌握 Beacon 设备的状态，如电池电量、温度等等。

更多关于 Eddystone 协议规范可以参考：[https://github.com/google/eddystone](https://github.com/google/eddystone).

本例将创建能够广播 Eddystone-UID, Eddystone-URL, Eddystone-TLM 三种数据的 Eddystone Beacon 设备。

## 主要源代码

``` c
#include <events/mbed_events.h>

#include <mbed.h>
#include "ble/BLE.h"
#include "EddystoneService.h"

#include "PersistentStorageHelper/ConfigParamsPersistence.h"

EddystoneService *eddyServicePtr;

/* Duration after power-on that config service is available. */
static const int CONFIG_ADVERTISEMENT_TIMEOUT_SECONDS = 30;

/* Default UID frame data */
static const UIDNamespaceID_t uidNamespaceID = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
static const UIDInstanceID_t  uidInstanceID  = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

/* Default version in TLM frame */
static const uint8_t tlmVersion = 0x00;

/* Values for ADV packets related to firmware levels, calibrated based on measured values at 1m */
static const PowerLevels_t defaultAdvPowerLevels = {-47, -33, -21, -13};
/* Values for radio power levels, provided by manufacturer. */
static const PowerLevels_t radioPowerLevels      = {-30, -16, -4, 4};

static EventQueue eventQueue(
    /* event count */ 16 * /* event size */ 32
);

DigitalOut led(LED1, 1);

/**
 * Callback triggered upon a disconnection event.
 */
static void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *cbParams)
{
    (void) cbParams;
    BLE::Instance().gap().startAdvertising();
}

/**
 * Callback triggered some time after application started to switch to beacon mode.
 */
static void timeout(void)
{
    Gap::GapState_t state;
    state = BLE::Instance().gap().getState();
    if (!state.connected) { /* don't switch if we're in a connected state. */
        eddyServicePtr->startBeaconService();
        EddystoneService::EddystoneParams_t params;
        eddyServicePtr->getEddystoneParams(params);
        saveEddystoneServiceConfigParams(&params);
    } else {
        eventQueue.call_in(CONFIG_ADVERTISEMENT_TIMEOUT_SECONDS * 1000, timeout);
    }
}

static void blinky(void)
{
    led = !led;
}

static void onBleInitError(BLE::InitializationCompleteCallbackContext* initContext)
{
    /* Initialization error handling goes here... */
    (void) initContext;
}

static void initializeEddystoneToDefaults(BLE &ble)
{
    /* Set everything to defaults */
    eddyServicePtr = new EddystoneService(ble, defaultAdvPowerLevels, radioPowerLevels, eventQueue);

    /* Set default URL, UID and TLM frame data if not initialized through the config service */
    const char* url = YOTTA_CFG_EDDYSTONE_DEFAULT_URL;
    eddyServicePtr->setURLData(url);
    eddyServicePtr->setUIDData(uidNamespaceID, uidInstanceID);
    eddyServicePtr->setTLMData(tlmVersion);
}

static void bleInitComplete(BLE::InitializationCompleteCallbackContext* initContext)
{
    BLE         &ble  = initContext->ble;
    ble_error_t error = initContext->error;

    if (error != BLE_ERROR_NONE) {
        onBleInitError(initContext);
        return;
    }

    ble.gap().onDisconnection(disconnectionCallback);

    EddystoneService::EddystoneParams_t params;
    if (loadEddystoneServiceConfigParams(&params)) {
        eddyServicePtr = new EddystoneService(ble, params, radioPowerLevels, eventQueue);
    } else {
        initializeEddystoneToDefaults(ble);
    }

    /* Start Eddystone in config mode */
   eddyServicePtr->startConfigService();

   eventQueue.call_in(CONFIG_ADVERTISEMENT_TIMEOUT_SECONDS * 1000, timeout);
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}


int main()
{
    /* Tell standard C library to not allocate large buffers for these streams */
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    setbuf(stdin, NULL);

    eventQueue.call_every(500, blinky);

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);

    eventQueue.dispatch_forever();
    return 0;
}

```

## 编译

``` sh
$ cd ./BLE_EddystoneService

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_EddystoneService
Elf2Bin: BLE_EddystoneService
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   184 |     3 |   43 |
| Misc                  | 35812 |  2488 |  230 |
| drivers               |   732 |     0 |    0 |
| events                |    52 |     0 |    0 |
| events/equeue         |  1416 |     0 |   61 |
| features/FEATURE_BLE  | 16472 |     5 |  540 |
| hal                   |   506 |     0 |   16 |
| platform              |  1581 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 23679 |   140 | 2200 |
| Subtotals             | 86961 |  2664 | 7636 |
+-----------------------+-------+-------+------+
Allocated Heap: 40948 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10300 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 89625 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_EddystoneService.hex

```

## 下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `BLE_EddystoneService.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_EddystoneService.hex

```


## 验证
打开 **nRF Connect** 应用，将扫描到设备 `EDDYSTONE CONFIG`，将其 ServiceData 设置为 Eddystone 格式，可以看到该设备在轮流广播三种数据：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-eddystone-service.gif)

Enjoy !

## 推荐资源

* [Eddystone 协议规范](https://github.com/google/eddystone)
* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)
* [mbed-os-example-ble/BLE_EddystoneService](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_EddystoneService)
* [Bluetooth Specifications](https://www.bluetooth.com/specifications)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

