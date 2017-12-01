#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include "fonctions.h"

libusb_device_handle * handle;


int main(void)
{

	libusb_context* context;
	if(libusb_init(&context)!=0){
		perror("libusb_get_device_list");
		exit(-1);
	}

	struct libusb_device_descriptor desc;
	if(get_usb_device(context,&desc)){perror("Device not found");exit(-1);}
	libusb_device* my_device = libusb_get_device(handle);
	uint8_t bus = libusb_get_bus_number(my_device);
	uint8_t address = libusb_get_device_address(my_device);
	printf("Device found @ (Bus:Adress) %d:%d\n",bus, address);
	printf("Vendor ID 0x0%x\n", desc.idVendor);
	printf("Product ID 0x0%x\n", desc.idProduct);

	
	/* Détacher le kernel de la configuration active */

	unclaim_active_config(my_device);


/*

	libusb_get_config_descriptor(my_device,0,&config);
	num_config = config->bConfigurationValue;
	if(libusb_set_configuration(handle, num_config)){perror("libusb_set_configuration"); exit(-1);}


	printf("\nTaille de la config : %d\n",config->bLength);
	printf("Consommation : %d\n",config->MaxPower);
	printf("\n##### Interfaces : %d #####\n\n",config->bNumInterfaces);
	num_config = config->bConfigurationValue;
	k = 0;
	


	for(i=0; i<config->bNumInterfaces; i++)
	{
		interf = (config->interface)[i];
		interd = (interf.altsetting)[0];
		printf("\t* Numero de l'interface : %d\n",interd.bInterfaceNumber);
		
		printf("\t\t+ Nombre de endpoints : %d\n",interd.bNumEndpoints);
		for(k=0; k<interd.bNumEndpoints; k++)
		{
			endp = (interd.endpoint)[k];
			printf("\t\t\t- Attributes : %d\n",endp.bmAttributes);
		}		
		
		
		if(libusb_claim_interface(handle,interd.bInterfaceNumber))
		{perror("Claim error"); exit(-1);}
		
				
	}


	for(i=0; i<config->bNumInterfaces; i++)
	{		
		interf = (config->interface)[i];
		interd = (interf.altsetting)[0];
		libusb_release_interface(handle,interd.bInterfaceNumber);		

	}

	//libusb_attach_kernel_driver(handle,0);
	//libusb_attach_kernel_driver(handle,1);

*/
	libusb_close(handle);
	libusb_exit(context);


	return 0;
}

