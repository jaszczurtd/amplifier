//
//  ds1267.c
//  Index
//
//  Created by Marcin Kielesiński on 21/12/2019.
//  Copyright © 2019 Commend. All rights reserved.
//

#include "ds1267.h"

void initDS1267(void) {
    
}

void setDS1267(uint8_t Stack, uint8_t Pot0, uint8_t Pot1) {
    uint8_t temp;

    DS_PORT |= RST_PIN; // Wysoki RST - umożliwienie transmisji
    _delay_us(C_DELAY); // jako t_cc
    
    // Który Stack wybrać
    if(Stack == 0) DS_PORT &= ~DQ_PIN;
    else DS_PORT |= DQ_PIN;

    // Clock sobie tyka
    DS_PORT |= CLK_PIN;
    _delay_us(C_DELAY); // Czas wysokiego stanu
    DS_PORT &= ~CLK_PIN;
    _delay_us(C_DELAY); // Czas niskiego stanu

    // Wysłanie danych Pot1
    temp = Pot1;

    for(uint8_t i = 0; i < 8 ; i++) {
        if((temp & 0x01) == 0) DS_PORT &= ~DQ_PIN; // Najmłodszy bit zawsze
        else DS_PORT |= DQ_PIN;

        DS_PORT |= CLK_PIN;
        _delay_us(C_DELAY);
        DS_PORT &= ~CLK_PIN;
        _delay_us(C_DELAY);

        temp = temp >> 1; // Przesunięcie rejestru
    }

    // Wysłanie informacji o Pot0
    temp = Pot0;

    for(uint8_t i = 0; i < 8 ; i++) {
        if((temp & 0x01) == 0) DS_PORT &= ~DQ_PIN;
        else DS_PORT |= DQ_PIN;

        DS_PORT |= CLK_PIN;
        _delay_us(C_DELAY); //
        DS_PORT &= ~CLK_PIN;
        _delay_us(C_DELAY); // Niski CLK

        temp = temp >> 1;
    }

    DS_PORT &= ~RST_PIN; // Opuszczenie Resetu
 
}
