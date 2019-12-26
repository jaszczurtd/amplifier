//
//  Impulsator.h
//  Index
//
//  Created by Marcin Kielesiński on 14/12/2019.
//

#ifndef Impulsator_h
#define Impulsator_h

#include <stdio.h>
#include <stdbool.h>

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "utils.h"

#define NONE 0
#define LEFT 1
#define RIGHT 2

void Impulsator_Init(int max);
void Impulsator_decrease(void);
void Impulsator_increase(void);

void setImpulsatorMaxValue(int value);
void setImpulsatorStep(int step);
int getImpulsatorValue(void);
void Read1StepEncoder(void);

#endif /* Impulsator_h */
