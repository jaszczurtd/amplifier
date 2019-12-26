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
static uint16_t pulses = 0;
static uint8_t delta = NONE;
static bool lastLeft = false, lastRight = false;

bool getImpulsatorLSW(void) {
    return bit_is_clear(PIND, PD0);
}

bool getImpulsatorRSW(void) {
    return bit_is_clear(PIND, PD1);
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

    cbi(DDRD,PD0);
    cbi(DDRD,PD1);

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

void setImpulsatorStep(int step) {
    stepValue = step;
}

void decrease(void) {
    if(currentValue > 0) {
        if(stepValueCounter++ > stepValue){
            currentValue--;
            stepValueCounter = 0;
        }
    }
    pulses = DETERMINATION_TIME;
}

void increase(void) {
    if(currentValue < maxValue) {
        if(stepValueCounter++ > stepValue){
            currentValue++;
            stepValueCounter = 0;
        }
    }
    pulses = DETERMINATION_TIME;
}

int last = 0;
void Read1StepEncoder(void) {
    
    bool left = getImpulsatorLSW();
    bool right = getImpulsatorRSW();
    int i = 0;

    bool movement = true;
    if(left == lastLeft && right == lastRight) {
        movement = false;
    }
    lastLeft = left;
    lastRight = right;
    
    if(determined) {
        if(pulses-- <= 0) {
            determination(NONE);
        }
        if(movement){
            if(delta == RIGHT) {
                increase();
            }
            if(delta == LEFT) {
                decrease();
            }
        }
    }
    
    if(movement) {
        
        if(left) i++;
        if(right) i ^= 3;
        i -= last;
        last += i;

        if(delta == NONE) {
            if(i & 1) {
                if(i & 2) {
                    determination(RIGHT);
                } else {
                    determination(LEFT);
                }
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
