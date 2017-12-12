#include "pad.h"
#include <LUFA/Drivers/Peripheral/Serial.h>

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
 
void SendNextReport(void)
{
	/* Select the IN Report Endpoint */
	Endpoint_SelectEndpoint(PAD_IN_EPADDR);

	if (sendReport)
	{
	/* Wait until the host is ready to accept another packet */
		while (!Endpoint_IsINReady()) {}

	/* Write IN Report Data */
		Endpoint_Write_Stream_LE(report, reportLen, NULL);

		sendReport = 0;

	/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
	}
}

/** Reads the next OUT report from the host from the OUT endpoint, if one has been sent. */
void ReceiveNextReport(void)
{
	static struct
	{
		struct
		{
			unsigned char type;
			unsigned char length;
		} header;
		unsigned char buffer[INTERRUPT_EPSIZE];
	} packet = { .header.type = BYTE_OUT_REPORT };

	uint16_t length = 0;

	/* Select the OUT Report Endpoint */
	Endpoint_SelectEndpoint(PAD_OUT_EPADDR);

	/* Check if OUT Endpoint contains a packet */
	if (Endpoint_IsOUTReceived())
	{
	/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
	/* Read OUT Report Data */
			uint8_t ErrorCode = Endpoint_Read_Stream_LE(packet.buffer, sizeof(packet.buffer), &length);
			if(ErrorCode == ENDPOINT_RWSTREAM_NoError)
			{
				length = sizeof(packet.buffer);
			}
		}	

	/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearOUT();

		if(length)
		{
			packet.header.length = length & 0xFF;
			Serial_SendData(&packet, sizeof(packet.header) + packet.header.length);

			if(!spoof_initialized && packet.buffer[0] == 0x06 && packet.buffer[1] == 0x20)
			{
				spoof_initialized = 1;
			}
		}
	}
}

void PAD_Task(void)
{
  /* Device must be connected and configured for the task to run */
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  /* Send the next keypress report to the host */
  SendNextReport();

  /* Process the LED report sent from the host */
  ReceiveNextReport();
}
 
int main(void)
{
	SetupHardware();

	GlobalInterruptEnable();

	for (;;)
	  PAD_Task();
	  USB_USBTask();
}

/** Configures the board hardware and chip peripherals for the project's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);
#endif

	/* Hardware Initialization */
	USB_Init();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{

	if(((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_TYPE) == REQTYPE_CLASS)
		&& ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_DEVICE))
	{
	
		if ((USB_ControlRequest.bmRequestType & CONTROL_REQDIR_DIRECTION) == REQDIR_HOSTTODEVICE)
		{
		
			if(USB_ControlRequest.bRequest == '1')
			{
				// Send serial je pense
			}
			
			if(USB_ControlRequest.bRequest == '0')
			{
				// Send serial
			}			
			
		}
		else
		{
			// Read Serial et envoie
		}
	
	}		

}

