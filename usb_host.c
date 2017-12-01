#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define ID_PRODUCT 0x2003

libusb_device_handle * handle;


int main(void)
{
	int err = 0;
	libusb_device *my_device = NULL;
	libusb_context *context;
	if(libusb_init(&context)i!=0) {perror("libusb_init"); exit(-1);}
	libusb_device* my_device = get_usb_device(context);
	struct libusb_device_descriptor desc;
	uint8_t bus = libusb_get_bus_number(my_device)	
	

	err = libusb_open(my_device, &handle);
	//printf("err = %d\n",err);
	if(err){perror("Fail open device"); exit(-1);}


	my_device = libusb_get_device(handle); //Inutile dans ce cas précis
	struct libusb_config_descriptor * config;
	libusb_get_active_config_descriptor(my_device,&config);

	uint8_t num_config = config->bConfigurationValue;
	uint8_t k = 0;
	
	struct libusb_interface interf;
	struct libusb_interface_descriptor interd;
	struct libusb_endpoint_descriptor endp;	


	
	for(i=0; i<config->bNumInterfaces; i++)
	{
		interf = (config->interface)[i];
		interd = (interf.altsetting)[0];


		if(libusb_kernel_driver_active(handle,interd.bInterfaceNumber)){
			int status=libusb_detach_kernel_driver(handle,interd.bInterfaceNumber);
			if(status!=0){ perror("libusb_detach_kernel_driver"); exit(-1); }
		}
				
	}

	libusb_get_config_descriptor(my_device,0,&config);
	num_config = config->bConfigurationValue;
	if(libusb_set_configuration(handle, num_config)){perror("libusb_set_configuration"); exit(-1);}


	printf("\nTaille de la config : %d\n",config->bLength);
	printf("Consommation : %d\n",config->MaxPower);
	printf("\n##### Interfaces : %d #####\n\n",config->bNumInterfaces);
	num_config = config->bConfigurationValue;
	k = 0;
	
	/*
	struct libusb_interface interf;
	struct libusb_interface_descriptor interd;
	struct libusb_endpoint_descriptor endp;	
	*/


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

	libusb_close(handle);
	libusb_free_device_list(list,1);
	libusb_exit(context);


	return 0;
}

