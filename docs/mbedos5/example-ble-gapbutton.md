# 低功耗蓝牙示例：GAP Button

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/mbedos5/mbed-os-example-ble/BLE_GAPButton"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍
你可能已经试验过前面介绍的 [BLE Button](/mbedos5/example-ble-button/) 示例，你会发现它其实是需要先建立 GATT 连接，然后再去读取 Button 服务里的 Characteristic 属性值。

本例通过另外一种方式来获取按钮的状态，只要按钮状态发生变化，就将按键次数放到广播数据包中发送出来，创建一个无需建立连接的按钮示例。


## 源代码

``` c
#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"

DigitalOut  led1(LED1, 1);
InterruptIn button(BLE_BUTTON_PIN_NAME);
uint8_t cnt;

// Change your device name below
const char DEVICE_NAME[] = "GAPButton";

/* We can arbiturarily choose the GAPButton service UUID to be 0xAA00
 * as long as it does not overlap with the UUIDs defined here:
 * https://developer.bluetooth.org/gatt/services/Pages/ServicesHome.aspx */
#define GAPButtonUUID 0xAA00
const uint16_t uuid16_list[] = {GAPButtonUUID};

static EventQueue eventQueue(
    /* event count */ 16 * /* event size */ 32
);

void print_error(ble_error_t error, const char* msg)
{
    printf("%s: ", msg);
    switch(error) {
        case BLE_ERROR_NONE:
            printf("BLE_ERROR_NONE: No error");
            break;
        case BLE_ERROR_BUFFER_OVERFLOW:
            printf("BLE_ERROR_BUFFER_OVERFLOW: The requested action would cause a buffer overflow and has been aborted");
            break;
        case BLE_ERROR_NOT_IMPLEMENTED:
            printf("BLE_ERROR_NOT_IMPLEMENTED: Requested a feature that isn't yet implement or isn't supported by the target HW");
            break;
        case BLE_ERROR_PARAM_OUT_OF_RANGE:
            printf("BLE_ERROR_PARAM_OUT_OF_RANGE: One of the supplied parameters is outside the valid range");
            break;
        case BLE_ERROR_INVALID_PARAM:
            printf("BLE_ERROR_INVALID_PARAM: One of the supplied parameters is invalid");
            break;
        case BLE_STACK_BUSY:
            printf("BLE_STACK_BUSY: The stack is busy");
            break;
        case BLE_ERROR_INVALID_STATE:
            printf("BLE_ERROR_INVALID_STATE: Invalid state");
            break;
        case BLE_ERROR_NO_MEM:
            printf("BLE_ERROR_NO_MEM: Out of Memory");
            break;
        case BLE_ERROR_OPERATION_NOT_PERMITTED:
            printf("BLE_ERROR_OPERATION_NOT_PERMITTED");
            break;
        case BLE_ERROR_INITIALIZATION_INCOMPLETE:
            printf("BLE_ERROR_INITIALIZATION_INCOMPLETE");
            break;
        case BLE_ERROR_ALREADY_INITIALIZED:
            printf("BLE_ERROR_ALREADY_INITIALIZED");
            break;
        case BLE_ERROR_UNSPECIFIED:
            printf("BLE_ERROR_UNSPECIFIED: Unknown error");
            break;
        case BLE_ERROR_INTERNAL_STACK_FAILURE:
            printf("BLE_ERROR_INTERNAL_STACK_FAILURE: internal stack faillure");
            break;
    }
    printf("\r\n");
}

void updatePayload(void)
{
    // Update the count in the SERVICE_DATA field of the advertising payload
    uint8_t service_data[3];
    service_data[0] = GAPButtonUUID & 0xff;
    service_data[1] = GAPButtonUUID >> 8;
    service_data[2] = cnt; // Put the button click count in the third byte
    ble_error_t err = BLE::Instance().gap().updateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, (uint8_t *)service_data, sizeof(service_data));
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Updating payload failed");
    }
}

void buttonPressedCallback(void)
{
    ++cnt;

    // Calling BLE api in interrupt context may cause race conditions
    // Using mbed-events to schedule calls to BLE api for safety
    eventQueue.call(updatePayload);
}

void blinkCallback(void)
{
    led1 = !led1;
}

void bleInitComplete(BLE::InitializationCompleteCallbackContext *context)
{
    BLE&        ble = context->ble;
    ble_error_t err = context->error;

    if (err != BLE_ERROR_NONE) {
        print_error(err, "BLE initialisation failed");
        return;
    }

    // Set up the advertising flags. Note: not all combination of flags are valid
    // BREDR_NOT_SUPPORTED: Device does not support Basic Rate or Enchanced Data Rate, It is Low Energy only.
    // LE_GENERAL_DISCOVERABLE: Peripheral device is discoverable at any moment
    err = ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Setting GAP flags failed");
        return;
    }

    // Put the device name in the advertising payload
    err = ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Setting device name failed");
        return;
    }

    err = ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list));
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Setting service UUID failed");
        return;
    }

    // The Service Data data type consists of a service UUID with the data associated with that service.
    // We will encode the number of button clicks in the Service Data field
    // First two bytes of SERVICE_DATA field should contain the UUID of the service
    uint8_t service_data[3];
    service_data[0] = GAPButtonUUID & 0xff;
    service_data[1] = GAPButtonUUID >> 8;
    service_data[2] = cnt; // Put the button click count in the third byte
    err = ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SERVICE_DATA, (uint8_t *)service_data, sizeof(service_data));
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Setting service data failed");
        return;
    }

    // It is not connectable as we are just boardcasting
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_NON_CONNECTABLE_UNDIRECTED);

    // Send out the advertising payload every 1000ms
    ble.gap().setAdvertisingInterval(1000);

    err = ble.gap().startAdvertising();
    if (err != BLE_ERROR_NONE) {
        print_error(err, "Sart advertising failed");
        return;
    }
}

void scheduleBleEventsProcessing(BLE::OnEventsToProcessCallbackContext* context) {
    BLE &ble = BLE::Instance();
    eventQueue.call(Callback<void()>(&ble, &BLE::processEvents));
}

int main()
{
    cnt = 0;

    BLE &ble = BLE::Instance();
    ble.onEventsToProcess(scheduleBleEventsProcessing);
    ble_error_t err = ble.init(bleInitComplete);
    if (err != BLE_ERROR_NONE) {
        print_error(err, "BLE initialisation failed");
        return 0;
    }

    // Blink LED every 500 ms to indicate system aliveness
    eventQueue.call_every(500, blinkCallback);

    // Register function to be called when button is released
    button.rise(buttonPressedCallback);

    eventQueue.dispatch_forever();

    return 0;
}

```

## 编译

``` sh
$ cd ./BLE_GAPButton

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_GAPButton
Elf2Bin: BLE_GAPButton
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   179 |     3 |   42 |
| Misc                  | 35190 |  2488 |  271 |
| drivers               |  1127 |     0 |    0 |
| events                |    48 |     0 |    0 |
| events/equeue         |  1250 |     0 |   61 |
| features/FEATURE_BLE  | 16350 |     5 |  540 |
| hal                   |   550 |     0 |   16 |
| platform              |  1526 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 24011 |   140 | 2200 |
| Subtotals             | 86758 |  2664 | 7676 |
+-----------------------+-------+-------+------+
Allocated Heap: 40908 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10340 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 89422 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_GAPButton.hex

```

## 下载运行

将 nRF52832-MDK 连接到 PC，拷贝 `BLE_GAPButton.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_GAPButton.hex

```

## 验证

在 nRF52832-MDK 的 P25 引脚上连接一个轻触按钮，按钮按下时，P25 下拉到低电平：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-button.jpg)

打开 **nRF Connect** 应用，将扫描到设备，发现其 `ServiceUUID` 为 `0xAA00`，按键次数包含在 `ServiceData` 中：

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-ble-gapbutton.gif)


Enjoy !

## 推荐资源
* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)
* [mbed-os-example-ble/BLE_GAPButton](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_GAPButton)
* [Bluetooth Specifications](https://www.bluetooth.com/specifications)


## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

