/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "stats_report.h"

DigitalOut led1(LED1);

// main() runs in its own thread in the OS
int main()
{
    SystemReport sys_state(500 /* Loop delay time in ms */);

    while (true) {
        // Blink LED and wait 0.5 seconds
        led1 = !led1;
        wait(0.5f);

        // Following the main thread wait, report on the current system status
        sys_state.report_state();
    }
}
