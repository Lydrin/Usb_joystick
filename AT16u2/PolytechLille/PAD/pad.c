#include "pad.h"


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	GlobalInterruptEnable();

	for (;;)
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

	/* Initialize Relays */
	DDRC  |=  ALL_RELAYS;
	PORTC &= ~ALL_RELAYS;
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    const uint8_t SerialNumber[5] = { 0, 0, 0, 0, 1 };
	uint8_t ControlData[2]        = { 0, 0 };
	
	
/*
    switch (USB_ControlRequest.bRequest)
	{
		case 0x09:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				LEDs_ToggleLEDs(LEDS_LED1);

				Endpoint_ClearSETUP();

				Endpoint_Read_Control_Stream_LE(ControlData, sizeof(ControlData));
				Endpoint_ClearIN();

				switch (USB_ControlRequest.wValue)
				{
					case 0x303:
						if (ControlData[1]) PORTC &= ~RELAY1; else PORTC |= RELAY1;
						break;
					case 0x306:
						if (ControlData[1]) PORTC &= ~RELAY2; else PORTC |= RELAY2;
						break;
					case 0x309:
						if (ControlData[1]) PORTC &= ~RELAY3; else PORTC |= RELAY3;
						break;
					case 0x30c:
						if (ControlData[1]) PORTC &= ~RELAY4; else PORTC |= RELAY4;
						break;
				}
			}

			break;
		case 0x01:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				LEDs_ToggleLEDs(LEDS_LED1);

				Endpoint_ClearSETUP();

				switch (USB_ControlRequest.wValue)
				{
					case 0x301:
						Endpoint_Write_Control_Stream_LE(SerialNumber, sizeof(SerialNumber));
						break;
					case 0x303:
						ControlData[1] = (PORTC & RELAY1) ? 2 : 3;
						break;
					case 0x306:
						ControlData[1] = (PORTC & RELAY2) ? 2 : 3;
						break;
					case 0x309:
						ControlData[1] = (PORTC & RELAY3) ? 2 : 3;
						break;
					case 0x30c:
						ControlData[1] = (PORTC & RELAY4) ? 2 : 3;
						break;
				}

				if (ControlData[1])
				  Endpoint_Write_Control_Stream_LE(ControlData, sizeof(ControlData));

				Endpoint_ClearOUT();
			}

			break;
	}
	*/
}

