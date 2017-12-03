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

volatile uint8_t ancien_portd = 0xFF; 

void init_boutons(void){    
   DDRD &= ~((1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6));
   //PD3,4,5,6 sont maintenant des entrées
   PORTD |= ((1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6));
   //On active le PULL-UP sur les entrées
   PCICR |= (1 << PCIE2);
   PCMSK2 |= ((1<< PCINT19) | (1<< PCINT20) | (1<< PCINT21) | (1<< PCINT22));
   sei();
}

void init_serial(void){
    uint8_t baudrate = BAUDRATE;
    UBRR0H = 0;
    UBRR0L = baudrate;

    UCSR0B = (1<<TXEN0);

    UCSR0C = 0x06;
}

void send_serial(unsigned char c)
{
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

void wait(void){
    cli();
    _delay_ms(10);
    sei();
}


ISR (PCINT2_vect){
    uint8_t bits_change;
    bits_change = PIND ^ ancien_portd; 
    ancien_portd = PIND;
    if(bits_change & (1 << PIND3) ){
        if( (PIND & (1 << PIND3) ) == (1 << PIND3)){
            //Front montant
            send_serial('d');
        }
        else{
            send_serial('D');
            //Front descendant
        }
    }
    if(bits_change & (1 << PIND4) ){
        if( (PIND & (1 << PIND4) ) == (1 << PIND4)){
            send_serial('h');
        }
        else{
            send_serial('H');
        }
    }
    if(bits_change & (1 << PIND5) ){
        if( (PIND & (1 << PIND5) ) == (1 << PIND5)){
            send_serial('b');
        }
        else{
            send_serial('B');
        }
    }
    if(bits_change & (1 << PIND6) ){
        if( (PIND & (1 << PIND6) ) == (1 << PIND6)){
            send_serial('g');
        }
        else{
            send_serial('G');
        }
    }
    /* TODO : Gerer les rebondissements */
    wait();
}


    
int main(void){
    init_serial();
    init_boutons();
    while(1){
    }
    return 0;
}
