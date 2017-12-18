#include "pad.h"
#include <LUFA/Drivers/Peripheral/Serial.h>

#define USART_BAUDRATE 9600
#define USART_DOUBLE_SPEED 0

uint8_t data_send;
uint8_t state_send = 0;

ISR(USART1_RX_vect)
{
	data_send = UDR1;
	state_send = 1;
}


void EVENT_USB_Device_ConfigurationChanged(void)
{
	Endpoint_ConfigureEndpoint(PAD_IN_EPADDR, EP_TYPE_INTERRUPT, INTERRUPT_EPSIZE, 1);
	Endpoint_ConfigureEndpoint(PAD_OUT_EPADDR, EP_TYPE_INTERRUPT, INTERRUPT_EPSIZE, 1);
	USB_Device_EnableSOFEvents();
}

/*
void EVENT_USB_Device_Connect(void)
{

}

void EVENT_USB_Device_Disconnect(void)
{

}
*/

void SendNextReport(void)
{
	/*
	int data;
	if(Serial_IsCharReceived())
	{
		data = Serial_ReceiveByte();
	}
	*/
	if(state_send)
	{
		Endpoint_SelectEndpoint(PAD_IN_EPADDR);

		if (Endpoint_IsReadWriteAllowed())
		{
			Endpoint_Write_Stream_LE(&data_send, sizeof(uint8_t), NULL);
			Endpoint_ClearIN();
		}	
		
		state_send = 0;
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

	for (;;){
	  USB_USBTask();
	  PAD_Task();
	}
}

void SetupHardware(void)
{
	Serial_Init(USART_BAUDRATE, USART_DOUBLE_SPEED);
	UCSR1B |= (1 << RXCIE1); // Enable the USART Receive Complete interrupt (USART_RXC)
	USB_Init();
}
