#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include "fonctions.h"

libusb_device_handle * handle;
struct libusb_endpoint_descriptor endp_list[10];

int main(void)
{

	libusb_context* context;
	if(libusb_init(&context)!=0){
		perror("libusb_get_device_list");
		exit(-1);
	}

	if(get_usb_device(context)){perror("Device not found");exit(-1);}
	libusb_device* my_device = libusb_get_device(handle); // handle récupéré de get_usb_device

	/* Détacher le kernel de la configuration active */

	unclaim_active_config(my_device); //Inutile dans le cas de notre propre périphérique

	/* Utiliser la configuration NUM_CONFIG (0) */

	struct libusb_config_descriptor * config;
	libusb_get_config_descriptor(my_device,NUM_CONFIG,&config);
	uint8_t num_config = config->bConfigurationValue;
	if(libusb_set_configuration(handle, num_config)){perror("libusb_set_configuration"); exit(-1);}

	/* Sauvegarde des interruptions dans un tableau déclaré en global */

	save_interrupt_endpoints();
	
	
	/*
	uint8_t endpoint_in=endp_list[0].bEndpointAddress;    
	unsigned char data[8];    
	int size=8;         
	int timeout=50;        

	while(1){
		int bytes_in;
		libusb_interrupt_transfer(handle,endpoint_in,data,size,&bytes_in,timeout);
		//if(status!=0){ perror("libusb_interrupt_transfer"); exit(-1); }
		for(int i = 0;i<bytes_in;i++){
			printf("Received : %d\n",data[i]);
		}
	}	
	*/
	
	while(1){
		uint8_t key = key_pressed();
		if(key != 0){printf("Key : %d\n",key);}
	}
	

	libusb_close(handle);
	libusb_exit(context);

	return 0;
}

