#!/bin/bash

make clean all
dfu-programmer atmega16u2 erase
dfu-programmer atmega16u2 flash pad.hex
dfu-programmer atmega16u2 reset

exit 0
