//
//  Impulsator.h
//  Index
//
//  Created by Marcin Kielesiński on 14/12/2019.
//

#ifndef Impulsator_h
#define Impulsator_h

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "utils.h"

void Impulsator_Init(void);
int getImpulsatorValue(void);

#endif /* Impulsator_h */
