# nRF5 SDK for Mesh <br><small>Nordic 官方的 Bluetooth Mesh 软件开发包</small>

[![](../../mesh/images/mesh_icon_small.png)](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK-for-Mesh)

## 简介

The nRF5 SDK for Mesh 是 Nordic 为在 nRF5 系列芯片上运行标准 [Bluetooth Mesh](https://www.bluetooth.com/bluetooth-technology/topology-options/le-mesh) 协议而推出的软件开发包。利用该软件开发包，开发者可以很方便地构建 Bluetooth Mesh 应用。

mesh 是低功耗蓝牙的一种全新网络拓扑结构选择，于2017年夏季推出。它代表蓝牙技术的一项重要进展，将蓝牙定位为包括智能楼宇和工业物联网在内的各大新领域和新用例的主流低功耗无线通信技术。

在 mesh 网络中，中继节点（RN）会将接收到的数据包不断广播下去，直到目标节点（N）收到该数据包为止，这是一种称为‘泛洪（flooding）’的网络结构。

![](../../mesh/images/mesh-network_topology.png)

了解更多信息, 可以前往 [nordicsemi.com](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK-for-Mesh)。

## 快速入门

你可以在 Bluetooth SIG 了解更多关于 Bluetooth Mesh 的介绍：

* [Bluetooth Mesh 简介](https://blog.bluetooth.com/introducing-bluetooth-mesh-networking)

* [Bluetooth Mesh 协议规范](https://www.bluetooth.com/specifications/mesh-specifications?_ga=2.18257471.723078495.1501226603-93769939.1480503530)

按照以下步骤快速搭建开发环境、编译运行简单的 mesh 示例。

### 安装 Mesh 工具链

在开始编译之前，需要安装 `CMake`、`GNU Arm Toolchain`、`ninja` 工具。这里以 macOS 系统为例，其他系统方法大同小异，可以自行摸索。

#### CMake

[CMake](https://cmake.org/) 是个一个开源的跨平台自动化建构系统，用来管理软件建置的程序，并不相依于某特定编译器。并可支持多层目录、多个应用程序与多个库。 它用配置文件控制建构过程的方式和 Unix 的 `make` 相似，只是 CMake 的配置文件取名为 `CMakeLists.txt` 。

编译 Mesh 协议栈需要 `3.6` 及以上版本 CMake，在 macOS 系统可使用 `brew` 命令进行安装：

``` sh
$ brew install cmake
```

#### GNU Arm Embedded Toolchain

GNU Arm Embedded Toolchain 是 Arm 公司提供的 GNU 开源工具链，集成 GCC 交叉编译器、标准库以及其他实用工具，使开发者能够轻松开发基于 Arm Cortex-M 和 Cortex-R 的软件。该工具支持跨平台，可以运行在 Windows，Linux 和 macOS 平台上。

你可以通过以下链接下载该工具链：

<a href="https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads"><button data-md-color-primary="marsala">下载工具链</button></a>


下载、安装 `6-2017-q2-update` 版本，并将工具链的目录添加到系统环境变量中：

``` sh
# in ~/.bash_profile, add the following script
export PATH="<path to install directory>/gcc-arm-none-eabi-6-2017-q2-update/bin:${PATH}"
```

可通过以下命令验证是否安装成功：

``` sh
$ arm-none-eabi-gcc --version
```

#### Ninja

[Ninja](https://ninja-build.org/) 是一个小巧而高效的编译工具，相比于 `make`，Ninja 编译速度更快、更加灵活。

在 macOS 系统可使用 `brew` 命令安装 `Ninja`：

``` sh
$ brew install ninja
```

### 编译 Mesh 协议栈和示例

#### 下载 SDKs

`nRF5 SDK for Mesh` 需要配合 `nRF5 SDK` 一起编译，可通过以下链接下载这两个 SDK：

* [nRF5 SDK](http://www.nordicsemi.com/eng/nordic/Products/nRF5-SDK/nRF5-SDK-zip/59011)
* [nRF5 SDK for Mesh](http://www.nordicsemi.com/eng/nordic/Products/nRF5-SDK-for-Mesh/nRF5-SDK-for-Mesh/62377)

将这两个 SDK 解压到同一工作目录：

``` sh
.
├── nrf5_SDK_for_Mesh_v2.0.1_src/
└── nRF5_SDK_15.0.0_a53641a/
```

#### 添加 nRF52832-MDK 支持

为了支持 nRF52832-MDK 硬件，你需要在 `<SDK>/external/nRF5_SDK_15.0.0_a53641a/components/boards/` 目录创建一个名为 `custom_board.h` 的配置文件，具体配置如下（可以使用 `pca10040.h` 作为模板）：

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

还需要创建一个 CMake 文件 `nrf52832_mdk.cmake`，该文件位于 `nrf5_SDK_for_Mesh_v2.0.1_src/CMake/board/`，具体内容如下：

``` sh
# nrf52832_mdk.cmake

set(nrf52832_mdk_DEFINES
    -DBOARD_CUSTOM
    -DCONFIG_GPIO_AS_PINRESET)
set(nrf52832_mdk_INCLUDE_DIRS
    "${SDK_ROOT}/components/boards")
```

然后修改 `nrf5_SDK_for_Mesh_v2.0.1_src/CMake/Board.cmake` 文件，指定 nRF52832-MDK 为 `nrf52832_xxAA` 平台默认的板子：

``` sh
# 
...

elseif (PLATFORM STREQUAL "nrf52832_xxAA")
    set(BOARD "pca10040" CACHE STRING "Board to build examples for.")
    set_property(CACHE BOARD PROPERTY STRINGS "pca10040" "nrf52832_mdk")
...

```

#### 使用 Cmake 构建

这里最简单的方法是在 `nrf5_SDK_for_Mesh_v2.0.1_src` 根目录创建 `build` 文件夹，用来存放 ninja 生成的文件：

``` sh
nrf5_SDK_for_Mesh_v2.0.1_src $ mkdir build
nrf5_SDK_for_Mesh_v2.0.1_src $ cd build
```

接着使用 `cmake` 工具来编译，你需要指定 `TOOLCHAIN`、`PLATFORM`、`BOARD` 参数，系统会自动确定可用的 `SOFTDEVICE`：

``` sh
build$ cmake -G Ninja -DTOOLCHAIN=gccarmemb -DPLATFORM=nrf52832_xxAA -DBOARD=nrf52832_mdk ..
```

生成编译文件后便可使用 `ninja` 命令进行编译所有示例和库：

``` sh
build $ ninja
```

### 下载运行 mesh 示例

Cmake 生成的固件不包含 SoftDevice，因此需要先下载对应的 SoftDevice。SoftDevice 的版本可以从前面生成的 `.hex` 文件中获得，例如：`light_switch_client_nrf52832_xxAA_s132_6.0.0.hex` 对应的 SoftDevice 是 `s132_nrf52_6.0.0_softdevice.hex`。 运行以下命令下载：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce s132_nrf52_6.0.0_softdevice.hex
```

接着便可下载应用程序：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -se ./build/examples/light_switch/client/light_switch_client_nrf52832_xxAA_s132_6.0.0.hex
```

## 如何创建新项目

你也可以从头创建新项目，最简单的方式是：

1. 从 `examples` 目录复制现有的示例，例如：复制 `examples/beaconing`，并改为 `examples/my_app`；
2. 修改 `examples/CMakeLists.txt` 文件，添加 `add_subdirectory("my_app")` 命令；
3. 修改 `examples/my_app/CMakeLists.txt` 文件，修改 target：`set(target "my_app")`；
4. 生成编译文件：
    ``` sh
    nrf5_SDK_for_Mesh_v2.0.1_src $ mkdir build
    nrf5_SDK_for_Mesh_v2.0.1_src $ cd build
    build $ cmake -G Ninja -DTOOLCHAIN=gccarmemb -DPLATFORM=nrf52832_xxAA -DBOARD=nrf52832_mdk ..
    ```
5. 编译新的目标示例：
    ``` sh
    build $ ninja my_app
    ```

## 更多示例

至此，你应该已经掌握如何编译运行基于 nRF5 SDK for Mesh 的示例，我们还会不断增加一些新的示例到 [nrf52832-mdk](https://github.com/makerdiary/nrf52832-mdk) 仓库中，敬请关注！

## 参考资源

* [Nordic nRF5 SDK for Mesh](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK-for-Mesh)

* [Nordic Semiconductor Infocenter](http://infocenter.nordicsemi.com/index.jsp)

* [makerdiary/nrf52832-mdk](https://github.com/makerdiary/nrf52832-mdk)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="marsala"><i class="fa fa-github"></i> 创建 Issue</button></a>
