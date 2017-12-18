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

	struct libusb_device_descriptor desc;
	if(get_usb_device(context,&desc)){perror("Device not found");exit(-1);}
	libusb_device* my_device = libusb_get_device(handle);
	uint8_t bus = libusb_get_bus_number(my_device);
	uint8_t address = libusb_get_device_address(my_device);
	printf("Device found @ (Bus:Adress) %d:%d\n",bus, address);
	printf("Vendor ID 0x%x\n", desc.idVendor);
	printf("Product ID 0x%x\n", desc.idProduct);

	
	/* Détacher le kernel de la configuration active */

	unclaim_active_config(my_device);


	struct libusb_config_descriptor * config;
	libusb_get_config_descriptor(my_device,0,&config);
	uint8_t num_config = config->bConfigurationValue;
	if(libusb_set_configuration(handle, num_config)){perror("libusb_set_configuration"); exit(-1);}


	printf("\nTaille de la config : %d\n",config->bLength);
	printf("Consommation : %d\n",config->MaxPower);
	printf("\n##### Interfaces : %d #####\n\n",config->bNumInterfaces);
	//num_config = config->bConfigurationValue;
	uint8_t i,k;
	struct libusb_interface interf;
	struct libusb_interface_descriptor interd;
	struct libusb_endpoint_descriptor endp;
	uint8_t endpoint_type;
	uint8_t cpt = 0;
	uint8_t done = 0;

	for(i=0; i<config->bNumInterfaces; i++)
	{
		interf = (config->interface)[i];
		interd = (interf.altsetting)[0];
		printf("\t* Numero de l'interface : %d\n",interd.bInterfaceNumber);
		
		printf("\t\t+ Nombre de endpoints : %d\n",interd.bNumEndpoints);
		for(k=0; k<interd.bNumEndpoints; k++)
		{
			endp = (interd.endpoint)[k];
			printf("\t\t\t- Attributes : %x\n",endp.bmAttributes);
			endpoint_type = endp.bmAttributes & 0x03;
			switch(endpoint_type)
			{
				case 0:
					printf("\t\t\t- Type : Control\n");
					break;
				case 1:
					printf("\t\t\t- Type Isochronous\n");
					break;
				case 2:
					printf("\t\t\t- Type : Bulk\n");
					break;
				case 3:
					printf("\t\t\t- Type : Interrupt\n");
					endp_list[cpt] = endp;
					cpt++;
					done = 1;
					break;	
			}
			
			if(done == 1){break;}
			
			
		}		
		
		
		if(libusb_claim_interface(handle,interd.bInterfaceNumber))
		{perror("Claim error"); exit(-1);}
		
				
	}

	libusb_attach_kernel_driver(handle,0);
	libusb_attach_kernel_driver(handle,1);

	//uint8_t endpoint_out=endp_list[1].bEndpointAddress;   /* ID of endpoint (bit 8 is 0) */
	uint8_t endpoint_in=endp_list[0].bEndpointAddress;    /* ID of endpoint (bit 8 is 1) */
	unsigned char data[8];    /* data to send or to receive */
	int size=8;           /* size to send or maximum size to receive */
	int timeout=1000;        /* timeout in ms */

	/* OUT interrupt, from host to device */
	/*int bytes_out;
	int status=libusb_interrupt_transfer(handle,endpoint_out,data,size,&bytes_out,timeout);
	if(status!=0){ perror("libusb_interrupt_transfer"); exit(-1); }
	*/
	/* IN interrupt, host polling device */
	while(1){
		int bytes_in;
		libusb_interrupt_transfer(handle,endpoint_in,data,size,&bytes_in,timeout);
		//if(status!=0){ perror("libusb_interrupt_transfer"); exit(-1); }
		for(int i = 0;i<bytes_in;i++){
			printf("Received : %d\n",data[i]);
		}
	}
	
/*
	for(i=0; i<config->bNumInterfaces; i++)
	{		
		interf = (config->interface)[i];
		interd = (interf.altsetting)[0];
		libusb_release_interface(handle,interd.bInterfaceNumber);		

	}
*/

	libusb_close(handle);
	libusb_exit(context);


	return 0;
}

