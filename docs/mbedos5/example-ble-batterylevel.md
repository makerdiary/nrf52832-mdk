# 低功耗蓝牙示例：获取电池电量

<a href="https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_BatteryLevel"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 功能介绍
本例会创建一个电池电量的首要服务，连接后每隔 500ms 更新一次电池电量。作为示例，让电池电量从 50% 每隔 500ms 增加 1%，超过 100% 再回到 20% 继续增加，以此来模拟电池的使用场景。

## 源代码

``` c
#include <events/mbed_events.h>
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/services/BatteryService.h"

DigitalOut led1(LED1, 1);

const static char     DEVICE_NAME[] = "BATTERY";
static const uint16_t uuid16_list[] = {GattService::UUID_BATTERY_SERVICE};

static uint8_t batteryLevel = 50;
static BatteryService* batteryServicePtr;

static EventQueue eventQueue(
    /* event count */ 16 * /* event size */ 32
);

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    BLE::Instance().gap().startAdvertising();
}

void updateSensorValue() {
    batteryLevel++;
    if (batteryLevel > 100) {
        batteryLevel = 20;
    }

    batteryServicePtr->updateBatteryLevel(batteryLevel);
}

void blinkCallback(void)
{
    led1 = !led1; /* Do blinky on LED1 while we're waiting for BLE events */

    BLE &ble = BLE::Instance();
    if (ble.gap().getState().connected) {
        eventQueue.call(updateSensorValue);
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

    /* Setup primary service */
    batteryServicePtr = new BatteryService(ble, batteryLevel);

    /* Setup advertising */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *) uuid16_list, sizeof(uuid16_list));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *) DEVICE_NAME, sizeof(DEVICE_NAME));
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
$ cd ./BLE_BatteryLevel

# 添加已经克隆到本地的 mbed-os 库。若有疑问，请参考 Blinky 示例。
$ mbed add <your-mbed-os-path>

# 编译
$ mbed compile -m NRF52832_MDK -t GCC_ARM

# 若编译成功，将输出以下信息：

...

Link: BLE_BatteryLevel
Elf2Bin: BLE_BatteryLevel
+-----------------------+-------+-------+------+
| Module                | .text | .data | .bss |
+-----------------------+-------+-------+------+
| Fill                  |   167 |    10 |   43 |
| Misc                  | 28414 |  2489 |  230 |
| drivers               |   732 |     0 |    0 |
| events                |    48 |     0 |    0 |
| events/equeue         |  1250 |     0 |   61 |
| features/FEATURE_BLE  | 16408 |     5 |  540 |
| hal                   |   506 |     0 |   16 |
| platform              |  1526 |     4 |  264 |
| rtos                  |   102 |     4 |    4 |
| rtos/rtx              |  6425 |    20 | 4278 |
| targets/TARGET_NORDIC | 23679 |   140 | 2200 |
| Subtotals             | 79257 |  2672 | 7636 |
+-----------------------+-------+-------+------+
Allocated Heap: 40940 bytes
Allocated Stack: 2048 bytes
Total Static RAM memory (data + bss): 10308 bytes
Total RAM memory (data + bss + heap + stack): 53296 bytes
Total Flash memory (text + data + misc): 81929 bytes

Image: ./BUILD/NRF52832_MDK/GCC_ARM/BLE_BatteryLevel.hex
```

## 下载运行
将 nRF52832-MDK 连接到 PC，拷贝 `BLE_BatteryLevel.hex` 到 **DAPLINK** 可移动磁盘，按下复位键运行。

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./BUILD/NRF52832_MDK/GCC_ARM/BLE_BatteryLevel.hex

```

## 验证
打开 **nRF Connect** 应用，将扫描到设备，发现一个 ServiceUUID 为 `0x180F` 的电池服务，该服务包含一个名为 `Battery Level` 的 Characteristic，读取 `Battery Level` 的属性值，就可以获得设备的电池电量。


![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-os-battery-app.png)

!!! note "提醒"
    如果电池服务里面出现很多 `Unknown Characteristic`，这是因为 nRF52832-MDK 运行过其他 BLE 示例，手机将这些 `Characteristics` 缓存下来了。解决方法是在系统设置将蓝牙关闭再重新打开，以清除缓存。

## 小彩蛋
如果你正在使用 Chrome 浏览器查看本文档，同时你的设备也支持 BLE，可以直接通过以下按钮获取设备电量：


<button id = "btn_get_battery_level" data-md-color-primary="indigo">获取电池电量</button>
<div id="output" class="output">
  <pre id="log">Output log here.</pre>
</div>

![](https://img.makerdiary.co/wiki/nrf52832mdk/web-bluetooth-battery-level-1.png)

!!! tip "说明"
    这是基于最新的 Web Bluetooth API 实现的，通过浏览器就可以与蓝牙设备通信，很酷吧~ 感兴趣的话下面有相关的推荐资源链接，这里暂不深入，后续有专门的章节开展这方面的内容。

<script>
  var ChromeSamples = {
    log: function() {
      var line = Array.prototype.slice.call(arguments).map(function(argument) {
        return typeof argument === 'string' ? argument : JSON.stringify(argument);
      }).join(' ');

      document.querySelector('#log').textContent += line + '\n';
    },

    clearLog: function() {
      document.querySelector('#log').textContent = '';
    },

    setStatus: function(status) {
      document.querySelector('#status').textContent = status;
    },

    setContent: function(newContent) {
      var content = document.querySelector('#content');
      while(content.hasChildNodes()) {
        content.removeChild(content.lastChild);
      }
      content.appendChild(newContent);
    }
  };
</script>

<script>
  if (/Chrome\/(\d+\.\d+.\d+.\d+)/.test(navigator.userAgent)){
    // Let's log a warning if the sample is not supposed to execute on this
    // version of Chrome.
    if (45 > parseInt(RegExp.$1)) {
      ChromeSamples.setStatus('Warning! Keep in mind this sample has been tested with Chrome ' + 45 + '.');
    }
  }
</script>

<script>
function onButtonClick() {
  log('Requesting Bluetooth Device...');
  navigator.bluetooth.requestDevice(
    {filters: [{services: ['battery_service']}]})
  .then(device => {
    log('Connecting to GATT Server...');
    return device.gatt.connect();
  })
  .then(server => {
    log('Getting Battery Service...');
    return server.getPrimaryService('battery_service');
  })
  .then(service => {
    log('Getting Battery Level Characteristic...');
    return service.getCharacteristic('battery_level');
  })
  .then(characteristic => {
    log('Reading Battery Level...');
    return characteristic.readValue();
  })
  .then(value => {
    let batteryLevel = value.getUint8(0);
    log('> Battery Level is ' + batteryLevel + '%');
  })
  .catch(error => {
    log('Argh! ' + error);
  });
}
</script>
    
<script>
  document.querySelector('#btn_get_battery_level').addEventListener('click', function() {
    if (isWebBluetoothEnabled()) {
      ChromeSamples.clearLog();
      onButtonClick();
    }
  });
</script>

<script>
  log = ChromeSamples.log;

  function isWebBluetoothEnabled() {
    if (navigator.bluetooth) {
      return true;
    } else {
      ChromeSamples.setStatus('Web Bluetooth API is not available.\n' +
          'Please make sure the "Experimental Web Platform features" flag is enabled.');
      return false;
    }
  }
</script>

## 推荐资源

* [mbed BLE API 文档](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/communication/ble/)

* [BLE Battery Service](https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.battery_service.xml&u=org.bluetooth.service.battery_service.xml)

* [mbed-os-example-ble/BLE_BatteryLevel](https://github.com/makerdiary/mbed-os-example-ble/tree/master/BLE_BatteryLevel)

* [Web Bluetooth API](https://webbluetoothcg.github.io/web-bluetooth/)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

