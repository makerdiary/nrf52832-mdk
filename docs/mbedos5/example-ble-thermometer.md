# 低功耗蓝牙示例：体温计

<a href="https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_Thermometer"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍

本例将创建一个蓝牙体温计，该服务的相关规定可以参考 [Health Thermometer Profile](https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.health_thermometer.xml&u=org.bluetooth.service.health_thermometer.xml)，这里将包含两个 Characteristic：

* [Temperature Measurement](https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.temperature_measurement.xml) - 用于发送温度测量值，这里用软件进行模拟，初始温度 39.6，每 500ms 增加 0.1，超过 43 就重新开始计数；

* [Temperature Type](https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.temperature_type.xml) - 用于描述体温计的位置，这里值为 `0x03`，即位于耳朵处。


## 源代码

``` sh
#include <events/mbed_events.h>
#include "mbed.h"
#include "ble/BLE.h"
#include "ble/services/HealthThermometerService.h"

DigitalOut led1(LED1, 1);

const static char     DEVICE_NAME[]        = "Therm";
static const uint16_t uuid16_list[]        = {GattService::UUID_HEALTH_THERMOMETER_SERVICE};

static float                     currentTemperature   = 39.6;
static HealthThermometerService *thermometerServicePtr;

static EventQueue eventQueue(
    /* event count */ 16 * /* event size */ 32
);

/* Restart Advertising on disconnection*/
void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *)
{
    BLE::Instance().gap().startAdvertising();
}

void updateSensorValue(void) {
    /* Do blocking calls or whatever is necessary for sensor polling.
       In our case, we simply update the Temperature measurement. */
    currentTemperature = (currentTemperature + 0.1 > 43.0) ? 39.6 : currentTemperature + 0.1;
    thermometerServicePtr->updateTemperature(currentTemperature);
}

void periodicCallback(void)
{
    led1 = !led1; /* Do blinky on LED1 while we're waiting for BLE events */

    if (BLE::Instance().gap().getState().connected) {
        eventQueue.call(updateSensorValue);
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

    ble.gap().onDisconnection(disconnectionCallback);

    /* Setup primary service. */
    thermometerServicePtr = new HealthThermometerService(ble, currentTemperature, HealthThermometerService::LOCATION_EAR);

    /* setup advertising */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::THERMOMETER_EAR);
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
$ cd ./BLE_Thermometer

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_Thermometer
Elf2Bin: BLE_Thermometer
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   205 |     7 |   43 |
| Misc                  | 28980 |  2492 |  230 |
| drivers               |   732 |     0 |    0 |
| events                |    48 |     0 |    0 |
| events/equeue         |  1250 |     0 |   61 |
| features/FEATURE_BLE  | 16412 |     5 |  540 |
| hal                   |   506 |     0 |   16 |
| platform              |  1526 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 23679 |   140 | 2200 |
| Subtotals             | 79865 |  2672 | 7636 |
+-----------------------+-------+-------+------+
Allocated Heap: 40940 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10308 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 82537 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_Thermometer.hex

```

## 下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `BLE_Thermometer.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_Thermometer.hex

```
打开 **nRF Connect** 应用，将发现该设备，连接后读取体温计的数据：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-thermmeter.png)

Enjoy !

## 推荐资源

* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)

* [mbed-os-example-ble/BLE_Thermometer](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_Thermometer)

* [Health Thermometer Service](https://www.bluetooth.com/zh-cn/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.health_thermometer.xml)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>
