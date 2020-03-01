//
//  Inputs.h
//  Index
//
//  Created by Marcin Kielesiński on 02/02/2020.
//

#ifndef Inputs_h
#define Inputs_h

#include <stdio.h>
#include "utils.h"
#include "rc5.h"
#include "74150.h"

#define RC() readCommands(READ_COMMANDS_DELAY_TIME)

void initInputs(void);
void readCommands(int time);
bool power_sw(void);
bool generic_sw(void);
bool tape_sw(void);
bool piezo_sw(void);
bool radio_sw(void);
bool dac_sw(void);
bool loudness_sw(void);

bool pr_1_sw(void);
bool pr_2_sw(void);
bool pr_3_sw(void);
bool pr_4_sw(void);
bool pr_5_sw(void);
bool pr_6_sw(void);
bool pr_7_sw(void);
bool pr_8_sw(void);

void readSelectorInputs(void);
void readRadioInputs(void);
bool pr_plus_sw(void);
bool pr_minus_sw(void);
    
#endif /* Inputs_h */
