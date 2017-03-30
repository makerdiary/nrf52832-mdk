nRF52832-MDK 板上集成了 [DAPLink](https://github.com/mbedmicro/DAPLink) 多功能仿真器，可以支持两种固件下载方式：[“拖放式”下载](#_1) 以及 [pyOCD 下载](#pyocd)。

## “拖放式”下载
将 nRF52832-MDK 连接到 PC，PC 自动将其识别成可移动磁盘设备，名称为：`DAPLINK`

![](https://img.makerdiary.co/wiki/nrf52832mdk/daplink_disk.png)

然后将 `hex` 或 `bin` 文件直接拷贝到该磁盘，板上绿灯闪烁表示正在下载，下载完成后磁盘会先弹出再重新挂载，如果有错误发生，磁盘内会产生提示错误信息的文件。

![](https://img.makerdiary.co/wiki/nrf52832mdk/daplink_download.png)

!!! warning "注意"
    使用“拖放”下载方式，下载完成后，程序不会自动运行，按下复位按钮或重新上电，程序才会运行！如果觉得不爽，请使用下面介绍的 pyOCD 下载方式。
    

## 使用 pyOCD 下载

[pyOCD](https://github.com/mbedmicro/pyOCD) 是一个开源、跨平台的 python 库，支持 Linux, OSX, Windows 系统，可对 ARM Cortex-M 系列 MCU 进行编程和调试，甚至可以利用其提供的 API 实现更复杂的功能。

### 安装
先确保系统已经安装好了 `Python 2.7.x`，建议使用 `Python 2.7.13` 或以上版本。

<a href="https://www.python.org/downloads/"><button data-md-color-primary="indigo">前往 Python 官网下载</button></a>

``` sh
$ python --version
# Python 2.7.13
$ pip --version
# pip 9.0.1
```

可以通过 `pip` 安装最新稳定版：

``` sh
$ pip install --pre -U pyocd
```

或者通过源码安装开发版本，体验正在开发中的功能：

``` sh
$ pip install --pre -U https://github.com/mbedmicro/pyOCD/archive/master.zip
```

!!! warning "注意"
    有的系统(例如 Ubuntu )可能会因为权限问题导致无法正常使用 pyOCD，出现这种情况，请在安装命令前增加 `sudo -H` 进行全局安装。

### 下载固件

准备好你的 hex 文件，或者下载 [nrf52832-mdk-hello-world.hex](nrf52832-mdk-hello-world.hex) 来练练手，相关命令如下，蓝色 LED 闪烁表示正在编程：

``` sh
# -se 参数表示按照所需 sector 进行擦除
$ pyocd-flashtool -d debug -t nrf52 -se your-hex-file.hex

# 如果想整片擦除，可以使用 -ce 参数
$ pyocd-flashtool -d debug -t nrf52 -ce your-hex-file.hex

# 如果只是想擦除，不编程，可以不带hex文件参数
$ pyocd-flashtool -d debug -t nrf52 -ce
```

!!! warning "注意"
    有的系统(例如 Ubuntu )可能会因为权限问题导致无法下载，请在命令增加 `sudo`。

![](https://img.makerdiary.co/wiki/nrf52832mdk/pyocd-flashtool.png)

## 更多内容
如果想了解更多关于 DAPLink 以及 pyOCD 的内容，可访问其官方仓库：

* [mbedmicro/DAPLink](https://github.com/mbedmicro/DAPLink)
* [mbedmicro/pyOCD](https://github.com/mbedmicro/pyOCD)

## 总结
至此，你应该已经对 nRF52832-MDK 的使用方式有了更进一步的了解，后面的章节会介绍一些能够使用 nRF52832-MDK 进行开发的技术，期待你的参与！

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

