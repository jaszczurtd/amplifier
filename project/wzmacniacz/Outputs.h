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
#include "74574.h"
#include "i2cEeprom.h"
#include "pcd8544.h"

#define DELAY_BETWEEN_STATES 30     //in ms

extern unsigned char MEM[];

const char *getOutputDisplayString(void);
const char *getOutputString(unsigned char index);

void restoreOutputs(void);
void setSpecifiedOutputDisableOthers(unsigned char index);

void setLoudness(bool state);
void setRadio(bool state);
void setDAC(bool state);
void setTape(bool state);
void setPiezo(bool state);
void setGeneric(bool state);

void setPowerRes(bool state);
void setSpeakers(bool state);

void restorePrograms(void);
void setProgram(int index, bool state);
void setSpecifiedProgramDisableOthers(unsigned char index);
void disableAllPrograms(void);

#endif /* Outputs_h */
