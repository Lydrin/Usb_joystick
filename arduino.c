#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

//Bouton haut <=> D4 <=> PD4
//Bouton gauche <=> D6 <=> PD6
//Bouton droit <=> D3 <=> PD3 
//Bouton bas <=> D5 <=> PD5


#define BOUTON_HAUT_PORT PORTD
#define BOUTON_HAUT_PIN PD4
#define BOUTON_HAUT_INT PCINT20
#define BOUTON_HAUT_INPUT_REG PCMSK

#define BOUTON_BAS PORTD
#define BOUTON_BAS_PIN PD5
#define BOUTON_BAS_INT PCINT21
#define BOUTON_BAS_INPUT_REG PCMSK

#define BOUTON_GAUCHE_PORT PORTD
#define BOUTON_GAUCHE_PIN PD6
#define BOUTON_GAUCHE_INT PCINT22
#define BOUTON_GAUCHE_INPUT_REG PCMSK

#define BOUTON_DROIT_PORT PORTD
#define BOUTON_DROIT_PIN PD3
#define BOUTON_DROIT_INT PCINT19
#define BOUTON_DROIT_INPUT_REG PCMSK


void init_boutons(void){    
    BOUTON_HAUT_INPUT_REG |= 1<<BUTON_HAUT_PIN;
    BOUTON_BAS_INPUT_REG |= 1<<BUTON_BAS_PIN;
    BOUTON_GAUCHE_INPUT_REG |= 1<<BUTON_GAUCHE_PIN;
    BOUTON_DROIT_INPUT_REG |= 1<<BUTON_DROIT_PIN;
    BOUTON_HAUT_PORT |= (0<<PUD);
}
