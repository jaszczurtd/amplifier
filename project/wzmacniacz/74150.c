//
//  74150.c
//  Index
//
//  Created by Marcin Kielesiński on 30/12/2019.
//

#include "74150.h"

void init74150(void) {
    sbi(DDRD,PD7); //D
    sbi(DDRD,PD6); //C
    sbi(DDRD,PD5); //B
    sbi(DDRD,PD4); //A
    
    cbi(DDRD,PD3); //output
}

static int result = 0;
int read74150(void) {
    
    register char res = I_74150_NOT_ACTIVE;
    register char hits = 0;
    
    for(register int a = 0; a < 16; a++) {
        
        if(((unsigned char)a & 1)) {
            sbi(PORTD, PD4);
        } else {
            cbi(PORTD, PD4);
        }
        if(((unsigned char)a >> 1) & 1) {
            sbi(PORTD, PD5);
        } else {
            cbi(PORTD, PD5);
        }
        if(((unsigned char)a >> 2) & 1) {
            sbi(PORTD, PD6);
        } else {
            cbi(PORTD, PD6);
        }
        if(((unsigned char)a >> 3) & 1) {
            sbi(PORTD, PD7);
        } else {
            cbi(PORTD, PD7);
        }
        _delay_us(1);
        
        if(bit_is_set(PIND, PD3)) {
            res = a;
            hits++;
        }
    }
    
    if(hits == 1) {
        result = res;
    } else {
        result = I_74150_NOT_ACTIVE;
    }
    
    return result;
}
