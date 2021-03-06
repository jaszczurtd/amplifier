//
//  Outputs.c
//  Index
//
//  Created by Marcin Kielesiński on 02/02/2020.
//

#include "Outputs.h"

static unsigned char outputsRegister = 0;

inline bool EORBit(unsigned char bit) {
    if(bit_is_set(MEM[E_OUTPUTS], bit)) {
        bitClear(MEM[E_OUTPUTS], bit);
    } else {
        bitSet(MEM[E_OUTPUTS], bit);
    }
    return bit_is_set(MEM[E_OUTPUTS], bit);
}

bool getLoudness(void) {
    return checkIfOutputIsActive(BIT_LOUDNESS);
}

bool checkIfOutputIsActive(unsigned char bit) {
    return bit_is_set(MEM[E_OUTPUTS], bit);
}

void restoreOutputs(void) {
    
    setVolume(true);
    
    setLoudness(true, getLoudness());
    
    delay_ms(DELAY_BETWEEN_STATES);
    
    if(MEM[E_OUTPUTS] == 0) {
        setSpecifiedOutputDisableOthers(BIT_DAC);
    }

    if(checkIfOutputIsActive(BIT_RADIO)){
        restorePrograms();
        setRadio(true);
    } else {
        setRadio(false);
        disableAllPrograms();
    }
    
    if(checkIfOutputIsActive(BIT_DAC)){
        setDAC(true);
    } else {
        setDAC(false);
    }

    if(checkIfOutputIsActive(BIT_TAPE)){
        setTape(true);
    } else {
        setTape(false);
    }

    if(checkIfOutputIsActive(BIT_PIEZO)){
        setPiezo(true);
    } else {
        setPiezo(false);
    }

    if(checkIfOutputIsActive(BIT_GENERIC)){
        setGeneric(true);
    } else {
        setGeneric(false);
    }
    
    delay_ms(DELAY_BETWEEN_STATES);
    setVolume(false);
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

unsigned char lastLoudness = -1;
void setLoudness(bool restoreOnly, unsigned char state) {
    
    if(lastLoudness != state) {
        lastLoudness = state;
        
        setSpeakers(false);
        
        if(restoreOnly) {
            delay_ms(LOUDNESS_START_DELAY);
        }
        delay_ms(LOUDNESS_START_DELAY);

        if(state) {
            bitSet(outputsRegister, BIT_LOUDNESS);
        } else {
            bitClear(outputsRegister, BIT_LOUDNESS);
        }
        pcf8574writeByte(PORT_OUTPUTS, outputsRegister);

        if(restoreOnly) {
            delay_ms(LOUDNESS_START_DELAY);
        } else {
            delay_ms(LOUDNESS_END_DELAY);
        }
        setSpeakers(speakersFlag);
    }
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
    
    if(MEM[E_PROGRAMS] == 0) {
        setSpecifiedProgramDisableOthers(BIT_PR_1);
    }
    
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
    
    delay_ms(DELAY_BETWEEN_STATES);
    setVolume(true);

    MEM[E_PROGRAMS] = 0;
    setProgram(bit, true);
    restorePrograms();
    setStoreStatusFlag(true);
    
    delay_ms(DELAY_BETWEEN_STATES);
    setVolume(false);
}

unsigned char getActiveFreq(void) {
    if(bit_is_clear(PINA, PC1)) {
        return AM;
    }
    return FM;
}

static char frequencyValueString[6];
const char *getFrequencyValueString(void) {
    
    int F_MIN = FM_MIN_FREQUENCY;
    int F_MAX = FM_MAX_FREQUENCY;
    
    if(getActiveFreq() == AM) {
        F_MIN = AM_MIN_FREQUENCY;
        F_MAX = AM_MAX_FREQUENCY;
    }
    
    float percent = ((float)getADCValue() * 100) / ADC_MAX_VALUE;
    float frequency = ((percent / 100) * (F_MAX - F_MIN));
    
    float num = ((float)(frequency + F_MIN)) / 10.0f;
    int intpart = (int)num;
    int decpart = ((float)(num - intpart)) * 10;
    
    if(getActiveFreq() == AM) {
        snprintf(frequencyValueString, sizeof(frequencyValueString), "%d%d", intpart, decpart);
    } else {
        snprintf(frequencyValueString, sizeof(frequencyValueString), "%d.%d", intpart, decpart);
    }
    
    return frequencyValueString;
}

const char *getFrequencyString(void) {
    if(getActiveFreq() == AM) {
        return "kHz";
    }
    return "MHz";
}
