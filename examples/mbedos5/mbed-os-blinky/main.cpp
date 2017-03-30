#include "mbed.h"

DigitalOut led2(LED2);

// main() runs in its own thread in the OS
int main() {
    while (true) {
        led2 = !led2;
        wait(0.5);
    }
}

