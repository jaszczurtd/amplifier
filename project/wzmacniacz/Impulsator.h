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
#include "main.h"

#define NONE 0
#define LEFT 1
#define RIGHT 2

#define MIN_VOLUME 0
#define MAX_VOLUME 127 - MIN_VOLUME

void Impulsator_Init(int max);
void Impulsator_decrease(unsigned char stepValue);
void Impulsator_increase(unsigned char stepValue);

void setImpulsatorMaxValue(int value);
int getImpulsatorMaxValue(void);
int getImpulsatorValue(void);
void setImpulsatorValue(int value);
void Read1StepEncoder(void);
const char *getVolumeText(void);

#endif /* Impulsator_h */
