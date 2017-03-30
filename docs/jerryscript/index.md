# JerryScript<br><small>A JavaScript engine for <strong>Internet of Things</strong></small>
![](https://img.makerdiary.co/wiki/nrf52832mdk/jerryscript-logo.png)

<a href="http://jerryscript.net/"><button data-md-color-primary="indigo">JerryScript 官方网站</button></a>

## 简介
JerryScript 是三星开源的一个非常轻量的 JavaScript 引擎，主要面向资源受限的物联网设备，可在 RAM<64KB/ROM<200KB 的微控制器上运行。

## 优势

JavaScript 是现在非常流行的编程语言之一，大量前端开发者对它非常熟悉，将物联网设备和 web 生态系统（web ecosystem）相互配合，对于构建物联网生态系统来说是个明智的选择，这也正是三星开发 JerryScript 的初衷。

> 另一方面，JavaScript 在嵌入式设备开发中是非常便利的。它支持异步函数回调和异步 I/O，这对基于事件驱动的硬件编程是非常有用的。

## 关键特性

从官方提供的文档，可以了解到，JerryScript 具备以下关键特性：

* 完全兼容 ECMAScript 5.1
* 若采用 ARM Thumb-2 指令集编译，仅需 160KB 空间
* 注重内存优化，减少内存消耗
* 使用 C99 编写，可移植性强
* 为预编译 JavaScript 源码提供快照支持
* 提供成熟的 C API，调用方便

更多内容可查阅：[JerryScript Wiki](https://github.com/jerryscript-project/jerryscript/wiki).

## 硬件支持

在广大社区开发者的努力下，JerryScript 支持的平台逐渐多了起来，能够支持的硬件也将越来越多。

这里要特别提到的是 JerryScript 已经可以支持 mbed OS 5 和 Zephyr，因此只要在硬件资源满足的条件下，理论上这两个 RTOS 支持的硬件是可以跑 JerryScript 的，这也充分体现了 JerryScript 的可移植性。

接下来的文章，我计划把 JerryScript 带到 nRF52832-MDK 上来，让 JS 开发者体验一下物联网开发，也让嵌入式开发者玩一玩 JavaScript。


## 推荐资源

* [JerryScript 官网](http://jerryscript.net)
* [jerryscript-project/jerryscript](https://github.com/jerryscript-project/jerryscript)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

