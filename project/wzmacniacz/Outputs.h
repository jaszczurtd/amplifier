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

#define DELAY_BETWEEN_STATES 30     //in ms

#define BIT_GENERIC 0
#define BIT_TAPE 1
#define BIT_PIEZO 2
#define BIT_RADIO 3
#define BIT_DAC 4
#define BIT_LOUDNESS 5
#define BIT_POWER_RES 6
#define BIT_SPEAKERS 7

extern unsigned char MEM[];

const char *getOutputDisplayString(void);
const char *getOutputString(unsigned char bit);
extern bool EORBit(unsigned char address);

void restoreOutputs(void);
void setSpecifiedOutputDisableOthers(unsigned char bit);

bool getLoudness(void);
void setLoudness(bool state);
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

#endif /* Outputs_h */
