#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include "fonctions.h"


int get_usb_device(libusb_context* context, struct libusb_device_descriptor* desc){ 
//Retourne le pointeur vers le device identifié par ID_PRODUCT
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
			if(libusb_open(device,&handle)){perror("Unable to open a device"); exit(-1);};
			libusb_free_device_list(list,1);
			return 0;
		}
	}
	libusb_free_device_list(list,1);
	return 1;
}


void unclaim_active_config(libusb_device* device){
//Détacher le kernel de la configuration active
	struct libusb_config_descriptor * config;
	libusb_get_active_config_descriptor(device,&config);

	uint8_t i = 0;
	
	struct libusb_interface interf;
	struct libusb_interface_descriptor interd;

	
	for(i=0; i<config->bNumInterfaces; i++)
	{
		interf = (config->interface)[i];
		interd = (interf.altsetting)[0];


		if(libusb_kernel_driver_active(handle,interd.bInterfaceNumber)){
			int status=libusb_detach_kernel_driver(handle,interd.bInterfaceNumber);
			if(status!=0){ perror("libusb_detach_kernel_driver"); exit(-1); }
		}
				
	}

}
