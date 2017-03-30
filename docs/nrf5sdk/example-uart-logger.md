# 串口调试：Hello World !

<a href="https://github.com/makerdiary/nrf52832-mdk/tree/master/examples/nrf5-sdk/hello-world"><button data-md-color-primary="indigo">查看本例源码</button></a>


## 简介
在调试软件时，经常需要输出调试信息，这时我们可以简单选择串口来输出调试信息。nRF5 SDK 已经实现了非常棒的 Logger 模块，我们使用该模块来快速输出调试信息。

## 硬件支持

nRF52832-MDK 集成了 DAPLink 调试器，该调试上拥有一个 CDC 接口，可以用来实现 USB-to-UART 功能：

![](https://img.makerdiary.co/wiki/nrf52832mdk/DAPLink-Block-Diagram.png)

与 nRF52832 的连接如下：

* TX <--> P20
* RX <--> P19

!!! warning "注意"
    该串口不具备硬件流控制功能，因此不需要设置 `RTS` 和 `CTS` 引脚

## 串口工具
可以使用的串口工具很多，常用的有：

* screen - 这是一个命令工具，OSX 和 Linux 都支持得非常好，这也是我常用的工具
* [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html) - Windows 平台下非常好用的 SSH & telnet 工具，支持串口
* Minicom - Linux 平台比较常用串口工具

具体的使用方法可以参考前面介绍过的：[配置串口](/getting-started/hello-world/#_4)

## 示例代码

``` c
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    // Initialize.
    ret_code_t err_code;
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Starting... \r\n");

    /* Print Logs. */
    while (true)
    {
        NRF_LOG_INFO("Hello World !\r\n");
        NRF_LOG_WARNING("Hello World !\r\n");
        NRF_LOG_ERROR("Hello World !\r\n");
        NRF_LOG_FLUSH();
        nrf_delay_ms(1000);
    }
}

/**
 *@}
 **/
```

为了能够使用 Logger 模块，需要在 `sdk_config.h` 文件中使能相关功能：

``` c
// sdk_config.h
// <h> nRF_Log 

//==========================================================
// <e> NRF_LOG_ENABLED - nrf_log - Logging
//==========================================================
#ifndef NRF_LOG_ENABLED
#define NRF_LOG_ENABLED 1
#endif

...

// 配置串口

#ifndef NRF_LOG_BACKEND_SERIAL_UART_BAUDRATE
#define NRF_LOG_BACKEND_SERIAL_UART_BAUDRATE 30801920
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_TX_PIN - UART TX pin 
#ifndef NRF_LOG_BACKEND_SERIAL_UART_TX_PIN
#define NRF_LOG_BACKEND_SERIAL_UART_TX_PIN 20
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_RX_PIN - UART RX pin 
#ifndef NRF_LOG_BACKEND_SERIAL_UART_RX_PIN
#define NRF_LOG_BACKEND_SERIAL_UART_RX_PIN 19
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_RTS_PIN - UART RTS pin 
#ifndef NRF_LOG_BACKEND_SERIAL_UART_RTS_PIN
#define NRF_LOG_BACKEND_SERIAL_UART_RTS_PIN 0xFFFFFFFF
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_CTS_PIN - UART CTS pin 
#ifndef NRF_LOG_BACKEND_SERIAL_UART_CTS_PIN
#define NRF_LOG_BACKEND_SERIAL_UART_CTS_PIN 0xFFFFFFFF
#endif

// <o> NRF_LOG_BACKEND_SERIAL_UART_FLOW_CONTROL  - Hardware Flow Control
 
// <0=> Disabled 
// <1=> Enabled 

#ifndef NRF_LOG_BACKEND_SERIAL_UART_FLOW_CONTROL
#define NRF_LOG_BACKEND_SERIAL_UART_FLOW_CONTROL 0
#endif
```

## 编译运行

``` sh
$ make flash
```

![](https://img.makerdiary.co/wiki/nrf52832mdk/hello-world-logger.png)

至此，相信你已经学会了串口调试的技能，你可以用这个例子作为模板，嵌入到自己的应用中进行调试。

## 问题反馈

如果在开发过程遇到任何问题，可以通过 [GitHub Issue](https://github.com/makerdiary/nrf52832-mdk/issues) 反馈。

<a href="https://github.com/makerdiary/nrf52832-mdk/issues/new"><button data-md-color-primary="green">New Issue</button></a>

