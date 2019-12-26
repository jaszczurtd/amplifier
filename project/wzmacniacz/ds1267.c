//
//  ds1267.c
//  Index
//
//  Created by Marcin Kielesiński on 21/12/2019.
//  Copyright © 2019 Commend. All rights reserved.
//

#include "ds1267.h"

/*
void Send_DS1267(uint8_t Stack, uint8_t Pot0, uint8_t Pot1) {
    uint8_t temp;

    PORTD |= RST_PIN; // Wysoki RST - umożliwienie transmisji
    _delay_us(C_DELAY); // jako t_cc
    
    // Który Stack wybrać
    if(Stack == 0) PORTD &= ~DQ_PIN;
    else PORTD |= DQ_PIN;

    // Clock sobie tyka
    PORTD |= CLK_PIN;
    _delay_us(C_DELAY); // Czas wysokiego stanu
    PORTD &= ~CLK_PIN;
    _delay_us(C_DELAY); // Czas niskiego stanu

    // Wysłanie danych Pot1
    temp = Pot1;

    for(uint8_t i = 0; i < 8 ; i++) {
        if((temp & 0x01) == 0) PORTD &= ~DQ_PIN; // Najmłodszy bit zawsze
        else PORTD |= DQ_PIN;

        PORTD |= CLK_PIN;
        _delay_us(C_DELAY);
        PORTD &= ~CLK_PIN;
        _delay_us(C_DELAY);

        temp = temp >> 1; // Przesunięcie rejestru
    }

    // Wysłanie informacji o Pot0
    temp = Pot0;

    for(uint8_t i = 0; i < 8 ; i++) {
        if((temp & 0x01) == 0) PORTD &= ~DQ_PIN;
        else PORTD |= DQ_PIN;

        PORTD |= CLK_PIN;
        _delay_us(C_DELAY); //
        PORTD &= ~CLK_PIN;
        _delay_us(C_DELAY); // Niski CLK

        temp = temp >> 1;
    }

    PORTD &= ~RST_PIN; // Opuszczenie Resetu
 
}

*/
