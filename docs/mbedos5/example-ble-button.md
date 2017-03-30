# 低功耗蓝牙示例：按钮

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/mbedos5/mbed-os-example-ble/BLE_Button"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍
本例将自定义一个 UUID 为 `0xA000` 的首要服务，该服务包含一个 UUID 为 `0xA001` 的 Characteristic，通过读取其属性值便可以获得按钮的状态。

!!! note "提示"
    本例可以作为如何创建 BLE Service 的模板，例如可以用来创建读取其他传感器数值的 Service。


## 源代码

[蓝牙联盟](https://www.bluetooth.com/specifications/gatt/services)没有定义名为 `Button` GATT Service，因此我们需要自定义该服务：

``` c
// ButtonService.h

class ButtonService {
public:
    const static uint16_t BUTTON_SERVICE_UUID              = 0xA000;
    const static uint16_t BUTTON_STATE_CHARACTERISTIC_UUID = 0xA001;

    ButtonService(BLE &_ble, bool buttonPressedInitial) :
        ble(_ble), buttonState(BUTTON_STATE_CHARACTERISTIC_UUID, &buttonPressedInitial, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY)
    {
        GattCharacteristic *charTable[] = {&buttonState};
        GattService         buttonService(ButtonService::BUTTON_SERVICE_UUID, charTable, sizeof(charTable) / sizeof(GattCharacteristic *));
        ble.gattServer().addService(buttonService);
    }

    void updateButtonState(bool newState) {
        ble.gattServer().write(buttonState.getValueHandle(), (uint8_t *)&newState, sizeof(bool));
    }

private:
    BLE                              &ble;
    ReadOnlyGattCharacteristic<bool>  buttonState;
};

```

主程序源码如下，只要按钮按下或释放就更新 Characteristic 的属性值：

``` c
#include <events/mbed_events.h>

#include <mbed.h>
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ButtonService.h"

DigitalOut  led1(LED1, 1);
InterruptIn button(BLE_BUTTON_PIN_NAME);

static EventQueue eventQueue(
    /* event count */ 10 * /* event size */ 32
);

const static char     DEVICE_NAME[] = "Button";
static const uint16_t uuid16_list[] = {ButtonService::BUTTON_SERVICE_UUID};

ButtonService *buttonServicePtr;

void buttonPressedCallback(void)
{
    eventQueue.call(Callback<void(bool)>(buttonServicePtr, &ButtonService::updateButtonState), true);
}

void buttonReleasedCallback(void)
{
    eventQueue.call(Callback<void(bool)>(buttonServicePtr, &ButtonService::updateButtonState), false);
}

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    BLE::Instance().gap().startAdvertising(); // restart advertising
}

void blinkCallback(void)
{
    led1 = !led1; /* Do blinky on LED1 to indicate system aliveness. */
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
    if(ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }

    ble.gap().onDisconnection(disconnectionCallback);

    button.fall(buttonPressedCallback);
    button.rise(buttonReleasedCallback);

    /* Setup primary service. */
    buttonServicePtr = new ButtonService(ble, false /* initial value for button pressed */);

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
$ cd ./BLE_Button

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_Button
Elf2Bin: BLE_Button
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   192 |     3 |   47 |
| Misc                  | 28761 |  2488 |  274 |
| drivers               |  1203 |     0 |    0 |
| events                |    48 |     0 |    0 |
| events/equeue         |  1250 |     0 |   61 |
| features/FEATURE_BLE  | 16408 |     5 |  540 |
| hal                   |   550 |     0 |   16 |
| platform              |  1526 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 24011 |   140 | 2200 |
| Subtotals             | 80476 |  2664 | 7684 |
+-----------------------+-------+-------+------+
Allocated Heap: 40900 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10348 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 83140 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_Button.hex
```

## 下载运行
将 nRF52832-MDK 连接到 PC，拷贝 `BLE_Button.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_Button.hex

```

## 验证
在 nRF52832-MDK 的 P25 引脚上连接一个轻触按钮，按钮按下时，P25 下拉到低电平：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-button.jpg)

打开 **nRF Connect** 应用，将扫描到设备，发现一个 ServiceUUID 为 `0xA000` 的按钮服务，该服务包含一个 UUID 为 `0xA001` 的 Characteristic，读取其属性值，即可获得按钮的状态：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-button-app.gif)

Enjoy !


## 推荐资源
* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)
* [mbed-os-example-ble/BLE_Button](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_Button)
* [Bluetooth Specifications](https://www.bluetooth.com/specifications)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

