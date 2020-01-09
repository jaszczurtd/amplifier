//
//  74574.c
//  Index
//
//  Created by Marcin Kielesiński on 02/01/2020.
//

#include "74574.h"

void init74574(void) {
    
    sbi(DDRC, PC5); //CP a
    sbi(DDRC, PC6); //CP b

    sbi(PORTC, PC5);
    sbi(PORTC, PC6);

    //data bits
    sbi(DDRC, PC7);
    sbi(DDRA, PA7);
    sbi(DDRA, PA6);
    sbi(DDRA, PA5);
    sbi(DDRA, PA4);
    sbi(DDRA, PA3);
    sbi(DDRA, PA2);
    sbi(DDRA, PA1);
    
    
}
