#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

//Bouton haut <=> D4 <=> PD4
//Bouton gauche <=> D6 <=> PD6
//Bouton droit <=> D3 <=> PD3 
//Bouton bas <=> D5 <=> PD5

#define BOUTON_PORT PORTD
#define BOUTON_DDR DDRB
#define BOUTON_PCIE PCIE2

#define BOUTON_HAUT_PIN PD4
#define BOUTON_HAUT_INT PCINT20

#define BOUTON_BAS_PIN PD5
#define BOUTON_BAS_INT PCINT21

#define BOUTON_GAUCHE_PIN PD6
#define BOUTON_GAUCHE_INT PCINT22

#define BOUTON_DROIT_PIN PD3
#define BOUTON_DROIT_INT PCINT19

#define BAUDRATE 103

volatile int state = -1;

void init_boutons(void)
{    
	DDRD &= ~((1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6));
	//PD3,4,5,6 sont maintenant des entrées
	PORTD |= ((1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6));
	//On active le PULL-UP sur les entrées
	PCICR |= (1 << PCIE2);
	PCMSK2 |= ((1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22));
	sei();
}

void init_serial(void)
{
    uint8_t baudrate = BAUDRATE;
    UBRR0H = 0;
    UBRR0L = baudrate;

    UCSR0B = ((1 << RXCIE0) |(1 << TXEN0) | (1 << RXEN0));

    UCSR0C = ((1 << UCSZ01) | (1 << UCSZ00));

    UCSR0A &= ~(1 << U2X0);
}

void send_serial(unsigned char c)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

void output_init(void){
	DDRB |= 0x20;
}

ISR (USART_RX_vect)
{
	if(UDR0 == 49){ //Allumage de la LED
		PORTB |= 0x20;
	}
	if(UDR0 == 48){ //Extinction de la LED
		PORTB &= 0xDF;
	}
}

ISR (PCINT2_vect)
{
    state = ((~(PIND) & 0x7C)>>2);
}

    
int main(void)
{
    init_serial();
    init_boutons();
    output_init();
    uint8_t msg;
    while(1){
	    if(state!=-1){
		    msg = state | 0x20;
		    if(msg != 0x20)
			{
		   		 send_serial(msg);
				 _delay_ms(10);
		   		 state = -1;
			}
	    }
    }
    return 0;
}
