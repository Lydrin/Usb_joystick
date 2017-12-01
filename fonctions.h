#ifndef FONCTIONS_H
#define FONCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

int get_usb_device(libusb_context*, struct libusb_device_descriptor*);

#endif
