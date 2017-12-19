#ifndef FONCTIONS_H
#define FONCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define ID_PRODUCT 0x1234
#define NUM_CONFIG 0

extern libusb_device_handle * handle;
extern struct libusb_endpoint_descriptor endp_list[10];

uint8_t init_usb(void);
int get_usb_device(libusb_context*);
void unclaim_active_config(libusb_device*);
void save_interrupt_endpoints(void);
uint8_t key_pressed(void);
void usb_send(uint8_t);

#endif
