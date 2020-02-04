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

extern unsigned char MEM[];

const char *getOutputString(unsigned char index);

void restoreOutputs(void);

void setLoudness(bool state);
void setRadio(bool state);
void setDAC(bool state);
void setTape(bool state);
void setPiezo(bool state);
void setGeneric(bool state);

void setPowerRes(bool state);
void setSpeakers(bool state);

#endif /* Outputs_h */
