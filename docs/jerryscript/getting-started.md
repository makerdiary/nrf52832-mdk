# JerryScript 开发初体验

本小节主要通过一个运行在 nRF52832-MDK 的 REPL 应用，简单体验一下使用 JerryScript 进行嵌入式开发。

!!! note "提示"
    这里为了快速体验，暂不介绍如何编译固件，后续章节将会涉及到。

## 下载固件
在 [janjongboom/mbed-js-repl-example](https://github.com/janjongboom/mbed-js-repl-example) 的基础上，我为 nRF52832-MDK 预编译了这个基于 mbed OS 5 的 JavaScript REPL 应用：

<a href="/jerryscript/mbedos5-js-repl-example.hex"><button data-md-color-primary="indigo">获取最新固件</button></a>

nRF52832-MDK 连接到 PC，将下载好的 `hex` 文件拷贝到 **DAPLINK** 可移动磁盘，按下复位键运行。

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-js-repl-download.png)

或直接运行以下命令下载运行：

``` sh
$ pyocd-flashtool -d debug -t nrf52 -ce ./mbedos5-js-repl-example.hex

```

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-js-repl-pyocd.png)

## 体验 JavaScript
打开串口，进入 REPL 终端：

``` sh
$ screen /dev/cu.usbmodem1412 115200
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-jerry-repl.png)

你可以随便输入一些 JavaScript 语句，观察其执行情况，例如：

``` javascript
=undefined

> var test=0; for (t=100; t<1000; t++) test+=t; print ('Hi JS World! '+test);
Hi JS World! 494550
/* 也可以复杂一点的，定义个函数什么的 */
> function hello(t) {t=t*10;return t}; print("result"+hello(10.5));
result105
=undefined
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-jerry-exec-js.png)



当然，操作硬件才是我们最想要，这里让 LED1 可以闪烁起来：

!!! note "提示"
    为了代码可读性，这里做了格式化处理，实际运行是需要将 JS 代码进行压缩的。

``` javascript
var led = DigitalOut(LED1); 
var on = false; 

setInterval(function() { 
    on = !on; 
    led.write(on? 0 : 1)
}, 500);

```

![](https://img.makerdiary.co/wiki/nrf52832mdk/mbed-jerry-blinky.png)

至此，你已经在 nRF52832-MDK 体验了一把 JavaScript 的新玩法，后续我将陆续开发一些有趣的示例，也非常欢迎你的参与。

Enjoy !

## 推荐资源

* [JerryScript 官网](http://jerryscript.net)
* [jerryscript-project/jerryscript](https://github.com/jerryscript-project/jerryscript)
* [janjongboom/mbed-js-repl-example](https://github.com/janjongboom/mbed-js-repl-example)


## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

