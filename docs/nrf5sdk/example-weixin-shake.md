# 微信摇一摇周边示例

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/nrf5-sdk/ble_app_beacon"><button data-md-color-primary="indigo">查看本例源码</button></a>

## 简介
摇一摇周边是微信在线下的全新功能, 为线下商户提供近距离连接用户的能力, 并支持线下商户向周边用户提供个性化营销、互动及信息推荐等服务。该服务主要基于 Apple 的 iBeacon 技术，而 iBeacon 则是利用低功耗蓝牙的广播包来发送信息，因此，我们可以使用 nRF52832-MDK 来演示一下这种应用。

更多关于摇一摇周边的内容，可以参考[摇一摇周边官网](https://zb.weixin.qq.com/)。

## iBeacon

iBeacon 是 Apple 在低功耗蓝牙技术基础上开发的通信协议，开发者可以利用该技术创建具备“位置上下文”的应用。iBeacon 技术自 2013 年推出至今，已经在很多商超零售、餐饮、公共场所等得到了广泛的应用。

这里只做简单应用介绍，前往苹果开发者网站了解更多关于 iBeacon 的内容：[iBeacon for Developers](https://developer.apple.com/ibeacon/).

iBeacon 设备的广播包主要包含：iBeacon ID 以及表示信号强度的信息，移动设备根据这些信息就知道自己正在接收哪个设备的信息，并且可以判断距离该设备有多近。

iBeacon ID 长度 20 个字节，被分成：UUID(16 字节), Major(2 字节), Minor(2 字节)，这20个字节便可确保设备的唯一性。下面的表格就是一个简单的例子：

![](https://img.makerdiary.co/wiki/nrf52832mdk/ibeacon-example.png)

## 摇一摇周边配置

要使用微信摇一摇周边，需要一个通过微信认证的公众号，通过添加 **摇一摇周边** 功能插件即可实现：

![](https://img.makerdiary.co/wiki/nrf52832mdk/zb_weixin_config.png)

添加设备申请后，微信会提供 **UUID**, **Major**, **Minor** 等信息，同时你也可以为该设备绑定一个页面。我在公众号添加了一个测试设备，以此为例进行说明：

| iBeacon ID | Value |
| ---------- | ----- |
| **UUID**   | FDA50693-A4E2-4FB1-AFCF-C6EB07647825 |
| **Major**  | 10100 |
| **Minor**  | 25137 |

## 示例代码

本例代码在 nRF5 SDK 的示例 `ble_app_beacon` 基础上修改，主要配置一下 iBeacon 的信息即可：

``` c
//./nrf52832-mdk/examples/nrf5-sdk/ble_app_beacon/src/main.c
...

#define APP_MEASURED_RSSI       0xC3 
#define APP_COMPANY_IDENTIFIER  0x004C      // 0x004C 为 Apple 的标识符，这里只做测试，商用需要去申请                             
#define APP_MAJOR_VALUE         0x27, 0x74  // 即 10100
#define APP_MINOR_VALUE         0x62, 0x31  // 即 25137
#define APP_BEACON_UUID         0xFD, 0xA5, 0x06, 0x93, \
                                0xA4, 0xE2, 0x4F, 0xB1, \
                                0xAF, 0xCF, 0xC6, 0xEB, \
                                0x07, 0x64, 0x78, 0x25            
...

```

将程序编译下载到 nRF52832-MDK 上：

``` sh
# 需要先下载蓝牙协议栈 softdevice
$ make flash_softdevice

# 再下载 ble_app_beacon
$ make flash

```

## 摇一摇

程序正常运行后就会一直在广播信息，打开微信 --> 发现 --> 摇一摇，出现 **周边** 图标就表明已经发现 iBeacon 设备了，摇一摇，你配置的页面就出来了：

![](https://img.makerdiary.co/wiki/nrf52832mdk/weixin_zb_icon.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/zh_shake_result.png)

## 推荐资源

* 一家把 Beacon 技术玩到极致的公司：[Estimote](http://developer.estimote.com/)
* [Apple iBeacon for Developers](https://developer.apple.com/ibeacon/)
* [摇一摇周边官网](https://zb.weixin.qq.com/)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

