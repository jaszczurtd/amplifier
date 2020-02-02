//
//  Inputs.c
//  Index
//
//  Created by Marcin Kielesiński on 02/02/2020.
//

#include "Inputs.h"

int rc5Code, switchCode;

void initInputs(void) {
    sbi(DDRB, PB2);    //power
}

void readCommands(int time) {
    switchCode = read74150();
    rc5Code = RC5_NewCommandReceived();
    if(time > 0) {
        delay_ms(time);
    }
    WR();
}

bool power_sw(void) {
    return (switchCode == I_74150_POWER) || (rc5Code == RC5_POWER);
}

bool generic_sw(void) {
    return (switchCode == I_74150_GEN) || (rc5Code == RC5_GEN);
}

bool tape_sw(void) {
    return (switchCode == I_74150_TAPE) || (rc5Code == RC5_TAPE);
}

bool piezo_sw(void) {
    return (switchCode == I_74150_PIEZO) || (rc5Code == RC5_PIEZO);
}

bool radio_sw(void) {
    return (switchCode == I_74150_RADIO) || (rc5Code == RC5_RADIO);
}

bool dac_sw(void) {
    return (switchCode == I_74150_DAC) || (rc5Code == RC5_DAC);
}

bool loudness_sw(void) {
    return (switchCode == I_74150_LOUDNESS) || (rc5Code == RC5_LOUDNESS);
}
