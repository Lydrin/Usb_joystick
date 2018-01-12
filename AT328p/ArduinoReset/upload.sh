#!/bin/bash

dfu-programmer atmega16u2 erase
dfu-programmer atmega16u2 flash ArduinoUno.hex
dfu-programmer atmega16u2 reset

exit 0
