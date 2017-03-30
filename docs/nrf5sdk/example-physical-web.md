# Physical Web <br><small>Walk up and use anything</small>

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/nrf5-sdk/ble_app_eddystone"><button data-md-color-primary="indigo">查看本例源码</button></a>


## Physical Web 是什么？

Physical Web 中文名为 “实物网”，是 Google 在 2015年推出的一个开源项目，旨在为物理世界的每个物体赋予 "URL", 让你轻松获取每个物体的信息，并与之进行交互，正如它的 Slogan 所表述的那样：

> Walk up and use anything.

![](https://img.makerdiary.co/wiki/nrf52832mdk/physical-web.jpg)

来看看 Scott Jenson 介绍 Physical Web 的视频：

<video src="https://img.makerdiary.co/20161202/Introduction%20to%20the%20Physical%20Web.mp4" controls="controls" width=100% poster="https://img.makerdiary.co/20161202/google-physical-web.jpg">
        您的浏览器不支持 video 标签。
</video> 

## 工作原理
Physical Web 也是 Beacon 技术的一种应用，它采用的是 Google 开源的 Beacon 协议：[**Eddystone**](https://github.com/google/eddystone)，更具体的讲就是 **Eddystone-URL** 。

与 iBeacon 类似，Eddystone-URL 也是基于低功耗蓝牙的通信协议。不同的是，iBeacon 发送的是 UUID + Mojor + Minor，Eddystone-URL 发送的则是简单明了的 URL 信息。以下是 Eddystone-URL 的规定：

| Byte offset | Field       | Description                |
| ----------- | ----------- | -------------------------- |
| 0           | Frame Type  | Value = 0x10               |
| 1           | TX Power    | Calibrated Tx power at 0 m |
| 2           | URL Scheme  | Encoded Scheme Prefix      |
| 3+          | Encoded URL | Length 1-17                |

URL 前缀规定：

| Decimal |	Hex  | Expansion    |
| ------- | ---- | ------------ |
| 0	      | 0x00 | http://www.  |
| 1	      | 0x01 | https://www. |
| 2	      | 0x02 | http://      |
| 3	      | 0x03 | https://     |

!!! warning "注意"
    要让 Chrome 识别该 URL，必须采用 `https://` 格式。

更多关于协议的信息可以参考：[https://github.com/google/eddystone](https://github.com/google/eddystone)

## 示例代码

这里我们让 nRF52832-MDK 以 Eddystone-URL 规定的方式广播 `https://makerdiary.co`，相关的修改如下：

``` c
// ./nrf52832-mdk/examples/ble_app_eddystone/inc/es_app_config.h

// Eddystone URL data
#define APP_ES_URL_FRAME_TYPE    ES_FRAME_TYPE_URL             // URL Frame type.
#define APP_ES_URL_SCHEME        0x03                          // 0x03 = "https://"
#define APP_ES_URL_URL           'm', 'a', 'k', 'e', 'r', \
                                 'd', 'i', 'a', 'r', 'y', \
                                 '.', 'c', 'o'                 // "makerdiary.co". 

```

将程序编译下载到 nRF52832-MDK 上：

``` sh
# 需要先下载蓝牙协议栈 softdevice
$ make flash_softdevice

# 再下载 ble_app_beacon
$ make flash

```

打开 **nRF Connect** App，将扫描到名称为 `nRF52832-MDK` 的设备，可查看设备的相关信息，会发现 URL `https://makerdiary.co` 已经包含在广播信息中了：

![](https://img.makerdiary.co/wiki/nrf52832mdk/hello-world-eddystone-ios.png)


## Chrome 配置

Physical Web 只能在 Chrome 浏览器中运行，需要在你的设备上安装 Chrome 浏览器。然后可以参考以下文档进行配置：

* [Setting up Physical Web in Chrome](https://google.github.io/physical-web/try-physical-web)

以 iOS 设备为例，配置成功后，手机就可以收到 nRF52832-MDK 发出的 URL：

![](https://img.makerdiary.co/wiki/nrf52832mdk/nrf52832-mdk-physical-web.png)

!!! tip "提示"
    使用 Google 的东西，翻墙是必须的~

## 推荐资源

* [Eddystone 协议规范](https://github.com/google/eddystone)
* [Physical Web 官网](https://google.github.io/physical-web/)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

