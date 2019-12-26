//
//  adc.c
//  Index
//
//  Created by Marcin Kielesiński on 26/12/2019.
//

#include "adc.h"

void ADC_Init(void) {
    bitSet(ADMUX, REFS0);
    bitClear(ADMUX, REFS1);
    
    bitClear(ADMUX, MUX0);
    bitClear(ADMUX, MUX1);
    bitClear(ADMUX, MUX2);

    bitSet(ADCSRA, ADEN | ADPS0 | ADPS1 | ADPS2);
}

int getADCValue(void) {
    
    bitSet(ADCSRA, ADSC);
    while(ADCSRA & (1<<ADSC));
    
    return ADC;
}
