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

void setClockSetMode(bool enabled);
void clockMainFunction(void);

#endif /* clockPart_h */
