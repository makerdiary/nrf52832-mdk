# Getting started with Blinky on mbed OS

This guide reviews the steps required to get Blinky working on an mbed OS platform.

## Using Arm Mbed CLI

In the Arm Mbed ecosystem, you have a choice in how you want to develop, Online and Offline. For offline development, Arm Mbed CLI is provided. Mbed CLI is compatible with Windows, Linux and OSX. This option provides more configuration options but also requires slightly more setup.

The setup process for Arm Mbed CLI depends on your operating system. Please choose your host operating system. The setup instructions for each operating system walk you through how to install Mbed CLI locally.

On Windows the easiest way to install Mbed CLI to run the [Mbed CLI Windows .exe installer](https://mbed-media.mbed.com/filer_public/7f/46/7f46e205-52f5-48e2-be64-8f30d52f6d75/mbed_installer_v041.exe).

On Linux and macOS, you can use Python and Pip:

``` sh
$ pip install mbed-cli
```

You can ensure Mbed CLI installed correctly by running `mbed help` from your command-line.

For any installed toolchain, be sure to add the Mbed CLI global configuration:

``` sh
$ mbed config -G GCC_ARM_PATH "<path to GNU Arm bin>"
[mbed] <path to GNU Arm bin> now set as global GCC_ARM_PATH

$ mbed config --list
[mbed] Global config:
GCC_ARM_PATH=<path to GNU Arm bin>
```

!!! note 
	You can also apply the same configuration to the IAR and ARM toolchains using `IAR_PATH` or `ARM_PATH`.

## Adding support for nRF52832-MDK

First clone the [mbed-os](https://github.com/ARMmbed/mbed-os) repository on GitHub or download it as a zip package and put its contents to your working directory.

``` sh
$ git clone https://github.com/ARMmbed/mbed-os.git
```

Add the target description to `mbed-os\targets\targets.json` using the following keys:

``` json
	"NRF52832_MDK": {
	    "inherits": ["MCU_NRF52"],
	    "macros_add": ["BOARD_PCA10040", "NRF52_PAN_12", "NRF52_PAN_15", "NRF52_PAN_58", "NRF52_PAN_55", "NRF52_PAN_54", "NRF52_PAN_31", "NRF52_PAN_30", "NRF52_PAN_51", "NRF52_PAN_36", "NRF52_PAN_53", "S132", "CONFIG_GPIO_AS_PINRESET", "BLE_STACK_SUPPORT_REQD", "SWI_DISABLE0", "NRF52_PAN_20", "NRF52_PAN_64", "NRF52_PAN_62", "NRF52_PAN_63"],
	    "device_has_add": ["ANALOGIN", "I2C", "I2C_ASYNCH", "INTERRUPTIN", "LOWPOWERTIMER", "PORTIN", "PORTINOUT", "PORTOUT", "PWMOUT", "RTC", "SERIAL", "SERIAL_ASYNCH", "SERIAL_FC", "SLEEP", "SPI", "SPI_ASYNCH", "SPISLAVE", "FLASH"],
	    "release_versions": ["2", "5"],
	    "device_name": "nRF52832_xxAA"
	},
```

!!! tip
	Arm Mbed uses JSON as a description language for its build targets. You can view the [Adding and configuring targets section](https://os.mbed.com/docs/v5.7/tools/adding-and-configuring-targets.html) describes for more details. 

To add support for the nRF52832-MDK board, you must create board files: `PinNames.h` and `device.h`. These files must be located in a directory in the `mbed-os/targets/TARGET_NORDIC/TARGET_NRF5/TARGET_MCU_NRF52832/TARGET_NRF52832_MDK/` path.

``` c
// PinNames.h

#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

#define PORT_SHIFT  3

typedef enum {
    p0  = 0,
    p1  = 1,
    p2  = 2,
    p3  = 3,
    p4  = 4,
    p5  = 5,
    p6  = 6,
    p7  = 7,
    p8  = 8,
    p9  = 9,
    p10 = 10,
    p11 = 11,
    p12 = 12,
    p13 = 13,
    p14 = 14,
    p15 = 15,
    p16 = 16,
    p17 = 17,
    p18 = 18,
    p19 = 19,
    p20 = 20,
    p21 = 21,
    p22 = 22,
    p23 = 23,
    p24 = 24,
    p25 = 25,
    p26 = 26,
    p27 = 27,
    p28 = 28,
    p29 = 29,
    p30 = 30,
    p31 = 31,
    // Not connected
    NC = (int)0xFFFFFFFF,

    LED1    = p22,
    LED2    = p23,
    LED3    = p24,

    BUTTON1 = p18,

    RX_PIN_NUMBER  = p19,
    TX_PIN_NUMBER  = p20,
    CTS_PIN_NUMBER = NC,   // not connected
    RTS_PIN_NUMBER = NC,   // not connected

    // mBed interface Pins
    USBTX = TX_PIN_NUMBER,
    USBRX = RX_PIN_NUMBER

} PinName;

typedef enum {
    PullNone = 0,
    PullDown = 1,
    PullUp = 3,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
```

``` c
// device.h

#ifndef MBED_DEVICE_H
#define MBED_DEVICE_H

#include "objects.h"

#endif
```

Commit your modifications:

``` sh
$ git add *
$ git commit -m "add support for nRF52832-MDK"
```

## Compile and program the board

Now you can try to compile this example. Open terminal and change directory to:

``` sh
$ cd ./nrf52832-mdk/examples/mbedos5/mbed-os-example-blinky/
```

Add mbed-os library to the example directory:

``` sh
mbed-os-example-blinky$ mbed add <path to mbed-os repository>
```

Invoke `mbed compile`, and specify the name of your platform and your installed toolchain (GCC_ARM, ARM, IAR). For example, for the NRF52832_MDK platform and GNU Arm embedded toolchain:

``` sh
mbed-os-example-blinky$ mbed compile --target NRF52832_MDK --toolchain GCC_ARM --flash
```

The `--flash` argument automatically flashes the compiled program onto your board if it is connected to your computer. You can add locally manufacturers id and platform name for nRF52832-MDK and see which boards are connected with `mbed detect`:

``` sh
$ mbedls --mock=1024:nRF52832-MDK
```

``` sh
...
Compile [ 96.0%]: gpio_api.c
Compile [ 96.3%]: lp_ticker.c
Compile [ 96.6%]: pinmap.c
Compile [ 96.9%]: nordic_critical.c
Compile [ 97.2%]: port_api.c
Compile [ 97.5%]: reloc_vector_table.c
Compile [ 97.8%]: i2c_api.c
Compile [ 98.2%]: rtc_api.c
Compile [ 98.5%]: trng_api.c
Compile [ 98.8%]: sleep.c
Compile [ 99.1%]: serial_api.c
Compile [ 99.4%]: us_ticker.c
Compile [ 99.7%]: spi_api.c
Compile [100.0%]: test_env.cpp
Link: mbed-os-example-blinky
Elf2Bin: mbed-os-example-blinky
Text 39KB Data 2.62KB BSS 7.01KB                   ROM 41.6KB RAM 9.64KB
ROM [|||                                                 ]  41.6KB/512KB
RAM [|||||||                                             ]   9.64KB/64KB
Image: ./BUILD/NRF52832_MDK/GCC_ARM/mbed-os-example-blinky.hex
[mbed] Detected "nRF52832-MDK" connected to "/Volumes/DAPLINK" and using com port "/dev/tty.usbmodem1412"
```
