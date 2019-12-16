//
//  Impulsator.c
//  Index
//
//  Created by Marcin Kielesiński on 14/12/2019.
//

#include "Impulsator.h"

//experimental value
#define DETERMINATION_TIME 250

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
    
    pulses = 0;
    delta = NONE;
    determined = false;
    stepValue = 1;
    stepValueCounter = 0;
    
    setImpulsatorMaxValue(max);
}

void setImpulsatorMaxValue(int value) {
    maxValue = value;
}

void setImpulsatorStep(int step) {
    stepValue = step;
}

void Read1StepEncoder(void) {
    
    bool left = getImpulsatorLSW();
    bool right = getImpulsatorRSW();
    
    bool movement = true;
    if(left == lastLeft && right == lastRight) {
        movement = false;
    }
    lastLeft = left;
    lastRight = right;

    if(determined) {
        pulses--;
        if(pulses <= 0) {
            determined = false;
            delta = NONE;
            return;
        }
        if(delta == RIGHT && movement) {
            if(currentValue < maxValue) {
                if(stepValueCounter++ > stepValue){
                    currentValue++;
                    stepValueCounter = 0;
                }
            }
            pulses = DETERMINATION_TIME;
        }
        if(delta == LEFT && movement) {
            if(currentValue > 0) {
                if(stepValueCounter++ > stepValue){
                    currentValue--;
                    stepValueCounter = 0;
                }
            }
            pulses = DETERMINATION_TIME;
        }
    } else {
        
        if(left && !right) {
            delta = LEFT;
            determined = true;
            pulses = DETERMINATION_TIME;
        }
        if(!left && right) {
            delta = RIGHT;
            determined = true;
            pulses = DETERMINATION_TIME;
        }
    }
}

ISR(TIMER2_OVF_vect) {
    Read1StepEncoder();
}

int getImpulsatorValue(void) {
    return currentValue;
}
