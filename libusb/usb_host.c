#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include "usb_host.h"

libusb_device_handle * handle;
struct libusb_endpoint_descriptor endp_list[10];

uint8_t init_usb(void)
{

	libusb_context* context;
	if(libusb_init(&context)!=0){
		perror("libusb_get_device_list");
		return 1;
	}

	if(get_usb_device(context)){perror("Device not found");return 1;}
	libusb_device* my_device = libusb_get_device(handle); // handle récupéré de get_usb_device

	/* Détacher le kernel de la configuration active */

	unclaim_active_config(my_device); //Inutile dans le cas de notre propre périphérique

	/* Utiliser la configuration NUM_CONFIG (0) */

	struct libusb_config_descriptor * config;
	libusb_get_config_descriptor(my_device,NUM_CONFIG,&config);
	uint8_t num_config = config->bConfigurationValue;
	if(libusb_set_configuration(handle, num_config)){perror("libusb_set_configuration"); return 1;}

	/* Sauvegarde des interruptions dans un tableau déclaré en global */

	save_interrupt_endpoints();
	
	return 0;
}

int get_usb_device(libusb_context* context){ 
//Modifie le handle (var. globale) du device identifié par ID_PRODUCT
	struct libusb_device_descriptor desc;
	libusb_device **list;
	ssize_t count = libusb_get_device_list(context,&list);
	if(count<0)
	{
		perror("libusb_get_device_list");
		exit(-1);
	}
	ssize_t i = 0;
	for(i=0;i<count;i++){
		libusb_device* device = list[i];
		int status = libusb_get_device_descriptor(device,&desc);
		if(status!=0) continue;
		if(desc.idProduct == ID_PRODUCT)
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

void save_interrupt_endpoints(void)
{
	uint8_t i,k;
	struct libusb_interface interf;
	struct libusb_interface_descriptor interd;
	struct libusb_endpoint_descriptor endp;
	uint8_t endpoint_type;
	uint8_t cpt = 0;
	uint8_t done;
	struct libusb_config_descriptor * config;
	
	libusb_device* my_device = libusb_get_device(handle);
	libusb_get_config_descriptor(my_device,NUM_CONFIG,&config);

	for(i=0; i<config->bNumInterfaces; i++)
	{
		done = 0;
		interf = (config->interface)[i];
		interd = (interf.altsetting)[0];

		for(k=0; k<interd.bNumEndpoints; k++)
		{
			endp = (interd.endpoint)[k];
			endpoint_type = endp.bmAttributes & 0x03;
			if(endpoint_type == 3)
			{
				endp_list[cpt] = endp;
				cpt++;
				done = 1;	
			}
			
			if(done == 1){break;}
		}		
		
		if(libusb_claim_interface(handle,interd.bInterfaceNumber))
		{perror("Claim error"); exit(-1);}
				
	}

}


uint8_t key_pressed(void)
{
	uint8_t endpoint_in = endp_list[0].bEndpointAddress;
	unsigned char data; 
	int size=8; 
	int timeout=120;
	int bytes_in;
	libusb_interrupt_transfer(handle,endpoint_in,&data,size,&bytes_in,timeout);
	if(bytes_in == 1){return data;}
	else{return 0;}
}

void usb_send(uint8_t msg)
{
	uint8_t data = msg;
	int endpoint_out=endp_list[1].bEndpointAddress;
	int size=8;
	int timeout=50;
	int bytes_out;
	libusb_interrupt_transfer(handle,endpoint_out,&data,size,&bytes_out,timeout);
}

