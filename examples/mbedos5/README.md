# Getting started with mbed OS examples

This guide reviews the steps required to get the examples working on nRF52832-MDK.

## Using Arm Mbed CLI

In the Arm Mbed ecosystem, you have a choice in how you want to develop, Online and Offline. For offline development, Arm Mbed CLI is provided. Mbed CLI is compatible with Windows, Linux and OSX. This option provides more configuration options but also requires slightly more setup.

The setup process for Arm Mbed CLI depends on your operating system. Please choose your host operating system. The setup instructions for each operating system walk you through how to install Mbed CLI locally.

On Windows the easiest way to install Mbed CLI to run the [Mbed CLI Windows .exe installer](https://mbed-media.mbed.com/filer_public/50/38/5038849b-16a8-42f3-be7a-43d98c7a3af3/mbed_installer_v043.exe).

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

## Compile and program the board

Now you can try to compile one of the examples. Will use the `mbed-os-example-blinky` example here to keep it simple.

Open terminal and change directory to:

``` sh
$ cd ./nrf52832-mdk/examples/mbedos5/mbed-os-example-blinky/
```

Add mbed-os library to the example directory:

``` sh
mbed-os-example-blinky$ mbed add https://github.com/makerdiary/mbed-os.git
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
