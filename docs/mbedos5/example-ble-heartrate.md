# 低功耗蓝牙示例：心率检测

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/mbedos5/mbed-os-example-ble/BLE_HeartRate"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍
本例会创建一个心率检测的首要服务，根据 [Bluetooth SIG](https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.heart_rate.xml) 对心率服务的定义，该服务会包含 3 个 Characteristic：Heart Rate Measurement, Body Sensor Location 和 Heart Rate Control Point, 分别用于获取心率测量值、传感器位置以及控制其行为。

连接成功后每隔 500ms 更新一次心率数值。作为示例，我们用软件模拟心率传感器，让其数值从 100 逐渐递增，超过 175 再回到 100 重新计数。

最后，我们可以通过一个使用 Web Bluetooth API 的页面来读取心率。



## 源代码

``` c
#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/services/HeartRateService.h"

DigitalOut led1(LED1, 1);

const static char     DEVICE_NAME[] = "HRM";
static const uint16_t uuid16_list[] = {GattService::UUID_HEART_RATE_SERVICE};

static uint8_t hrmCounter = 100; // init HRM to 100bps
static HeartRateService *hrServicePtr;

static EventQueue eventQueue(
    /* event count */ 16 * /* event size */ 32
);

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    BLE::Instance().gap().startAdvertising(); // restart advertising
}

void updateSensorValue() {
    // Do blocking calls or whatever is necessary for sensor polling.
    // In our case, we simply update the HRM measurement.
    hrmCounter++;

    //  100 <= HRM bps <=175
    if (hrmCounter == 175) {
        hrmCounter = 100;
    }

    hrServicePtr->updateHeartRate(hrmCounter);
}

void periodicCallback(void)
{
    led1 = !led1; /* Do blinky on LED1 while we're waiting for BLE events */

    if (BLE::Instance().getGapState().connected) {
        eventQueue.call(updateSensorValue);
    }
}

void onBleInitError(BLE &ble, ble_error_t error)
{
    (void)ble;
    (void)error;
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

    ble.gap().onDisconnection(disconnectionCallback);

    /* Setup primary service. */
    hrServicePtr = new HeartRateService(ble, hrmCounter, HeartRateService::LOCATION_FINGER);

    /* Setup advertising. */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::GENERIC_HEART_RATE_SENSOR);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1000); /* 1000ms */
    ble.gap().startAdvertising();
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
$ cd ./BLE_HeartRate

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_HeartRate
Elf2Bin: BLE_HeartRate
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   165 |    10 |   43 |
| Misc                  | 28996 |  2489 |  230 |
| drivers               |   732 |     0 |    0 |
| events                |    48 |     0 |    0 |
| events/equeue         |  1250 |     0 |   61 |
| features/FEATURE_BLE  | 16412 |     5 |  540 |
| hal                   |   506 |     0 |   16 |
| platform              |  1526 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 23679 |   140 | 2200 |
| Subtotals             | 79841 |  2672 | 7636 |
+-----------------------+-------+-------+------+
Allocated Heap: 40940 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10308 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 82513 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_HeartRate.hex
```

## 下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `BLE_HeartRate.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_HeartRate.hex

```

使用 **nRF Connect** 应用，我们可以查看该设备的相关信息：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-heartrate.gif)


## Web Bluetooth
如果你的 PC 支持 BLE，可以通过 Web Bluetooth 获取心率：

<a href="http://wiki.makerdiary.co/WebBluetoothDemos/heart-rate-sensor/"><button data-md-color-primary="indigo">获取心率</button></a>

![](https://img.makerdiary.co/wiki/nrf52832mdk/web-bluetooth-hrm.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/web-bluetooth-heartrate.png)


Enjoy !


# 推荐资源

* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)

* [BLE Heart Rate Service](https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.heart_rate.xml)

* [mbed-os-example-ble/BLE_HeartRate](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_HeartRate)

* [Web Bluetooth API](https://webbluetoothcg.github.io/web-bluetooth/)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

