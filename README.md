# nRF52832-MDK

*An Open Source, Micro Development Kit for IoT Embedded Applications*

[中文文档](https://wiki.makerdiary.co/nrf52832-mdk)

[Buy Now!](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CTFAMGTNSZGGC)

**nRF52832-MDK** is a fully open-source development kit based on [Nordic nRF52832](http://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF52832) for IoT embedded applications. 

It also includes an integrated debug adapter -- [DAPLink](https://github.com/mbedmicro/DAPLink/), which provides methods to program and debug your applications. This enables developers to quickly establish a Bluetooth connection and evaluate nRF52832 SoC.


![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-boardbox.jpg)

## Hardware Features

nRF52832-MDK provides the following hardware features:

* Nordic Semiconductor nRF52832 SoC
    * ARM® Cortex™-M4F
    * 512kB Flash Memory + 64kB RAM
    * Bluetooth low energy
    * NFC™-A tag

* On-board debug probe DAPLink
    * MSC - drag-n-drop programming flash memory
    * CDC - virtual com port for log, trace and terminal emulation
    * HID - CMSIS-DAP compliant debug channel

* Up to 24 GPIOs

* Reset Button

* RGB LED

* On board chip antenna

* 3.3V working voltage

* 48mm x 18mm, Breadboard-friendly


## Pinout

![](https://img.makerdiary.co/wiki/nrf52832mdk/nRF52832-PINOUT.png)


## Programming & Debugging

nRF52832-MDK can be flashed via [DAPLink](https://github.com/mbedmicro/DAPLink),
which provides methods to program and debug an application running on ARM processors via SWD. 

Using the CMSIS-DAP interface, the board can be flashed via the USB storage
interface (drag-and-drop) and also via [pyOCD](https://github.com/mbedmicro/pyOCD).


### Installing pyOCD


The latest stable version of pyOCD may be done via pip as follows:

```
   
   $ pip install --pre -U pyocd
```


To install the latest development version (master branch), you can do the following:


```
   
   $ pip install --pre -U https://github.com/mbedmicro/pyOCD/archive/master.zip
```

Note that you may run into permissions issues running these commands. You have a few options here:

  1. Run with :code:`sudo -H` to install pyOCD and dependencies globally

  2. Specify the :code:`--user` option to install local to your user

  3. Run the command in a [virtualenv](https://virtualenv.pypa.io/en/latest/) local to a specific project working set.

You can also install from source by cloning the git repository and running


```

   $ python setup.py install
```


You can then verify that your board is detected by pyOCD by running:

```

   $ pyocd-flashtool -l

```


### Debugging with GDB


To debug your applications with GDB launch the GDB server on a terminal:

```

   $ pyocd-gdbserver

```

and then launch GDB against the .elf file you built:


```

   $ arm-none-eabi-gdb your_app.elf
```

And finally connect GDB to the GDB Server:

```

   (gdb) target remote localhost:3333
```

## Resource

* [nRF52832-MDK Schematic](/docs/hardware/nRF52832-MDK_SCH_V1.0.pdf)
* [nRF52832-MDK PCB](/docs/hardware/nRF52832-MDK_PCB_V1.0.pdf)
* [nRF52832-MDK 3D STEP](/docs/hardware/nRF52832-MDK_3D_STEP.step)

## License

**MIT License**

Copyright (c) 2017 [makerdiary.co](https://makerdiary.co)

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
