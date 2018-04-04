#!/bin/sh
esptool.py --port /dev/cu.usbserial-A505WVU9 --baud 115200 write_flash --flash_size=detect 0 .pioenvs/nodemcuv2/firmware.bin