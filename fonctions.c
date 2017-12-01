#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include "fonctions.h"

#define ID_PRODUCT 0x2003

libusb_device_handle* handle;

int get_usb_device(libusb_context* context, struct libusb_device_descriptor* desc){ //Retourne le pointeur vers le device identifié par ID_PRODUCT
	libusb_device **list;
	ssize_t count = libusb_get_device_list(context,&list);
	if(count<0){
		perror("libusb_get_device_list");
		exit(-1);
	}
	ssize_t i = 0;
	for(i=0;i<count;i++){
		libusb_device* device = list[i];
		int status = libusb_get_device_descriptor(device,desc);
		if(status!=0) continue;
		if(desc->idProduct == ID_PRODUCT)
		{
			libusb_open(device,&handle);
			return 0;
		}
	}
	return 1;
}


int main(char argc, char **argv){
	libusb_context* context;
	if(libusb_init(&context)!=0){
		perror("libusb_get_device_list");
		exit(-1);
	}
	struct libusb_device_descriptor desc;
	get_usb_device(context,&desc); //TODO : GERER LE IF
	libusb_device* my_device = libusb_get_device(handle);
	uint8_t bus = libusb_get_bus_number(my_device);
	uint8_t address = libusb_get_device_address(my_device);
	printf("Device found @ (Bus:Adress) %d:%d\n",bus, address);
	printf("Vendor ID 0x0%x\n", desc.idVendor);
	printf("Product ID 0x0%x\n", desc.idProduct);
	return 0;
}


