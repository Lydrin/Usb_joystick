#include "pad.h"
#include <LUFA/Drivers/Peripheral/Serial.h>

#define USART_BAUDRATE 9600
#define USART_DOUBLE_SPEED false


void EVENT_USB_Device_ConfigurationChanged(void)
{
	Endpoint_ConfigureEndpoint(PAD_IN_EPADDR, EP_TYPE_INTERRUPT, INTERRUPT_EPSIZE, 1);
	Endpoint_ConfigureEndpoint(PAD_OUT_EPADDR, EP_TYPE_INTERRUPT, INTERRUPT_EPSIZE, 1);
	USB_Device_EnableSOFEvents();
}

void EVENT_USB_Device_Connect(void)
{

}

void EVENT_USB_Device_Disconnect(void)
{

}

void SendNextReport(void)
{
	uint8_t data;
	if(Serial_IsCharReceived())
	{
		data = Serial_ReceiveByte(); //
	}

	Endpoint_SelectEndpoint(PAD_IN_EPADDR);

	if (Endpoint_IsReadWriteAllowed())
	{
		Endpoint_Write_Stream_LE(&data, sizeof(uint8_t), NULL);
		Endpoint_ClearIN();
	}
}

void ReceiveNextReport(void)
{
	Endpoint_SelectEndpoint(PAD_OUT_EPADDR);

	if (Endpoint_IsOUTReceived())
	{
		if (Endpoint_IsReadWriteAllowed())
		{
			uint8_t LEDReport = Endpoint_Read_8();
			Serial_SendByte(LEDReport);
		}

		Endpoint_ClearOUT();
	}
}

void PAD_Task(void)
{
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;
		
	SendNextReport();
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

void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	clock_prescale_set(clock_div_1);
#endif

	Serial_Init(USART_BAUDRATE, USART_DOUBLE_SPEED);
	USB_Init();
}
