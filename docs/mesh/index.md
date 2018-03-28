# nRF5 SDK for Mesh <br><small>Software Development Kit for Bluetooth mesh solutions using nRF51 Series and nRF52 Series</small>

## Introduction

The nRF5 SDK for Mesh is Nordic Semiconductor's implementation of the [Bluetooth Mesh](https://www.bluetooth.com/bluetooth-technology/topology-options/le-mesh). It allows applications to utilize the features provided by the Bluetooth Mesh when running on Nordic's nRF5 Series chips.

The addition of mesh networking capabilities to Bluetooth Low Energy enables extended range, increases the number of nodes and a many-to-many (m:m) communication topology. In a Bluetooth mesh network every received packet is broadcasted by relays, until the packet is received by the destination node (a so called managed flooding mesh.) A simple, but effective way of spreading information across the mesh network with no single point of failure. The mesh functionality is a significant update and enables new applications for Bluetooth in smart home, lighting, beaconing and asset tracking applications.

![](images/mesh-network_topology.png)

For more information, go to [nordicsemi.com](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK-for-Mesh).

## Getting started

At first, download the SDK file `nrf5_SDK_for_Mesh_x.x.x_src.zip` (for example, `nrf5_SDK_for_Mesh_v1.0.1_src`) from [nordicsemi.com](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF5-SDK-for-Mesh).

Extract the zip file to the directory that you want to use to work with the SDK.

To get started, take a look at the *Light switch* demo. It shows how a simple application can use the mesh stack and serves as an introduction to the Bluetooth Mesh concepts and nRF5 SDK for Mesh features and APIs.

Bluetooth Mesh introduces a whole new network topology and concepts to Bluetooth. An introduction is given in the [Basic Bluetooth Mesh concepts](http://infocenter.nordicsemi.com/topic/com.nordic.infocenter.meshsdk.v1.0.1/md_doc_introduction_basic_concepts.html) guide. More useful information may be found in [Bluetooth SIG's introduction to Bluetooth Mesh](https://blog.bluetooth.com/introducing-bluetooth-mesh-networking) and the [Bluetooth Mesh Networking Specifications](https://www.bluetooth.com/specifications/mesh-specifications?_ga=2.18257471.723078495.1501226603-93769939.1480503530).


### Adding support for nRF52832-MDK

To add support for nRF52832-MDK, you must create a support file with the name `custom_board.h`. This file must be located in a directory in the `<SDK>/external/nRF5_SDK_14.2.0_17b948a/components/boards/` path. 

The easiest way to create the `custom_board.h` file is to start with an existing platform definition file (for example `pca10040.h`) and adapt it to:

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

Create a CMake file with the name `nrf52832_mdk.cmake`. This file must be located in `<SDK>/CMake/board/`.

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

Specify the nRF52832-MDK board for the `nrf52832_xxAA` platform by adapting the `<SDK>/CMake/Board.cmake` file to:

``` sh
# 
...

elseif (PLATFORM STREQUAL "nrf52832_xxAA")
    set(BOARD "nrf52832_mdk" CACHE STRING "Board to build examples for.")
    set_property(CACHE BOARD PROPERTY STRINGS "nrf52832_mdk")
...
```

### Installing the mesh toolchain

To build the example applications, a toolchain based on CMake is required. Install instructions are provided for macOS. The steps should be similar for other platforms.

Install tools needed for building the mesh stack:

``` sh
$ brew install cmake ninja
```

### Building the mesh stack and examples

Good practice is to create a build folder in the root directory for the mesh stack repository, where all artifacts generated by the Ninja build system are stored, such as:

``` sh
$ mkdir build
$ cd build
```

Before you can build with Ninja, you must generate the correct build files with CMake.

You can specify the required `TOOLCHAIN`, `PLATFORM` and `BOARD` name to CMAKE. The build system will ensure a valid `SOFTDEVICE` for the given platform.

``` sh
build$ cmake -G Ninja -DTOOLCHAIN=gccarmemb -DPLATFORM=nrf52832_xxAA -DBOARD=nrf52832_mdk ..
```

### Programming the SoftDevice

!!! note
	All examples built by the CMake-generated build system do not include the SoftDevice as part of the generated HEX files. Therefore, the SoftDevice must already be present on the device before flashing the HEX file for the example mesh application.

Download the SoftDevice which you chose to build mesh stack with. If you do not know the SoftDevice version that was used to build the mesh stack, check the name of the example binary.

For example, if the example's binary name is `light_switch_client_nrf52832_xxAA_s132_5.0.0.hex`, the required SoftDevice binary is `s132_nrf52_5.0.0_softdevice.hex`. To program this example run:

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./external/softdevice/s132_5.0.0/s132_nrf52_5.0.0_softdevice.hex
```

### Programming the application

To program the example binary mentioned above, run:

``` sh
$ pyocd-flashtool -d debug -t nrf52 -se ./build/examples/light_switch/client/light_switch_client_nrf52832_xxAA_s132_5.0.0.hex
```

## Any Issue ?

Interested in contributing to this project? Want to report a bug? Feel free and click here:

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="indigo"><i class="fa fa-github"></i> New Issue</button></a>

<a href="https://join.slack.com/t/makerdiary/shared_invite/enQtMzIxNTA4MjkwMjc2LTM5MzcyNDhjYjI3YjEwOWE1YzM3YmE0YWEzNGNkNDU3NmE5M2M0MWYyM2QzZTFkNzQ2YjdmMWJlZjIwYmQwMDk"><button data-md-color-primary="red"><i class="fa fa-slack"></i> Add to Slack</button></a>

