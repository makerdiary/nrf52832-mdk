# Espruino 快速入门指南

本小节主要介绍如何使用 nRF52832-MDK 快速玩转 Espruino，简单体验一下使用 JavaScript 进行嵌入式应用开发。

## 开发环境

这里我们可以使用 Espruino Web IDE 进行开发：

### 安装 Chrome 浏览器

请确保你的电脑已经安装好 [Chrome 浏览器](http://www.google.cn/chrome/browser/desktop/index.html)，最新版的 Chrome 浏览器已经支持 Web Bluetooth API，如果你想体验无线下载的话，需要升级到最新版；

![](https://img.makerdiary.co/wiki/nrf52832mdk/espruino-chrome.png)

### 安装 Espruino Web IDE
点击以下按钮安装 Espruino Web IDE，这是 Chrome 的扩展程序，可用于编辑、下载 JavaScript 代码。

<a href="https://chrome.google.com/webstore/detail/espruino-web-ide/bleoifhkdalbjfbobjackfdifdneehpo"><button data-md-color-primary="indigo">Espruino Web IDE</button></a>

![](https://img.makerdiary.co/wiki/nrf52832mdk/installing-espruino-web-ide.png)

## 下载固件

这里为了能够快速使用，我为 nRF52832-MDK 预编译了最新版的固件：

<a href="/nrf52832-mdk/espruino/espruino_1v91.2835_nrf52832_mdk.hex"><button data-md-color-primary="indigo">获取最新固件</button></a>

nRF52832-MDK 连接到 PC，将下载好的 `hex` 文件拷贝到 **DAPLINK** 可移动磁盘，按下复位键运行。

![](https://img.makerdiary.co/wiki/nrf52832mdk/espruino-latest-firmware.png)

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./espruino_1v91.2835_nrf52832_mdk.hex

```

![](https://img.makerdiary.co/wiki/nrf52832mdk/espruino-pyocd-download.png)

## 体验 JavaScript

[启动 Espruino Web IDE](https://chrome.google.com/webstore/detail/espruino-web-ide/bleoifhkdalbjfbobjackfdifdneehpo)，通过 Web Bluetooth 或者 DAPLink 的串口连接 nRF52832-MDK：

![](https://img.makerdiary.co/wiki/nrf52832mdk/espruino-connecting-hardware.png)

连接后，你可以在其终端输入一些 JavaScript 语句，观察其执行情况：

``` javascript
> var test=0; for (t=100; t<1000; t++) test+=t; print ('Hi JS World! '+test);
Hi JS World! 494550
=undefined

/* 当然也可以复杂一点的 */
> function hello(t) {t=t*10;return t}; print("result"+hello(10.5));
result105
=undefined
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/espruino-js-repl.png)


也可以直接操控硬件，让 LED1 闪烁起来：

``` javascript

var  on = false;
setInterval(function() {
  on = !on;
  LED1.write(on);
}, 500);

```

![](https://img.makerdiary.co/wiki/nrf52832mdk/espruino_js_download.png)

![](https://img.makerdiary.co/wiki/nrf52832mdk/espruino-blinky.gif)

至此，你已经在 nRF52832-MDK 成功使用 Espruino，并简单体验了一下 JavaScript 操控嵌入式硬件的玩法。后面我将陆续开发一些有趣的示例，也非常欢迎你的参与。

Enjoy !

## 推荐资源

* [Espruino API 文档](http://www.espruino.com/Reference#software)
* [Espruino 源码仓库](https://github.com/espruino/Espruino)
* [支持 nRF52832-MDK 的仓库](https://github.com/makerdiary/Espruino/tree/nrf52832-mdk)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>



