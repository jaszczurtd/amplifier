//
//  Outputs.c
//  Index
//
//  Created by Marcin Kielesiński on 02/02/2020.
//

#include "Outputs.h"

static char outputString[S_WIDTH + 1];

const char *getOutputDisplayString(void) {
    return outputString;
}

static char *outputs[5] = {
    "Radio",
    "Konwerter D/A",
    "Uniwersalne",
    "CD/Magnetofon",
    "Gramofon"
};

const char *getOutputString(unsigned char index) {
    return outputs[index];
}

void restoreOutputs(void) {
    setLoudness(MEM[E_LOUDNESS]);
    
    const char *pointer = NULL;
    if(MEM[E_RADIO]){
        setRadio(true);
        pointer = getOutputString(E_RADIO);
    } else {
        setRadio(false);
    }
    
    if(MEM[E_DAC]){
        setDAC(true);
        pointer = getOutputString(E_DAC);
    } else {
        setDAC(false);
    }

    if(MEM[E_TAPE]){
        setTape(true);
        pointer = getOutputString(E_TAPE);
    } else {
        setTape(false);
    }

    if(MEM[E_PIEZO]){
        setPiezo(true);
        pointer = getOutputString(E_PIEZO);
    } else {
        setPiezo(false);
    }

    if(MEM[E_GENERIC]){
        setGeneric(true);
        pointer = getOutputString(E_GENERIC);
    } else {
        setGeneric(false);
    }

    if(pointer != NULL) {
        strncpy(outputString, pointer, S_WIDTH);
    } else {
        memset(outputString, 0, S_WIDTH + 1);
    }
}

void setSpecifiedOutputDisableOthers(unsigned char index) {
    
    MEM[E_RADIO] =
        MEM[E_DAC] =
        MEM[E_TAPE] =
        MEM[E_PIEZO] =
        MEM[E_GENERIC] = false;
    
    restoreOutputs();
    MEM[index] = true;
    
    restoreOutputs();
    setStoreStatusFlag();
}


void setLoudness(bool state) {
    clockPort(PORT_OUTPUTS, false);
    (state) ? sbi(PORTA, PA3) : cbi(PORTA, PA3);
    clockPort(PORT_OUTPUTS, true);
}

void setRadio(bool state) {
    clockPort(PORT_OUTPUTS, false);
    (state) ? sbi(PORTA, PA5) : cbi(PORTA, PA5);
    clockPort(PORT_OUTPUTS, true);
}

void setDAC(bool state) {
    clockPort(PORT_OUTPUTS, false);
    (state) ? sbi(PORTA, PA4) : cbi(PORTA, PA4);
    clockPort(PORT_OUTPUTS, true);
}

void setTape(bool state) {
    clockPort(PORT_OUTPUTS, false);
    (state) ? sbi(PORTA, PA7) : cbi(PORTA, PA7);
    clockPort(PORT_OUTPUTS, true);
}

void setGeneric(bool state) {
    clockPort(PORT_OUTPUTS, false);
    (state) ? sbi(PORTC, PC7) : cbi(PORTC, PC7);
    clockPort(PORT_OUTPUTS, true);
}

void setPiezo(bool state) {
    clockPort(PORT_OUTPUTS, false);
    (state) ? sbi(PORTA, PA6) : cbi(PORTA, PA6);
    clockPort(PORT_OUTPUTS, true);
}

void setPowerRes(bool state) {
    clockPort(PORT_OUTPUTS, false);
    (state) ? sbi(PORTA, PA1) : cbi(PORTA, PA1);
    clockPort(PORT_OUTPUTS, true);
}

void setSpeakers(bool state) {
    clockPort(PORT_OUTPUTS, false);
    (state) ? sbi(PORTA, PA2) : cbi(PORTA, PA2);
    clockPort(PORT_OUTPUTS, true);
}
