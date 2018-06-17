# JerryScript<br><small>开源、面向小型嵌入式设备的 JavaScript 引擎</small>

[![](../../jerryscript/images/jerryscript-logo.png)](https://github.com/jerryscript-project/jerryscript)

## 简介

JerryScript 是三星开源的一个非常轻量的 JavaScript 引擎，主要面向资源受限的物联网设备，可在 小于 64KB RAM 64KB 和 200KB ROM 的微控制器上运行。

JavaScript 是现在非常流行的编程语言之一，大量前端开发者对它非常熟悉，将物联网设备和 web 生态系统相互配合，对于构建物联网生态系统来说是个明智的选择，这也正是三星开发 JerryScript 的初衷。

另一方面，JavaScript 在嵌入式设备开发中是非常便利的。它支持异步函数回调和异步 I/O，这对基于事件驱动的硬件编程是非常有用的。

JerryScript 具备以下关键特性：

* 完全兼容 ECMAScript 5.1
* 若采用 ARM Thumb-2 指令集编译，仅需 160KB 空间
* 注重内存优化，减少内存消耗
* 使用 C99 编写，可移植性强
* 为预编译 JavaScript 源码提供快照支持
* 提供成熟的 C API，调用方便

了解更多细节，可以访问 [JerryScript 官方仓库](https://github.com/jerryscript-project/jerryscript)。

## 快速入门

以下内容将介绍如何快速编译、运行 JerryScript。

### 搭建编译环境

编译 JerryScript 引擎之前，需要配置好开发环境，安装以下工具：

* `gcc` 或 其他 C99 编译器 (本地或交叉编译器，例如：arm-none-eabi)
* `cmake` >= `2.8.12.2`
* `bash` >= `4.3.11`
* `cppcheck` >= `1.61`
* `vera++` >= `1.2.1`
* `python` >= `2.7.6`

另外确保以下命令行工具可用：

* `awk`
* `bc`
* `find`
* `sed`

从官方仓库克隆 JerryScript 源码：

``` sh
$ git clone https://github.com/jerryscript-project/jerryscript.git
```

### 基于 Zephyr 编译 JerryScript

这里我们基于 Zephyr 来编译 JerryScript 引擎，你也可以基于 Arm Mbed OS 进行编译。

Zephyr 开发环境配置请参考：[搭建 Zephyr 开发环境](../zephyr)。

配置 Zephyr 环境变量：

``` sh
$ cd zephyr
$ source zephyr-env.sh
```

### 编译运行 JerryScript

接下来，你可以开始编译下载 JerryScript 引擎：

``` sh
$ cd jerryscript
$ make -f ./targets/zephyr/Makefile.zephyr BOARD=nrf52832_mdk flash
```

打开串口工具，JerryScript 将以 REPL 模式运行：

``` sh
***** BOOTING ZEPHYR OS v1.10.99 - BUILD: Mar  2 2018 22:19:04 *****
JerryScript build: Mar  2 2018 22:19:03
JerryScript API 1.0
Zephyr version 1.10.99
js>
```

尝试运行下面比较简单的脚本：

``` js
js> var test=0; for (t=100; t<1000; t++) test+=t; print ('Hi JS World! '+test);
Hi JS World! 494550
undefined
```

或者更复杂的：

``` js
js> function hello(t) {t=t*10;return t}; print("result"+hello(10.5));
result105
undefined
```

![](../../jerryscript/images/jerryscript_for_zephyr_repl.png)

!!! tip "提示"
	我们提供预编译好的固件 `jerryscript_v1.0_for_zephyr.hex`， 该固件位于 `./nrf52832-mdk/firmware/jerryscript/` 目录。

## 更多示例

JerryScript 引擎目前还没有直接支持 nRF5 SoC，相信在社区开发者的努力下，很快会完成这部分移植工作。

目前 Intel 基于 Zephyr 已经做大量移植工作，这个项目就是 [Zephyr.js](https://github.com/intel/zephyr.js)，对 Arduino 101 支持比较好，对于 nRF52 还有一些功能需要添加，你可以保持关注，更多功能会逐渐加入。

## 参考资源

* [JerryScript 官方仓库](https://github.com/jerryscript-project/jerryscript)
* [zephyr.js 官方仓库](https://github.com/intel/zephyr.js)


## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="marsala"><i class="fa fa-github"></i> 创建 Issue</button></a>

