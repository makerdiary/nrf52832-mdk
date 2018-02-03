## How to Update DAPLINK Firmware

To update the firmware on a device hold the reset button while attaching USB. The device will boot into bootloader mode. From there remove `firmware.bin` and then copy `lpc11u35_nrf52832_mdk_if_crc_xxxx.bin` onto the drive. Reconnect the board, if successful the device will leave bootloader mode and start running the new firmware.