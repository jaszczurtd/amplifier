//
//  clockPart.h
//  Index
//
//  Created by Marcin Kielesiński on 27/01/2020.
//

#ifndef clockPart_h
#define clockPart_h

#include <stdio.h>
#include "main.h"

extern int rc5Code, switchCode;
extern bool powerIsOn;
extern char s[];

void setClockSetMode(bool enabled);
void clockMainFunction(void);

void startTimerForSeconds(unsigned char seconds);
bool checkIfTimerReached(void);


#endif /* clockPart_h */
