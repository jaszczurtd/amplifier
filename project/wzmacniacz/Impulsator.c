//
//  Impulsator.c
//  Index
//
//  Created by Marcin Kielesiński on 14/12/2019.
//

#include "Impulsator.h"

//experimental value
#define DETERMINATION_TIME 150

static int currentValue, maxValue, stepValue, stepValueCounter;
static bool determined = false;
static int pulses = 0;
static char delta = NONE;
static bool lastLeft = false, lastRight = false;
static char preDetermination = NONE;

bool getImpulsatorLSW(void) {
    return bit_is_clear(PINC, PC6);
}

bool getImpulsatorRSW(void) {
    return bit_is_clear(PINC, PC7);
}

void determination(int deltaTo) {
    switch(deltaTo) {
        case LEFT:
        case RIGHT:
            delta = deltaTo;
            determined = true;
            pulses = DETERMINATION_TIME;
            break;
        case NONE:
            preDetermination = NONE;
            determined = false;
            delta = NONE;
            pulses = 0;
            break;
    }
}

void Impulsator_Init(int max) {

    bitClear(TCCR2, CS22);
    bitSet(TCCR2, CS21);
    bitClear(TCCR2, CS20);
    
    bitSet(TIMSK, TOIE2);
    
    TCNT2 = 0;

    cbi(DDRC, PC6);
    cbi(DDRC, PC7);

    lastLeft = getImpulsatorLSW();
    lastRight = getImpulsatorRSW();
    
    determination(NONE);
    
    stepValue = 0;
    stepValueCounter = 0;
    
    setImpulsatorMaxValue(max);
}

void setImpulsatorMaxValue(int value) {
    maxValue = value;
}

int getImpulsatorMaxValue(void) {
    return maxValue;
}

void setImpulsatorStep(int step) {
    stepValue = step;
}

void Impulsator_decrease(void) {
    setVolumeChangerTimer();
    if(currentValue > 0) {
        if(stepValueCounter++ > stepValue){
            currentValue--;
            stepValueCounter = 0;
        }
    }
    pulses = DETERMINATION_TIME;
}

void Impulsator_increase(void) {
    setVolumeChangerTimer();
    if(currentValue < maxValue) {
        if(stepValueCounter++ > stepValue){
            currentValue++;
            stepValueCounter = 0;
        }
    }
    pulses = DETERMINATION_TIME;
}

static int last = 0;
void Read1StepEncoder(void) {
    
    register bool left = getImpulsatorLSW();
    register bool right = getImpulsatorRSW();
    register bool movement = true;
    
    int i = 0;

    if(left == lastLeft && right == lastRight) {
        movement = false;
    }
    lastLeft = left;
    lastRight = right;
    
    if(movement) {
        
        if(preDetermination == NONE){
            if(left && !right) {
                preDetermination = LEFT;
            }
            if(!left && right) {
                preDetermination = RIGHT;
            }
        }
        
        if(left) i++;
        if(right) i ^= 3;
        i -= last;
        last += i;

        if(delta == NONE) {
            if(i & 1) {
                if(i & 2) {
                    if(preDetermination == RIGHT){
                        determination(RIGHT);
                    }
                } else {
                    if(preDetermination == LEFT){
                        determination(LEFT);
                    }
                }
            }
        }
    }

    if(determined) {
        if(pulses-- <= 0) {
            determination(NONE);
        }
        if(movement){
            if(delta == RIGHT) {
                Impulsator_increase();
            }
            if(delta == LEFT) {
                Impulsator_decrease();
            }
        }
    }
    
 }

ISR(TIMER2_OVF_vect) {
    Read1StepEncoder();
}

int getImpulsatorValue(void) {
    return currentValue;
}

void setImpulsatorValue(int value) {
    if(value > maxValue) {
        currentValue = maxValue;
    } else {
        currentValue = value;
    }
}

static char vt[16];
const char *getVolumeText(void) {
    snprintf(vt, sizeof(vt), "Glosnosc: %d" , getImpulsatorValue());
    return vt;
}
