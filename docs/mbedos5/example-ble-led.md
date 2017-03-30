# 低功耗蓝牙示例：LED 外设

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/mbedos5/mbed-os-example-ble/BLE_LED"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍
本例将自定义一个 UUID 为 `0xA000` 的首要服务，该服务包含一个 UUID 为 `0xA001` 的 Characteristic，该 Characteristic 是可读可写的，对其写入数值即可控制 LED 的开关。

!!! note "提示"
    本例可以作为如何创建 BLE Service 的模板，例如可以用来给其他设备发送控制命令。

## 源代码

``` sh
#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "LEDService.h"

DigitalOut alivenessLED(LED1, 0);
DigitalOut actuatedLED(LED2, 0);

const static char     DEVICE_NAME[] = "LED";
static const uint16_t uuid16_list[] = {LEDService::LED_SERVICE_UUID};

static EventQueue eventQueue(
    /* event count */ 10 * /* event size */ 32
);

LEDService *ledServicePtr;

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    (void) params;
    BLE::Instance().gap().startAdvertising();
}

void blinkCallback(void)
{
    alivenessLED = !alivenessLED; /* Do blinky on LED1 to indicate system aliveness. */
}

/**
 * This callback allows the LEDService to receive updates to the ledState Characteristic.
 *
 * @param[in] params
 *     Information about the characterisitc being updated.
 */
void onDataWrittenCallback(const GattWriteCallbackParams *params) {
    if ((params->handle == ledServicePtr->getValueHandle()) && (params->len == 1)) {
        actuatedLED = *(params->data);
    }
}

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

    ble.gap().onDisconnection(disconnectionCallback);
    ble.gattServer().onDataWritten(onDataWrittenCallback);

    bool initialValueForLEDCharacteristic = false;
    ledServicePtr = new LEDService(ble, initialValueForLEDCharacteristic);

    /* setup advertising */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1000); /* 1000ms. */
    ble.gap().startAdvertising();
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
    eventQueue.call_every(500, blinkCallback);

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble.init(bleInitComplete);

    eventQueue.dispatch_forever();

    return 0;
}
```


## 编译

``` sh
$ cd ./BLE_LED

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_LED
Elf2Bin: BLE_LED
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   225 |     3 |   42 |
| Misc                  | 28420 |  2488 |  231 |
| drivers               |   732 |     0 |    0 |
| events                |    48 |     0 |    0 |
| events/equeue         |  1250 |     0 |   61 |
| features/FEATURE_BLE  | 16408 |     5 |  540 |
| hal                   |   506 |     0 |   16 |
| platform              |  1526 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 23679 |   140 | 2200 |
| Subtotals             | 79321 |  2664 | 7636 |
+-----------------------+-------+-------+------+
Allocated Heap: 40948 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10300 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 81985 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_LED.hex

```

## 下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `BLE_LED.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_LED.hex

```
打开 **nRF Connect** 应用，将发现该设备：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-led.png)

通过对其 Characteristic 写 `0x00` 或 `0x01` 来控制 LED(红色)：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-led-ctl.png)

Enjoy !

## 推荐资源

* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)

* [mbed-os-example-ble/BLE_LED](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_LED)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>
