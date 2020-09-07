# nRF52832-MDK

*An Open Source, Micro Development Kit for IoT Embedded Applications*

[中文文档](https://wiki.makerdiary.com/nrf52832-mdk/cn)

## Table of Contents

* [Introduction](#introduction)
* [Hardware Specification](#hardware-specification)
* [Pinout Diagram](#pinout-diagram)
* [Getting Started with nRF52832-MDK](#getting-started-with-nrf52832-mdk)
  * [Connecting the board](#connecting-the-board)
  * [Using terminal applications](#using-terminal-applications)
    * [On Windows](#on-windows)
    * [On macOS](#on-macos)
    * [On Linux](#on-linux)
  * [Using nRF Connect](#using-nrf-connect)
* [How to program nRF52832-MDK](#how-to-program-nrf52832-mdk)
  * [Drag-n-drop Programming](#drag-n-drop-programming)
  * [Using pyOCD](#using-pyocd)
* [Hardware Resource](#hardware-resource)
* [How to get an nRF52832-MDK](#how-to-get-an-nrf52832-mdk)
* [License](#license)

## Introduction

The nRF52832-MDK is a fully open-source, versatile single board development kit for *Bluetooth*® low energy, ANT and 2.4GHz proprietary applications using the [nRF52832](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52832) SoC.

The kit gives access to 24 I/Os and interfaces via headers and has a RGB LED which is user-programmable. It also has a 2.4GHz chip antenna onboard which is quite convenient to develop IoT wireless applications. 

It supports the Nordic offical SDK using GCC, Keil and IAR. Program/Debug options on the kit is [DAPLink](https://github.com/ARMmbed/DAPLink).

The nRF52832-MDK can also be used to play with multi-frameworks such as: nRF5 SDK, nRF5 SDK for Mesh, Mbed OS 5, Zephyr, Mynewt, NuttX, JerryScript, Espruino, MicroPython, Web Bluetooth, Eddystone, etc.

There are two versions of nRF52832-MDK available:

* **V1**: with Micro USB socket and headers soldered in

* **V2**: without headers soldered in and can be connected to PC without any cable

Just choose one that's right for you!

![V1&V2 image](/docs/images/nrf52832-mdk-v1-n-v2.jpg)

## Hardware Specification

nRF52832-MDK provides the following hardware features:

* Nordic Semiconductor nRF52832 

  * ARM® Cortex™-M4F
  * 512kB Flash Memory + 64kB RAM
  * Bluetooth low energy, ANT and 2.4GHz
  * NFC™-A tag

* Program/Debug options with DAPLink

  * MSC - drag-n-drop programming flash memory
  * CDC - virtual com port for log, trace and terminal emulation
  * HID - CMSIS-DAP compliant debug channel

* Up to 24 GPIOs at headers

* Reset Button

* RGB LED

* 2.4GHz chip antenna

* 3.3V regulator with 1A peak current output

* VBUS & VIN Power-Path Management

* Breadboard-friendly

* Two dimensions available:

  * V1: 48mm x 18mm x 13mm with headers soldered in
  * V2: 63mm x 18mm x 4mm without headers soldered in

## Pinout Diagram

![](/docs/images/nrf52832_mdk_v1_pinout.jpeg)

![](/docs/images/nrf52832_mdk_v2_pinout.jpeg)


## Getting Started with nRF52832-MDK

Congratulations on getting the nRF52832-MDK. In this section we'll show you how to connect and power up your nRF52832-MDK, and what to expect when you do so. This will take about 20 minutes.

### Connecting the board

You can connect the board by performing the following steps:

 1. Connect the nRF52832-MDK to one of your PC's USB host ports. This provides power to the board. Observe that two GREEN LEDs are on. One indicates that 3.3V power is good, and another indicates that the application is advertising.

 2. Open a file explorer. Confirm that the nRF52832-MDK has appeared as a removable drive named **DAPLINK**. This allows you to program the onboard chip. You'll do this under "[how to program nRF52832-MDK](#how-to-program-nrf52832-mdk)" below.

 ![](docs/getting-started/images/connecting_nrf52832-mdk.jpg)

### Using terminal applications

Terminal applications (for example [PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/) or [screen](https://www.gnu.org/software/screen/manual/screen.html)) run on your host PC. They provide a window where your board can print messages.

You can connect your board to your computer over USB. This should work out of the box on Linux and macOS, but for Windows earlier than Windows 10, you will probably need to install a serial port driver:

1. Download the [Arm Mbed Windows serial port driver](http://os.mbed.com/media/downloads/drivers/mbedWinSerial_16466.exe).

2. Plug in your board over USB. It mounts as a drive.

3. Close all Explorer windows showing the drive.

4. Run the installer. This may take some time or display a few "unsigned driver" warnings.

The default setup for the USB serial port is 115200 baud, 8 bits, 1 stop bit, no parity (115200-8-N-1)

#### On Windows

There are many terminal applications for Windows, [PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/) is recommended.

Start PuTTY and open the used COM port with the following configuration. If there is nothing printed, just press RESET button on the board.

![](/docs/getting-started/images/putty_configuration.png)

#### On macOS

[screen](https://www.gnu.org/software/screen/manual/screen.html) should work on macOS. In Terminal, run:

``` sh
$ screen /dev/cu.usbmodem1412 115200
```

If there is nothing printed, just press RESET button on the board.

You can type `screen /dev/cu.usb` first, and then press <kbd>tab</kbd>. The device name will autocomplete.

To end a screen session, type <kbd>Ctrl</kbd> + <kbd>A</kbd>, and then <kbd>Ctrl</kbd> + <kbd>K</kbd>. At the bottom of the Terminal window, you should see a `[y/n]` prompt. Type <kbd>y</kbd>.

#### On Linux

[screen](https://www.gnu.org/software/screen/manual/screen.html) should work on Linux. If for some reason it doesn't, you can try one of the following:

* [CoolTerm](http://freeware.the-meiers.org/)

* [Minicom](https://help.ubuntu.com/community/Minicom)

Here we will use screen on Ubuntu 16.04 for example. If there is nothing printed, just press RESET button on the board.

``` sh
$ sudo apt-get install screen
```

``` sh
$ sudo screen /dev/ttyACM0 115200
```

### Using nRF Connect

You can also test the application with nRF Connect by performing the following steps:

1. Get the **nRF Connect** app from App Store or Google Play.

2. Connect to the device from nRF Connect (the device is advertising as **nRF52832-MDK**).

3. Observe that BLUE LED is on and GREEN LED is off. This indicates that the connections is established.

4. Observe that the services are shown in the connected device.

5. Write `01` to the *Nordic Blinky LED* Characteristic (0x1525) and observe that RED LED is turned on.

6. Write `00` to the *Nordic Blinky LED* Characteristic (0x1525) and observe that RED LED is turned off.

![](/docs/getting-started/images/nrf_connect_blinky.jpg)

## How to program nRF52832-MDK

You have the following two options to program your nRF52832-MDK: 

* [Drag-n-drop Programming](#drag-n-drop-programming)

* [Using pyOCD](#using-pyocd).

### Drag-n-drop Programming

Drag and drop the hex file into the **DAPLINK** removable drive mentioned above. The file contains firmware which will be automatically programmed onto the nRF52832 SoC. This happens immediately. The explorer window may close when programming starts. When programming is completed, the unit will be re-detected by the computer. The application will run after pressing the RESET button.

It's so easy, right?

![](/docs/getting-started/images/drag-n-drop_programming.png)

### Using pyOCD

pyOCD is an Open Source python 2.7 based library for programming and debugging ARM Cortex-M microcontrollers using CMSIS-DAP. Linux, OSX and Windows are supported.

The latest stable version of pyOCD may be done via [pip](https://pip.pypa.io/en/stable/index.html) as follows:

``` sh
$ pip install --pre -U pyocd
```

Alternatively, to install the latest development version (master branch), you can do the following:

``` sh
$ pip install --pre -U https://github.com/mbedmicro/pyOCD/archive/master.zip
```

Note that you may run into permissions issues running these commands. You have a few options here:

* Run with `sudo -H` to install pyOCD and dependencies globally
* Specify the `--user` option to install local to your user
* Run the command in a [virtualenv](https://virtualenv.pypa.io/en/latest/) local to a specific project working set.

You can verify that your board is detected by pyOCD by running:

``` sh
$ pyocd-flashtool -l
```

and then flash the board by running:

``` sh
$ pyocd-flashtool -d debug -t nrf52 -se your-hex-file.hex
```

## Hardware Resource

| Releases | Design Files                   |
| -------- | ------------------------------ |
| V2.0     | [nRF52832-MDK V2.0 Schematic](/docs/hardware/nRF52832-MDK_SCH_V2.0.pdf)<br/>[nRF52832-MDK V2.0 PCB](/docs/hardware/nRF52832-MDK_PCB_V2.0.pdf)<br/>[nRF52832-MDK V2.0 3D STEP](/docs/hardware/nRF52832-MDK_3D_STEP_V2.0.step)|
| V1.1     | [nRF52832-MDK V1.1 Schematic](/docs/hardware/nRF52832-MDK_SCH_V1.1.pdf)<br/>[nRF52832-MDK V1.1 PCB](/docs/hardware/nRF52832-MDK_PCB_V1.1.pdf)<br/>[nRF52832-MDK V1.1 3D STEP](/docs/hardware/nRF52832-MDK_3D_STEP_V1.1.step)|
| V1.0     | [nRF52832-MDK V1.0 Schematic](/docs/hardware/nRF52832-MDK_SCH_V1.0.pdf)<br/>[nRF52832-MDK V1.0 PCB](/docs/hardware/nRF52832-MDK_PCB_V1.0.pdf)<br/>[nRF52832-MDK V1.0 3D STEP](/docs/hardware/nRF52832-MDK_3D_STEP_V1.0.step)|

## How to get an nRF52832-MDK

nRF52832-MDK is available on the following channels (click to go directly to the product):

[![makerdiary store](docs/images/makerdiary-store-logo.png)](https://store.makerdiary.com/collections/frontpage)

[![SeeedStudio](docs/images/seeed_logo_2018_horizontal.png)](https://www.seeedstudio.com/nRF52832-Micro-Development-Board-p-2992.html)

[![Tindie](docs/images/tindie-logo.png)](https://www.tindie.com/stores/Zelin/)

[![Taobao](docs/images/taobao-logo.png)](https://zaowubang.taobao.com/category-1347611684.htm?spm=a1z10.1-c-s.w4010-18605444712.9.39be796cJVdvk7&search=y&parentCatId=769990253&parentCatName=%CE%EF%C1%AA%CD%F8%D3%B2%BC%FE&catName=nRF52832-MDK#bd)

## License

**MIT License**

Copyright (c) 2019 [makerdiary.com](https://makerdiary.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
