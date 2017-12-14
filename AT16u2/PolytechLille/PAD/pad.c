#include "pad.h"
#include <LUFA/Drivers/Peripheral/Serial.h>

#define USART_BAUDRATE 9600
#define USART_DOUBLE_SPEED false

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */

void EVENT_USB_Device_ConfigurationChanged(void)
{

	Endpoint_ConfigureEndpoint(PAD_IN_EPADDR, EP_TYPE_INTERRUPT, INTERRUPT_EPSIZE, 1);
	Endpoint_ConfigureEndpoint(PAD_OUT_EPADDR, EP_TYPE_INTERRUPT, INTERRUPT_EPSIZE, 1);

	/* Turn on Start-of-Frame events for tracking HID report period expiry */
	USB_Device_EnableSOFEvents();

}

void ProcessLEDReport(uint8_t data)
{
	// Envoyer en série 0 ou 1 selon la data
	Serial_SendByte(data);
}
 
void SendNextReport(void)
{
	// Fonction pour lire sur le port série et stocker la valeur (pointeur)
	
	uint8_t data;
	if(Serial_IsCharReceived())
	{
		data = Serial_ReceiveByte(); //
	}

	/* Check if the idle period is set and has elapsed */

	/* Select the Keyboard Report Endpoint */
	Endpoint_SelectEndpoint(PAD_IN_EPADDR);

	/* Check if Keyboard Endpoint Ready for Read/Write and if we should send a new report */
	if (Endpoint_IsReadWriteAllowed())
	{
		/* Write Keyboard Report Data */
		Endpoint_Write_Stream_LE(&data, sizeof(uint8_t), NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
	}
}

/** Reads the next LED status report from the host from the LED data endpoint, if one has been sent. */
void ReceiveNextReport(void)
{
	/* Select the Keyboard LED Report Endpoint */
	Endpoint_SelectEndpoint(PAD_OUT_EPADDR);

	/* Check if Keyboard LED Endpoint contains a packet */
	if (Endpoint_IsOUTReceived())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Read in the LED report from the host */
			uint8_t LEDReport = Endpoint_Read_8();

			/* Process the read LED report from the host */
			ProcessLEDReport(LEDReport);
		}

		/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearOUT();
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
	Serial_Init(USART_BAUDRATE, USART_DOUBLE_SPEED);
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


/*
void EVENT_USB_Device_ControlRequest(void) // A comprendre
{

	if(((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_TYPE) == REQTYPE_CLASS)
		&& ((USB_ControlRequest.bmRequestType & CONTROL_REQTYPE_RECIPIENT) == REQREC_DEVICE))
	{
	
		if ((USB_ControlRequest.bmRequestType & CONTROL_REQDIR_DIRECTION) == REQDIR_HOSTTODEVICE)
		{
			
			
		}
		else
		{
			
		}
	
	}		

}
*/

