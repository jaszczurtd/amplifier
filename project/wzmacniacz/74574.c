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
    sbi(PORTC, PC5);
    sbi(PORTC, PC6);
    
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
    cbi(PORTC, PC5);
    cbi(PORTC, PC6);
}

void setPower(bool state) {
    cbi(PORTC, PC6);
    
    _delay_us(1);
    (state) ? sbi(PORTA, PA3) : cbi(PORTA, PA3);
    _delay_us(1);
    
    sbi(PORTC, PC6);
}

void setRadio(bool state) {
    
}

//cbi(PORTC, PC5);
//sbi(PORTC, PC5);
