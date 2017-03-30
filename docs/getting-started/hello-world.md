## 开箱
![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-package-preview.png)
![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-package-open.png)

## 文档资源
nRF52832-MDK 的全部资料（包括硬件设计资料、源代码、应用示例以及文档）均托管在 [GitHub](https://github.com/makerdiary/nrf52832-mdk) 上，可以[点击下载](https://github.com/makerdiary/nrf52832-mdk/archive/master.zip)，或通过以下命令克隆到本地：

``` sh
$ git clone https://github.com/makerdiary/nrf52832-mdk
```

## Hello World !

nRF52832-MDK 预置了出厂固件，该固件的主要功能如下：

* 绿色 LED 闪烁指示低功耗蓝牙正在广播；
* DAPLink 上的串口以 `115200bps` 波特率输出 "Hello World!"；
* 低功耗蓝牙使用 [Eddystone](https://github.com/google/eddystone) 协议发出 URL `https://makerdiary.co`，可以使用终端设备获取该信息。

!!! tip "提示"
    该 demo 使用 nRF5 SDK 开发，源代码位置是 `./examples/nrf5-sdk/factory-firmware`, 可以暂时不管具体的实现方法，后面的章节会陆续实现。其主要目的是帮助开发者初步了解硬件，并确认硬件功能正常。

## 第一步：上电
使用 Micro USB 数据线，将 nRF52832-MDK 连接至 PC，绿色电源指示灯将常亮，绿色 LED 闪烁：

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-hello-blinky.gif)

## 第二步：配置串口

### Mac OSX

OSX 用户可直接使用系统内置的串口工具 `screen`, 具体命令如下：

``` sh
# 在终端输入以下命令，其中串口设备名称 "usbmodem1422" 不同 PC 不一样
$ screen /dev/cu.usbmodem1422 115200
```

!!! tip "小技巧"
    在终端先输入 `screen /dev/cu.usb`，然后按下 `tab` 键，自动补齐串口设备名称。
    退出 `screen` 可使用快捷键 `ctl+A` + `ctl+\`，然后根据提示输入`y`。

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf528320-mdk-hello-osx.png)

### Linux

在 Linux 环境下同样可以使用 `screen` 工具，这里以 Ubuntu 16.04 为例：

``` sh
# 安装 screen
$ sudo apt-get install screen

# 在终端输入以下命令（可能需要权限），设备名称可能会不一样：
$ sudo screen /dev/ttyACM0 115200

```

!!! note "小技巧"
    在 Ubuntu，退出 `screen` 可使用快捷键 `ctl+A` + `\`，然后根据提示输入`y`。

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-hello-linux.png)


### Windows

在 Windows 环境下需要安装串口驱动，[点击下载](https://developer.mbed.org/media/downloads/drivers/mbedWinSerial_16466.exe) 并安装驱动。

!!! tip "提示"
    如果是 Windows 10，可以选择下载驱动安装或让系统自动安装。

串口工具可以使用免费的 [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)，相关设置如下，串口号请在 `设备管理器` 中查看：

![](https://img.makerdiary.co/wiki/nrf52832mdk/putty-setting.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-hello-win.png)


## 第三步：体验低功耗蓝牙

nRF52832-MDK 出厂已经配置成了 Eddystone Beacon 设备。[Eddystone](https://github.com/google/eddystone) 是 Google 开源的 Beacon 协议，这是比 Apple iBeacon 更强大的存在，可以兼容 Android 以及 iOS 设备，更多细节可以参考[官方仓库](https://github.com/google/eddystone)，这里只演示一下应用：

下载 Nordic 的 **nRF Connect** 应用：

* [iOS](https://itunes.apple.com/us/app/nrf-connect/id1054362403?ls=1&mt=8)
* [Android](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp)
* [桌面版](https://www.nordicsemi.com/eng/Products/Bluetooth-low-energy/nRF-Connect-for-desktop#Downloads) (需要你的电脑支持 BLE)

以 iOS 为例，打开 **nRF Connect** App，将扫描到一个低功耗蓝牙设备，可查看设备的相关信息，会发现 URL `https://makerdiary.co` 以 Eddystone 协议规定的方式包含在广播信息中了：

![](https://img.makerdiary.co/wiki/nrf52832mdk/hello-world-eddystone-ios.png)


如果感兴趣的话，可以用来体验一下 Google 的 Physical Web 技术，相关设置如下：

* [Setting up Physical Web in Chrome](https://google.github.io/physical-web/try-physical-web)

!!! tip "提示"
    Google 的东西，你懂的，翻墙是必备技能~

最终你会看到，Chrome 浏览器的 Physical Web 已经发现了该设备，并显示广播信息中的链接：

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-physical-web.png)

## 总结

通过这个出厂 Demo，相信你已经对 nRF52832-MDK 有了基本的了解，接着将讲解"如何下载固件"，之后你便可以选择你感兴趣的技术学习开发自己的应用了。

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

