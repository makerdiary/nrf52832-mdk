# 低功耗蓝牙示例：LED Blinker

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/mbedos5/mbed-os-example-ble/BLE_LEDBlinker"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍
本例将 nRF52832-MDK 当做 BLE 主机，用来控制 [BLE LED](/mbedos5/example-ble-led/)，因此你需要另外一块 nRF52832-MDK 来做为从机设备。

## 源代码

``` c
#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/DiscoveredCharacteristic.h"
#include "ble/DiscoveredService.h"

DigitalOut alivenessLED(LED1, 1);
static DiscoveredCharacteristic ledCharacteristic;
static bool triggerLedCharacteristic;
static const char PEER_NAME[] = "LED";

static EventQueue eventQueue(
    /* event count */ 16 * /* event size */ 32    
);

void periodicCallback(void) {
    alivenessLED = !alivenessLED; /* Do blinky on LED1 while we're waiting for BLE events */
}

void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params) {
    // parse the advertising payload, looking for data type COMPLETE_LOCAL_NAME
    // The advertising payload is a collection of key/value records where
    // byte 0: length of the record excluding this byte
    // byte 1: The key, it is the type of the data
    // byte [2..N] The value. N is equal to byte0 - 1
    for (uint8_t i = 0; i < params->advertisingDataLen; ++i) {

        const uint8_t record_length = params->advertisingData[i];
        if (record_length == 0) {
            continue;
        }
        const uint8_t type = params->advertisingData[i + 1];
        const uint8_t* value = params->advertisingData + i + 2;
        const uint8_t value_length = record_length - 1;

        if(type == GapAdvertisingData::COMPLETE_LOCAL_NAME) {
            if ((value_length == sizeof(PEER_NAME)) && (memcmp(value, PEER_NAME, value_length) == 0)) {
                printf(
                    "adv peerAddr[%02x %02x %02x %02x %02x %02x] rssi %d, isScanResponse %u, AdvertisementType %u\r\n",
                    params->peerAddr[5], params->peerAddr[4], params->peerAddr[3], params->peerAddr[2],
                    params->peerAddr[1], params->peerAddr[0], params->rssi, params->isScanResponse, params->type
                );
                BLE::Instance().gap().connect(params->peerAddr, Gap::ADDR_TYPE_RANDOM_STATIC, NULL, NULL);
                break;
            }
        }
        i += record_length;
    }
}

void serviceDiscoveryCallback(const DiscoveredService *service) {
    if (service->getUUID().shortOrLong() == UUID::UUID_TYPE_SHORT) {
        printf("S UUID-%x attrs[%u %u]\r\n", service->getUUID().getShortUUID(), service->getStartHandle(), service->getEndHandle());
    } else {
        printf("S UUID-");
        const uint8_t *longUUIDBytes = service->getUUID().getBaseUUID();
        for (unsigned i = 0; i < UUID::LENGTH_OF_LONG_UUID; i++) {
            printf("%02x", longUUIDBytes[i]);
        }
        printf(" attrs[%u %u]\r\n", service->getStartHandle(), service->getEndHandle());
    }
}

void updateLedCharacteristic(void) {
    if (!BLE::Instance().gattClient().isServiceDiscoveryActive()) {
        ledCharacteristic.read();
    }
}

void characteristicDiscoveryCallback(const DiscoveredCharacteristic *characteristicP) {
    printf("  C UUID-%x valueAttr[%u] props[%x]\r\n", characteristicP->getUUID().getShortUUID(), characteristicP->getValueHandle(), (uint8_t)characteristicP->getProperties().broadcast());
    if (characteristicP->getUUID().getShortUUID() == 0xa001) { /* !ALERT! Alter this filter to suit your device. */
        ledCharacteristic        = *characteristicP;
        triggerLedCharacteristic = true;
    }
}

void discoveryTerminationCallback(Gap::Handle_t connectionHandle) {
    printf("terminated SD for handle %u\r\n", connectionHandle);
    if (triggerLedCharacteristic) {
        triggerLedCharacteristic = false;
        eventQueue.call(updateLedCharacteristic);
    }
}

void connectionCallback(const Gap::ConnectionCallbackParams_t *params) {
    if (params->role == Gap::CENTRAL) {
        BLE &ble = BLE::Instance();
        ble.gattClient().onServiceDiscoveryTermination(discoveryTerminationCallback);
        ble.gattClient().launchServiceDiscovery(params->handle, serviceDiscoveryCallback, characteristicDiscoveryCallback, 0xa000, 0xa001);
    }
}

void triggerToggledWrite(const GattReadCallbackParams *response) {
    if (response->handle == ledCharacteristic.getValueHandle()) {
        printf("triggerToggledWrite: handle %u, offset %u, len %u\r\n", response->handle, response->offset, response->len);
        for (unsigned index = 0; index < response->len; index++) {
            printf("%c[%02x]", response->data[index], response->data[index]);
        }
        printf("\r\n");

        uint8_t toggledValue = response->data[0] ^ 0x1;
        ledCharacteristic.write(1, &toggledValue);
    }
}

void triggerRead(const GattWriteCallbackParams *response) {
    if (response->handle == ledCharacteristic.getValueHandle()) {
        ledCharacteristic.read();
    }
}

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *) {
    printf("disconnected\r\n");
    /* Start scanning and try to connect again */
    BLE::Instance().gap().startScan(advertisementCallback);
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
        /* In case of error, forward the error handling to onBleInitError */
        onBleInitError(ble, error);
        return;
    }

    /* Ensure that it is the default instance of BLE */
    if (ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    ble.gap().onDisconnection(disconnectionCallback);
    ble.gap().onConnection(connectionCallback);

    ble.gattClient().onDataRead(triggerToggledWrite);
    ble.gattClient().onDataWrite(triggerRead);

    // scan interval: 400ms and scan window: 400ms.
    // Every 400ms the device will scan for 400ms
    // This means that the device will scan continuously.
    ble.gap().setScanParams(400, 400);
    ble.gap().startScan(advertisementCallback);
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
    triggerLedCharacteristic = false;
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
$ cd ./BLE_LEDBlinker

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_LEDBlinker
Elf2Bin: BLE_LEDBlinker
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   154 |     3 |   42 |
| Misc                  | 34551 |  2488 |  263 |
| drivers               |   732 |     0 |    0 |
| events                |    48 |     0 |    0 |
| events/equeue         |  1250 |     0 |   61 |
| features/FEATURE_BLE  | 16574 |     5 |  540 |
| hal                   |   506 |     0 |   16 |
| platform              |  1526 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 23679 |   140 | 2200 |
| Subtotals             | 85547 |  2664 | 7668 |
+-----------------------+-------+-------+------+
Allocated Heap: 40916 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10332 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 88211 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_LEDBlinker.hex

```


## 下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `BLE_LEDBlinker.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_LEDBlinker.hex

```

!!! note "注意"
    另外一块 nRF52832-MDK 当做从机，运行 [BLE LED](/mbedos5/example-ble-led/) 固件。

正常运行后，可发现从机的 红色 LED 由主机控制闪烁，打开主机串口，可查看输出信息：

``` sh
$ screen /dev/cu.usbmodem1412 9600
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-blinker.png) 

Enjoy !

## 推荐资源

* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)

* [mbed-os-example-ble/BLE_LEDBlinker](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_LEDBlinker)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>
