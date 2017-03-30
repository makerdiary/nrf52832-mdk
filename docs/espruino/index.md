# Espruino<br><small>A JavaScript interpreter for microcontrollers</small>

<a href="https://www.espruino.com/"><button data-md-color-primary="indigo">Espruino 官方网站</button></a>

## 简介

[Espruino](https://www.espruino.com/) 是一个非常轻量的 JavaScript 解析器，可以运行在 128KB Flash & 8KB RAM 的微控制器 (MCU) 上。

Espruino 是一个开源项目，使用 MPL-2.0 协议开源，其设计者是来自英国的 Gordon Williams。Gordon Williams 还为 Espruino 开发了很多周边硬件，感兴趣的朋友可以前往官网了解。

## 关键特性
相比于其他 JavaScript 解析器，Espruino 具有以下特点：

* **轻量级** —— 可以运行在 128KB Flash & 8KB RAM 的微控制器 (MCU) 上；

* **事件驱动** —— 与 node.js 类似，Espruino 采用事件驱动机制，支持异步函数回调和异步 I/O；

* **简单易用** —— Espruino 提供丰富的外设库，对硬件层做了很好的抽象，只要会写 JavaScript 就能用它来控制硬件；

* **跨平台开发** —— Espruino 提供一个基于 Chrome 浏览的 Web IDE，可以进行跨平台开发；

* **无线调试** —— Espruino Web IDE 使用 Web Bluetooth API，可以提供无线下载调试 JavaScript 代码功能。

## 硬件支持
除了支持 Espruino 官方发布的硬件外，Espruino 也可移植到其他硬件上，目前支持的有 STM32系列，ESP8266 & ESP32，nRF5x系列等。

nRF52832-MDK 只需在官方开发板的基础上，修改一下配置文件便可轻松支持：

<a href="https://github.com/makerdiary/Espruino"><button data-md-color-primary="indigo">Espruino for nRF52832-MDK</button></a>

!!! note "提示"
    `master` 主分支为 Espruino 官方的源码，`nrf52832-mdk` 分支包含支持 nRF52832-MDK 的代码。

## 推荐资源

* [Espruino 官方网站](https://www.espruino.com/)
* [Espruino 源码仓库](https://github.com/espruino/Espruino)
* [支持 nRF52832-MDK 的仓库](https://github.com/makerdiary/Espruino/tree/nrf52832-mdk)

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>






