//
//  Outputs.h
//  Index
//
//  Created by Marcin Kielesiński on 02/02/2020.
//

#ifndef Outputs_h
#define Outputs_h

#include <stdio.h>
#include "utils.h"
#include "pcf8574a.h"
#include "i2cEeprom.h"
#include "pcd8544.h"
#include "adc.h"
#include "main.h"

#define DELAY_BETWEEN_STATES 30     //in ms

#define BIT_GENERIC 0
#define BIT_TAPE 1
#define BIT_PIEZO 2
#define BIT_RADIO 3
#define BIT_DAC 4
#define BIT_LOUDNESS 5
#define BIT_POWER_RES 6
#define BIT_SPEAKERS 7

enum progs {
    BIT_PR_1 = 0, BIT_PR_2, BIT_PR_3, BIT_PR_4, BIT_PR_5, BIT_PR_6, BIT_PR_7, BIT_PR_8
};

enum freq {
    AM = 0, FM
};

#define FM_MIN_FREQUENCY 875
#define FM_MAX_FREQUENCY 1080

#define AM_MIN_FREQUENCY 165
#define AM_MAX_FREQUENCY 285

extern unsigned char MEM[];

extern bool EORBit(unsigned char address);
extern bool speakersFlag;

#if DEBUG_VAL
extern debugval;
#endif

void restoreOutputs(void);
void setSpecifiedOutputDisableOthers(unsigned char bit);
bool checkIfOutputIsActive(unsigned char bit);

bool getLoudness(void);
void setLoudness(unsigned char state);
void setRadio(bool state);
void setDAC(bool state);
void setTape(bool state);
void setPiezo(bool state);
void setGeneric(bool state);
void clearOutputs(void);

void setPowerRes(bool state);
void setSpeakers(bool state);

void restorePrograms(void);
void setProgram(int bit, bool state);
void setSpecifiedProgramDisableOthers(unsigned char bit);
void disableAllPrograms(void);

unsigned char getActiveFreq(void);
const char *getFrequencyValueString(void);
const char *getFrequencyString(void);

#endif /* Outputs_h */
