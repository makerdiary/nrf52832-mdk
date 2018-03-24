# Getting started with Zephyr examples

## Checking out the repository

Clone the [Zephyr Project](https://github.com/zephyrproject-rtos/zephyr) repository on GitHub or download it as a zip package and put its contents to your working directory.

```
$ git clone https://github.com/zephyrproject-rtos/zephyr.git
```

## Setting up the development environment

The Zephyr project uses [CMake](https://cmake.org/) as a tool for managing the building of the project. CMake is able to generate build files in different formats (also known as “generators”), and the following ones are currently supported by Zephyr:

* `make`: Supported on UNIX-like platforms (Linux, macOS).
* `ninja`: Supported on all platforms.

The setup process for the development environment depends on your operating system. Please choose your host operating system. The setup instructions for each operating system walk you through how to set up the development environment.

### Windows

The easiest way to install the dependencies natively on Microsoft Windows is to use the Chocolatey package manager ([Chocolatey website](https://chocolatey.org/)). 

1. Install Chocolatey by following the instructions on the [Chocolatey install](https://chocolatey.org/install) website.

2. Open a Command Prompt (cmd.exe) as an **Administrator**.

3. Optionally disable global confirmation to avoid having to add -y to all commands:

	```
	$ choco feature enable -n allowGlobalConfirmation
	```

4. Install CMake:

	```
	$ choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System'
	```

5. Install the rest of the tools:

	```
	$ choco install git python ninja dtc-msys2 gperf
	```

6. Close the Command Prompt window.

7. Open a Command Prompt (cmd.exe) as a regular user.

8. Install the required Python modules:

	```
	$ cd ./zephyr
	$ pip install --user -r scripts/requirements.txt
	$ pip2 install --user -r scripts/py2-requirements.txt
	```

9. Download and install the [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) (install to `c:\gccarmemb`).

10. Within the Command Prompt, set up environment variables for the installed tools and for the Zephyr environment:

	```
	$ set ZEPHYR_TOOLCHAIN_VARIANT=gccarmemb
	$ set GCCARMEMB_TOOLCHAIN_PATH=c:\gccarmemb
	```

### macOS

First, install the *Homebrew* (The missing package manager for macOS). Homebrew is a free and open-source software package management system that simplifies the installation of software on Apple’s macOS operating system.

To install Homebrew, visit the [Homebrew site](http://brew.sh/) and follow the installation instructions on the site.

After Homebrew was successfully installed, install the following tools using the brew command line:

```
$ brew install cmake ninja dfu-util doxygen qemu dtc python3 gperf
$ curl -O 'https://bootstrap.pypa.io/get-pip.py'
$ ./get-pip.py
$ rm get-pip.py
$ cd ~/zephyr   # or to the folder where you cloned the zephyr repo
$ pip3 install --user -r scripts/requirements.txt
$ pip2 install --user -r scripts/py2-requirements.txt
```

Download and install the [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads). Then make sure to add the path to your toolchain to your OS PATH environment variable:

```
# add the following scripts to ~/.bash_profile 
export GCCARMEMB_TOOLCHAIN_PATH="<path to install directory>/gcc-arm-none-eabi-7-2017-q4-major"
export ZEPHYR_GCC_VARIANT=gccarmemb
```

### Linux

Before proceeding with the build, ensure your OS is up to date. On Ubuntu, you’ll first need to update the local database list of available packages before upgrading:

```
$ sudo apt-get update
$ sudo apt-get upgrade
```

Install the required packages in a Ubuntu host system with:

```
$ sudo apt-get install --no-install-recommends git cmake ninja-build gperf \
  ccache doxygen dfu-util device-tree-compiler \
  python3-ply python3-pip python3-setuptools xz-utils file make gcc-multilib \
  autoconf automake libtool
```

Install additional packages required for development with Zephyr:

```
$ cd ~/zephyr  # or to your directory where zephyr is cloned
$ pip3 install --user -r scripts/requirements.txt
$ pip2 install --user -r scripts/py2-requirements.txt
```

Download and install the [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads). Then make sure to add the path to your toolchain to your OS PATH environment variable:

```
# add the following scripts to ~/.bash_profile 
export GCCARMEMB_TOOLCHAIN_PATH="<path to install directory>/gcc-arm-none-eabi-7-2017-q4-major"
export ZEPHYR_GCC_VARIANT=gccarmemb
```

## Adding support for nRF52832-MDK

To add support for nRF52832-MDK, you must add the board support files. This should give you the following folder structure:

```
.zephyr/boards/arm/nrf52832_mdk
├── Kconfig
├── Kconfig.board
├── Kconfig.defconfig
├── board.cmake
├── board.h
├── nrf52832_mdk.dts
├── nrf52832_mdk.yaml
└── nrf52832_mdk_defconfig
```

You can checkout the patch from [makerdiary/zephyr](https://github.com/makerdiary/zephyr):

```
$ cd <zephyr git clone location>
$ git remote add makerdiary https://github.com/makerdiary/zephyr.git
$ git pull makerdiary master
```

## Compiling and running an example

Now you can try to build one of the examples. Will use the `hello_world` example here to keep it simple.

Source `zephyr-env.sh` wherever you have cloned the Zephyr Git repository:

```
$ unset ZEPHYR_SDK_INSTALL_DIR
$ cd <zephyr git clone location>
$ source zephyr-env.sh
```

Create a `build` folder in the example directory, where all artifacts generated by the Ninja build system are stored, such as:

```
$ cd nrf52832-mdk/examples/zephyr/hello_world/
$ mkdir build && cd build

build$ cmake -GNinja -DBOARD=nrf52832_mdk ..
build$ ninja
build$ ninja flash
```

![](../../docs/zephyr/images/hello_world_ninja_building.png)

Check the board output from serial port, you will see the following messages:

```
***** BOOTING ZEPHYR OS v1.10.99 - BUILD: Feb 26 2018 12:44:01 *****
Hello World! arm
```

That's it! You can also try other examples in the path `nrf52832-mdk/examples/zephyr/`.