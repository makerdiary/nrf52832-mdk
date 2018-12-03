/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

/* Number of possible I2C addresses. */
#define I2C_ADDRESSES      255

DigitalOut led1(LED1);

I2C i2c(I2C_SDA0, I2C_SCL0); 


// main() runs in its own thread in the OS
int main()
{
    int address;
    char sample_data;
    bool detected_device = false;

    int rslt;

    for (address = 1; address <= I2C_ADDRESSES; address++)
    {
    	rslt = i2c.read(address, &sample_data, sizeof(sample_data));

    	if(rslt == 0) //0 on success (ack)
    	{
    		detected_device = true;
    		printf("I2C device detected at address 0x%x.\r\n", address);
    	}
    }
    if (!detected_device)
    {
    	printf("No device was found.\r\n");
    }

    while (true) {
        // Blink LED and wait 0.5 seconds
        led1 = !led1;
        wait(0.5f);
    }
}
