## Installing the nRF5 SDK

Download the SDK package from [www.nordicsemi.com](https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK/).

The latest version is `15.2.0`, it can be downloaded directly here: [nRF5_SDK_v15.2.0_9412b96](https://www.nordicsemi.com/Software-and-Tools/Software/nRF5-SDK/Download#infotabs)

Extract the zip file into this directory. This should give you the following folder structure:

``` sh
./nrf52832-mdk/
├── LICENSE
├── README.md
├── firmware
├── config
├── docs
├── examples
├── mkdocs.yml
├── nrf_sdks
│   └── nRF5_SDK_v15.2.0_9412b96
└── tools
```

Config the toolchain path in `makefile.windows` or `makefile.posix` depending on platform you are using. That is, the `.posix` should be edited if your are working on either Linux or macOS. These files are located in:

``` sh
<nRF5 SDK>/components/toolchain/gcc
```

Open the file in a text editor and make sure that the `GNU_INSTALL_ROOT` variable is pointing to your GNU Arm Embedded Toolchain install directory. For example:

``` sh
GNU_INSTALL_ROOT ?= $(HOME)/gcc-arm-none-eabi/gcc-arm-none-eabi-6-2017-q2-update/bin/
GNU_VERSION ?= 6.3.1
GNU_PREFIX ?= arm-none-eabi
```
