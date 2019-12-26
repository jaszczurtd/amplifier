/*
* RC5 (36KHz Phillips protocol) Decoding library for AVR
* Copyright (c) 2011 Filip Sobalski <pinkeen@gmail.com>
* based on the idea presented by Guy Carpenter
* on http://www.clearwater.com.au/rc5
*
* Tested on ATmega328P. Designed for 16MHz crystal.
* Should work on the ATmega{4/8/16/32}8 family
* without modification. Uses 16bit timer and an
* external interrupt.
*
* I you use a different clock then adjust the timer
* prescaler and pulse lengths accordingly.
*
* Should be trivial to adapt to other AVRs sporting
* a 16bit timer and an external interrupt.
*
*/

#include "rc5.h"

#define SHORT_MIN 888   /* 444 microseconds */
#define SHORT_MAX 2666  /* 1333 microseconds */
#define LONG_MIN 2668   /* 1334 microseconds */
#define LONG_MAX 4444   /* 2222 microseconds */

typedef enum {
    STATE_START1, 
    STATE_MID1, 
    STATE_MID0, 
    STATE_START0, 
    STATE_ERROR, 
    STATE_BEGIN, 
    STATE_END
} State;

///////////////////ADAPTED FOR ATMEGA32 by Hydrawerk 2014//////////////////////
/////////////////Well, I only changed some register names.////////////////////

const uint8_t trans[4] = {0x01, 0x91, 0x9b, 0xfb};
volatile uint16_t command;
uint8_t ccounter;
volatile uint8_t has_new;
State state = STATE_BEGIN;
static int resultCommand = 0;
static int counter = 0;

void RC5_Init(void) {
    /* Set INT0 to trigger on any edge */
    //EICRA |= _BV(ISC00);
    MCUCR |= _BV(ISC00);
    /* Set PD2 to input */
    DDRD &= ~_BV(PD2);
    
    /* Reset Timer1 Counter */
    TCCR1A = 0;
    /* Enable Timer1 in normal mode with /8 clock prescaling */
    /* One tick is 500ns with 16MHz clock */
    TCCR1B = _BV(CS11);
    
    RC5_Reset();
    
    resultCommand = 0;
    counter = 0;
}


void RC5_Reset(void) {
    has_new = 0;
    ccounter = 14;
    command = 0;
    state = STATE_BEGIN;
    
    /* Enable INT0 */
    //EIMSK |= _BV(INT0);
    GICR |= _BV(INT0);
}

int RC5_NewCommandReceived(void) {
    if(has_new &&
       counter == 0 &&
       resultCommand == 0 &&
       command != 0) {
        resultCommand = command &~ 2048;
    }
    
    if(has_new) {
        RC5_Reset();
        counter = RC5_STABILITY_VALUE;
    } else {
        if(counter > 0) {
            counter--;
        } else {
            resultCommand = 0;
        }
    }
    return resultCommand;
}

ISR(INT0_vect) {
    
    uint16_t delay = TCNT1;

    /* TSOP2236 pulls the data line up, giving active low,
     * so the output is inverted. If data pin is high then the edge
     * was falling and vice versa.
     * 
     *  Event numbers:
     *  0 - short space
     *  2 - short pulse
     *  4 - long space
     *  6 - long pulse
     */
    uint8_t event = (PIND & _BV(PIND2)) ? 2 : 0;
    
    if(delay > LONG_MIN && delay < LONG_MAX) {
        event += 4;
    } else if(delay < SHORT_MIN || delay > SHORT_MAX) {
        /* If delay wasn't long and isn't short then
         * it is erroneous so we need to reset but
         * we don't return from interrupt so we don't
         * loose the edge currently detected. */
        RC5_Reset();
    }

    if(state == STATE_BEGIN) {
        ccounter--;
        command |= 1 << ccounter;
        state = STATE_MID1;
        TCNT1 = 0;
        return;
    }
    
    State newstate = (trans[state] >> event) & 0x03;

    if(newstate == state || state > STATE_START0) {
        /* No state change or wrong state means
         * error so reset. */
        RC5_Reset();
        return;
    }
    
    state = newstate;
    
    /* Emit 0 - jest decrement bit position counter
     * cause data is already zeroed by default. */
    if(state == STATE_MID0) {
        ccounter--;
    } else if(state == STATE_MID1) {
        /* Emit 1 */
        ccounter--;
        command |= 1 << ccounter;
    }
    
    /* The only valid end states are MID0 and START1.
     * Mid0 is ok, but if we finish in MID1 we need to wait
     * for START1 so the last edge is consumed. */
    if(ccounter == 0 && (state == STATE_START1 || state == STATE_MID0)) {
        state = STATE_END;
        has_new = 1;
        
        /* Disable INT0 */
        
        //EIMSK &= ~_BV(INT0);
        GICR &= ~_BV(INT0);
    }
    
    TCNT1 = 0;
}

