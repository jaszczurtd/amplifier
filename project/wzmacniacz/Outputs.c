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
        restorePrograms();
        setRadio(true);
        pointer = getOutputString(E_RADIO);
    } else {
        setRadio(false);
        disableAllPrograms();
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
    setStoreStatusFlag(true);
}


void setLoudness(bool state) {
    
}

void setRadio(bool state) {
    
}

void setDAC(bool state) {
    
}

void setTape(bool state) {
    
}

void setGeneric(bool state) {
    
}

void setPiezo(bool state) {
    
}

void setPowerRes(bool state) {
    
}

void setSpeakers(bool state) {
    
}

void restorePrograms(void) {
    
}

void disableAllPrograms(void) {
    pcf8574writeByte(PORT_PROGRAMS, 0);
}

void setProgram(int index, bool state) {
    
}

void setSpecifiedProgramDisableOthers(unsigned char index) {
    
    MEM[E_PR_1] =
    MEM[E_PR_2] =
    MEM[E_PR_3] =
    MEM[E_PR_4] =
    MEM[E_PR_5] =
    MEM[E_PR_6] =
    MEM[E_PR_7] =
    MEM[E_PR_8] = false;
    
    MEM[E_PR_1 + index] = true;
    
    restorePrograms();
    setStoreStatusFlag(true);
}

