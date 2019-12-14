//
//  Impulsator.c
//  Index
//
//  Created by Marcin Kielesiński on 14/12/2019.
//

#include "Impulsator.h"

static int counter = 0;

void Impulsator_Init(void) {
    //enable INT1 interrupts (PD3)
    GIMSK |= _BV(INT1);
    GICR |= _BV(INT1);
    //falling edge of INT1
    MCUCR &= ~(_BV(ISC01));
    MCUCR |= _BV(ISC11);
    
    cbi(DDRD,PD0);
    cbi(DDRD,PD1);
    cbi(DDRD,PD3);
}

ISR(INT1_vect) {
    
    if(bit_is_clear(PIND, PD0)) {
        counter++;
    }
    if(bit_is_clear(PIND, PD1)) {
        counter++;
    }
}

int getImpulsatorValue(void) {
    return counter;
}
