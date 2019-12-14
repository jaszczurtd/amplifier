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
    bitSet(GIMSK, INT1);
    bitSet(GICR, INT1);
    
    //falling edge of INT1
    bitClear(MCUCR, ISC01);
    bitSet(MCUCR, ISC11);
    
    //PD0 and PD1 - impulsator input, PD3 - interrupt
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
