//
//  Inputs.c
//  Index
//
//  Created by Marcin Kielesiński on 02/02/2020.
//

#include "Inputs.h"

int rc5Code, switchCode;

void initInputs(void) {
    sbi(DDRB, PB2);  //power
    
    cbi(DDRA, PA1);  //AM/FM
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

void readSelectorInputs(void) {
    if(tape_sw()) {
        while (tape_sw()) { RC(); }
        setSpecifiedOutputDisableOthers(BIT_TAPE);
    }
    if(radio_sw()) {
        while (radio_sw()) { RC(); }
        setSpecifiedOutputDisableOthers(BIT_RADIO);
    }
    if(dac_sw()) {
        while (dac_sw()) { RC(); }
        setSpecifiedOutputDisableOthers(BIT_DAC);
    }
    if(piezo_sw()) {
        while (piezo_sw()) { RC(); }
        setSpecifiedOutputDisableOthers(BIT_PIEZO);
    }
    if(generic_sw()) {
        while (generic_sw()) { RC(); }
        setSpecifiedOutputDisableOthers(BIT_GENERIC);
    }
}

bool pr_1_sw(void) {
    return (switchCode == I_74150_PR_1 || rc5Code == RC5_PR_1);
}
bool pr_2_sw(void) {
    return (switchCode == I_74150_PR_2 || rc5Code == RC5_PR_2);
}
bool pr_3_sw(void) {
    return (switchCode == I_74150_PR_3 || rc5Code == RC5_PR_3);
}
bool pr_4_sw(void) {
    return (switchCode == I_74150_PR_4 || rc5Code == RC5_PR_4);
}
bool pr_5_sw(void) {
    return (switchCode == I_74150_PR_5 || rc5Code == RC5_PR_5);
}
bool pr_6_sw(void) {
    return (switchCode == I_74150_PR_6 || rc5Code == RC5_PR_6);
}
bool pr_7_sw(void) {
    return (switchCode == I_74150_PR_7 || rc5Code == RC5_PR_7);
}
bool pr_8_sw(void) {
    return (switchCode == I_74150_PR_8 || rc5Code == RC5_PR_8);
}

bool pr_plus_sw(void) {
    return rc5Code == RC5_CHANNEL_PLUS;
}
bool pr_minus_sw(void) {
    return rc5Code == RC5_CHANNEL_MINUS;
}

void readRadioInputs(void) {
    if(checkIfOutputIsActive(BIT_RADIO)) {
        if(pr_1_sw()) {
            while (pr_1_sw()) { RC(); }
            setSpecifiedProgramDisableOthers(BIT_PR_1);
        }
        if(pr_2_sw()) {
            while (pr_2_sw()) { RC(); }
            setSpecifiedProgramDisableOthers(BIT_PR_2);
        }
        if(pr_3_sw()) {
            while (pr_3_sw()) { RC(); }
            setSpecifiedProgramDisableOthers(BIT_PR_3);
        }
        if(pr_4_sw()) {
            while (pr_4_sw()) { RC(); }
            setSpecifiedProgramDisableOthers(BIT_PR_4);
        }
        if(pr_5_sw()) {
            while (pr_5_sw()) { RC(); }
            setSpecifiedProgramDisableOthers(BIT_PR_5);
        }
        if(pr_6_sw()) {
            while (pr_6_sw()) { RC(); }
            setSpecifiedProgramDisableOthers(BIT_PR_6);
        }
        if(pr_7_sw()) {
            while (pr_7_sw()) { RC(); }
            setSpecifiedProgramDisableOthers(BIT_PR_7);
        }
        if(pr_8_sw()) {
            while (pr_8_sw()) { RC(); }
            setSpecifiedProgramDisableOthers(BIT_PR_8);
        }
        
        if(pr_plus_sw()) {
            while (pr_plus_sw()) { RC(); }
            
            if(!bit_is_set(MEM[E_PROGRAMS], BIT_PR_8)){
                MEM[E_PROGRAMS] <<= 1;
            }

            restorePrograms();
            setStoreStatusFlag(true);
        }
        if(pr_minus_sw()) {
            while (pr_minus_sw()) { RC(); }
            
            if(!bit_is_set(MEM[E_PROGRAMS], BIT_PR_1)){
                MEM[E_PROGRAMS] >>= 1;
            }
            
            restorePrograms();
            setStoreStatusFlag(true);
        }
        
    }
}
