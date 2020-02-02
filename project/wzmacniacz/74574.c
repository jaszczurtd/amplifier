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

    //data bits
    sbi(DDRC, PC7);
    sbi(DDRA, PA7);
    sbi(DDRA, PA6);
    sbi(DDRA, PA5);
    sbi(DDRA, PA4);
    sbi(DDRA, PA3);
    sbi(DDRA, PA2);
    sbi(DDRA, PA1);
    
    clearPorts();
}

void clearPorts(void) {
    //clock transition
    clockPort(PORT_OUTPUTS, false);
    clockPort(PORT_PROGRAMS, false);

    //set initial state to "off"
    cbi(PORTC, PC7);
    cbi(PORTA, PA1);
    cbi(PORTA, PA2);
    cbi(PORTA, PA3);
    cbi(PORTA, PA4);
    cbi(PORTA, PA5);
    cbi(PORTA, PA6);
    cbi(PORTA, PA7);
    
    //clock transition
    clockPort(PORT_OUTPUTS, true);
    clockPort(PORT_PROGRAMS, true);
}

void clockPort(unsigned char portNumber, bool state) {
    switch(portNumber) {
        case PORT_OUTPUTS:
            
            (state) ? sbi(PORTC, PC6) : cbi(PORTC, PC6);
            
            break;
            
        case PORT_PROGRAMS:
            
            (state) ? sbi(PORTC, PC5) : cbi(PORTC, PC5);

            break;
    }
    _delay_us(1);
}

