# nRF5 SDK for Mesh <br><small>Nordic 官方的 Bluetooth Mesh 软件开发包</small>

## 简介

The nRF5 SDK for Mesh 是 Nordic 为在 nRF5 系列芯片上运行标准 [Bluetooth Mesh](https://www.bluetooth.com/bluetooth-technology/topology-options/le-mesh) 协议而推出的软件开发包。利用该软件开发包，开发者可以很方便地构建 Bluetooth Mesh 应用。

mesh 是低功耗蓝牙的一种全新网络拓扑结构选择，于2017年夏季推出。它代表蓝牙技术的一项重要进展，将蓝牙定位为包括智能楼宇和工业物联网在内的各大新领域和新用例的主流低功耗无线通信技术。

在 mesh 网络中，中继节点（RN）会将接收到的数据包不断广播下去，直到目标节点（N）收到该数据包为止，这是一种称为‘泛洪（flooding）’的网络结构。

![](../../mesh/images/mesh-network_topology.png)

了解更多信息, 可以前往 [nordicsemi.com](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK-for-Mesh)。

## 快速入门

你可以按照以下步骤快速搭建开发环境、编译运行简单的 mesh 示例。

### 下载 nRF5 SDK for Mesh

nRF5 SDK for Mesh 以 `.zip` 压缩包的方式发布，你可以直接前往 [nordicsemi.com](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK-for-Mesh) 网站下载。

下载完成后，将压缩包解压到你的工作目录。为了简单起见，我们以 *Light switch* demo 为例。

### 支持 nRF52832-MDK

为了支持 nRF52832-MDK 硬件，你需要在 `<SDK>/external/nRF5_SDK_14.2.0_17b948a/components/boards/` 目录创建一个名为 `custom_board.h` 的配置文件，具体配置如下（可以使用 `pca10040.h` 作为模板）：

``` c
// custom_board.h

#ifndef CUSTOM_BOARD_H
#define CUSTOM_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

// LEDs definitions for nRF52832-MDK
#define LEDS_NUMBER    3

#define LED_START      22
#define LED_1          22
#define LED_2          23
#define LED_3          24
#define LED_4          25
#define LED_STOP       24

#define LEDS_ACTIVE_STATE 0

#define LEDS_INV_MASK  LEDS_MASK

#define LEDS_LIST { LED_1, LED_2, LED_3 }

#define BSP_LED_0      LED_1
#define BSP_LED_1      LED_2
#define BSP_LED_2      LED_3
#define BSP_LED_3      LED_4

#define BUTTONS_NUMBER 1
#define BUTTON_START   18
#define BUTTON_1       18
#define BUTTON_STOP    18
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST { BUTTON_1}

#define BSP_BUTTON_0   BUTTON_1

#define RX_PIN_NUMBER  19
#define TX_PIN_NUMBER  20
#define CTS_PIN_NUMBER 7    // not available
#define RTS_PIN_NUMBER 5    // not available
#define HWFC           false

// Low frequency clock source to be used by the SoftDevice
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_XTAL,            \
                                 .rc_ctiv       = 0,                                \
                                 .rc_temp_ctiv  = 0,                                \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}

#ifdef __cplusplus
}
#endif

#endif // CUSTOM_BOARD_H
```

还需要创建一个 CMake 文件 `nrf52832_mdk.cmake`，该文件位于 `<SDK>/CMake/board/`，具体内容如下：

``` sh
# nrf52832_mdk.cmake

set(nrf52832_mdk_DEFINES
    -DBOARD_CUSTOM
    -DCONFIG_GPIO_AS_PINRESET)
set(nrf52832_mdk_INCLUDE_DIRS
    "${SDK_ROOT}/components/boards"
    "${SDK_ROOT}/components/drivers_nrf/hal"
    "${SDK_ROOT}/components/libraries/util")
```

然后修改 `<SDK>/CMake/Board.cmake` 文件，指定 nRF52832-MDK 为 `nrf52832_xxAA` 平台默认的板子：

``` sh
# 
...

elseif (PLATFORM STREQUAL "nrf52832_xxAA")
    set(BOARD "nrf52832_mdk" CACHE STRING "Board to build examples for.")
    set_property(CACHE BOARD PROPERTY STRINGS "nrf52832_mdk")
...

```

### 安装 mesh 开发工具链

在开始编译之前，需要安装 `CMake` 和 `ninja` 工具。这里以 macOS 系统为例，其他系统方法大同小异，可以自行摸索：

``` sh
$ brew install cmake ninja
```

### 编译 mesh 示例

这里最简单的方法是在 SDK 根目录创建 `build` 文件夹，用来存放 ninja 生成的文件：

``` sh
$ mkdir build
$ cd build
```

接着使用 `cmake` 工具来编译，你需要指定 `TOOLCHAIN`、`PLATFORM`、`BOARD` 参数，系统会自动确定可用的 `SOFTDEVICE`：

``` sh
build$ cmake -G Ninja -DTOOLCHAIN=gccarmemb -DPLATFORM=nrf52832_xxAA -DBOARD=nrf52832_mdk ..
```

### 下载运行 mesh 示例

Cmake 生成的固件不包含 SoftDevice，因此需要先下载对应的 SoftDevice。SoftDevice 的版本可以从前面生成的 `.hex` 文件中获得，例如：`light_switch_client_nrf52832_xxAA_s132_5.0.0.hex` 对应的 SoftDevice 是 `s132_nrf52_5.0.0_softdevice.hex`。 运行以下命令下载：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./external/softdevice/s132_5.0.0/s132_nrf52_5.0.0_softdevice.hex
```

接着便可下载应用程序：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -se ./build/examples/light_switch/client/light_switch_client_nrf52832_xxAA_s132_5.0.0.hex
```

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 或 [Slack](https://join.slack.com/t/makerdiary/shared_invite/enQtMzIxNTA4MjkwMjc2LTM5MzcyNDhjYjI3YjEwOWE1YzM3YmE0YWEzNGNkNDU3NmE5M2M0MWYyM2QzZTFkNzQ2YjdmMWJlZjIwYmQwMDk) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="indigo"><i class="fa fa-github"></i> 创建 Issue</button></a>

<a href="https://join.slack.com/t/makerdiary/shared_invite/enQtMzIxNTA4MjkwMjc2LTM5MzcyNDhjYjI3YjEwOWE1YzM3YmE0YWEzNGNkNDU3NmE5M2M0MWYyM2QzZTFkNzQ2YjdmMWJlZjIwYmQwMDk"><button data-md-color-primary="red"><i class="fa fa-slack"></i> 加入 Slack</button></a>

