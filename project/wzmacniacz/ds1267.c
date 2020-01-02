//
//  ds1267.c
//  Index
//
//  Created by Marcin Kielesiński on 21/12/2019.
//  Copyright © 2019 Commend. All rights reserved.
//

#include "ds1267.h"

void initDS1267(void) {
    sbi(DS_PORT_INIT, RST_PIN);
    sbi(DS_PORT_INIT, DQ_PIN);
    sbi(DS_PORT_INIT, CLK_PIN);
    
    cbi(DS_PORT, RST_PIN);
    cbi(DS_PORT, DQ_PIN);
    cbi(DS_PORT, CLK_PIN);
}

static inline void clockUpdate(void) {
    sbi(DS_PORT, CLK_PIN);
    cbi(DS_PORT, CLK_PIN);
}

void setDS1267(unsigned char Pot0, unsigned char Pot1) {

    sbi(DS_PORT, RST_PIN);
    _delay_us(C_DELAY); // jako t_cc
    
    //stack bit
    cbi(DS_PORT, DQ_PIN);
    clockUpdate();

    for(int i = 0; i < 8 ; i++) {
        if(((Pot1 << i) & 128)) {
            sbi(DS_PORT, DQ_PIN);
        } else {
            cbi(DS_PORT, DQ_PIN);
        }
        clockUpdate();
    }

    for(int i = 0; i < 8 ; i++) {
        if(((Pot0 << i) & 128)) {
            sbi(DS_PORT, DQ_PIN);
        } else {
            cbi(DS_PORT, DQ_PIN);
        }
        clockUpdate();
    }

    cbi(DS_PORT, RST_PIN);
}
