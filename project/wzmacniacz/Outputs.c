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
    "Uniwersalne",
    "CD/Magnetofon",
    "Gramofon",
    "Radio",
    "Konwerter D/A",
};

static unsigned char outputsRegister = 0;
static unsigned char programsRegister = 0;

const char *getOutputString(unsigned char bit) {
    return outputs[bit];
}

inline bool EORBit(unsigned char bit) {
    if(bit_is_set(MEM[E_OUTPUTS], bit)) {
        bitClear(MEM[E_OUTPUTS], bit);
    } else {
        bitSet(MEM[E_OUTPUTS], bit);
    }
    return bit_is_set(MEM[E_OUTPUTS], bit);
}

bool getLoudness(void) {
    return bit_is_set(MEM[E_OUTPUTS], BIT_LOUDNESS);
}

void restoreOutputs(void) {
    
    setLoudness(getLoudness());

    const char *pointer = NULL;
    if(bit_is_set(MEM[E_OUTPUTS], BIT_RADIO)){
        restorePrograms();
        setRadio(true);
        pointer = getOutputString(BIT_RADIO);
    } else {
        setRadio(false);
        disableAllPrograms();
    }
    
    if(bit_is_set(MEM[E_OUTPUTS], BIT_DAC)){
        setDAC(true);
        pointer = getOutputString(BIT_DAC);
    } else {
        setDAC(false);
    }

    if(bit_is_set(MEM[E_OUTPUTS], BIT_TAPE)){
        setTape(true);
        pointer = getOutputString(BIT_TAPE);
    } else {
        setTape(false);
    }

    if(bit_is_set(MEM[E_OUTPUTS], BIT_PIEZO)){
        setPiezo(true);
        pointer = getOutputString(BIT_PIEZO);
    } else {
        setPiezo(false);
    }

    if(bit_is_set(MEM[E_OUTPUTS], BIT_GENERIC)){
        setGeneric(true);
        pointer = getOutputString(BIT_GENERIC);
    } else {
        setGeneric(false);
    }

    if(pointer != NULL) {
        strncpy(outputString, pointer, S_WIDTH);
    } else {
        memset(outputString, 0, S_WIDTH + 1);
    }
}

void setSpecifiedOutputDisableOthers(unsigned char bit) {
    
    bitClear(MEM[E_OUTPUTS], BIT_GENERIC);
    bitClear(MEM[E_OUTPUTS], BIT_TAPE);
    bitClear(MEM[E_OUTPUTS], BIT_PIEZO);
    bitClear(MEM[E_OUTPUTS], BIT_RADIO);
    bitClear(MEM[E_OUTPUTS], BIT_DAC);
    
    bitSet(MEM[E_OUTPUTS], bit);
    
    restoreOutputs();
    setStoreStatusFlag(true);
}

void clearOutputs(void) {
    bitClear(outputsRegister, BIT_GENERIC);
    bitClear(outputsRegister, BIT_TAPE);
    bitClear(outputsRegister, BIT_PIEZO);
    bitClear(outputsRegister, BIT_RADIO);
    bitClear(outputsRegister, BIT_DAC);
    bitClear(outputsRegister, BIT_LOUDNESS);
    pcf8574writeByte(PORT_OUTPUTS, outputsRegister);
}

void setLoudness(bool state) {
    if(state) {
        bitSet(outputsRegister, BIT_LOUDNESS);
    } else {
        bitClear(outputsRegister, BIT_LOUDNESS);
    }
    pcf8574writeByte(PORT_OUTPUTS, outputsRegister);
}

void setRadio(bool state) {
    if(state) {
        bitSet(outputsRegister, BIT_RADIO);
    } else {
        bitClear(outputsRegister, BIT_RADIO);
    }
    pcf8574writeByte(PORT_OUTPUTS, outputsRegister);
}

void setDAC(bool state) {
    if(state) {
        bitSet(outputsRegister, BIT_DAC);
    } else {
        bitClear(outputsRegister, BIT_DAC);
    }
    pcf8574writeByte(PORT_OUTPUTS, outputsRegister);
}

void setTape(bool state) {
    if(state) {
        bitSet(outputsRegister, BIT_TAPE);
    } else {
        bitClear(outputsRegister, BIT_TAPE);
    }
    pcf8574writeByte(PORT_OUTPUTS, outputsRegister);
}

void setGeneric(bool state) {
    if(state) {
        bitSet(outputsRegister, BIT_GENERIC);
    } else {
        bitClear(outputsRegister, BIT_GENERIC);
    }
    pcf8574writeByte(PORT_OUTPUTS, outputsRegister);
}

void setPiezo(bool state) {
    if(state) {
        bitSet(outputsRegister, BIT_PIEZO);
    } else {
        bitClear(outputsRegister, BIT_PIEZO);
    }
    pcf8574writeByte(PORT_OUTPUTS, outputsRegister);
}

void setPowerRes(bool state) {
    if(state) {
        bitSet(outputsRegister, BIT_POWER_RES);
    } else {
        bitClear(outputsRegister, BIT_POWER_RES);
    }
    pcf8574writeByte(PORT_OUTPUTS, outputsRegister);
}

void setSpeakers(bool state) {
    if(state) {
        bitSet(outputsRegister, BIT_SPEAKERS);
    } else {
        bitClear(outputsRegister, BIT_SPEAKERS);
    }
    pcf8574writeByte(PORT_OUTPUTS, outputsRegister);
}

void restorePrograms(void) {
    pcf8574writeByte(PORT_PROGRAMS, MEM[E_PROGRAMS]);
}

void disableAllPrograms(void) {
    pcf8574writeByte(PORT_PROGRAMS, 0);
}

void setProgram(int bit, bool state) {
    if(state) {
        bitSet(MEM[E_PROGRAMS], bit);
    } else {
        bitClear(MEM[E_PROGRAMS], bit); 
    }
}

void setSpecifiedProgramDisableOthers(unsigned char bit) {
    MEM[E_PROGRAMS] = 0;
    setProgram(bit, true);
    restorePrograms();
    setStoreStatusFlag(true);
}

