#ifndef FONCTIONS_H
#define FONCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define ID_PRODUCT 0xc226
extern libusb_device_handle * handle;

int get_usb_device(libusb_context*, struct libusb_device_descriptor*);
void unclaim_active_config(libusb_device*);

#endif
