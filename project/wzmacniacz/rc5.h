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
#ifndef RC5_H
#define RC5_H

#include <stdint.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "utils.h"

//pin configuration

#define RC5_DDR DDRD
#define RC5_PD PD2
#define RC5_PIN PIND
#define RC5_PIN_N PIND2

#define RC5_GetStartBits(command) ((command & 0x3000) >> 12)
#define RC5_GetToggleBit(command) ((command & 0x800) >> 11)
#define RC5_GetAddressBits(command) ((command & 0x7C0) >> 6)
#define RC5_GetCommandBits(command) (command & 0x3F)
#define RC5_GetCommandAddressBits(command) (command & 0x7FF)

#define RC5_STABILITY_VALUE 10

/* Initialize timer and interrupt */
void RC5_Init(void);

/* Reset the library back to waiting-for-start state */
void RC5_Reset(void);

/* Poll the library for new command.
 * 
 * You should call RC5_Reset immediately after
 * reading the new command because it's halted once 
 * receiving a full command to ensure you can read it
 * before it becomes overwritten. If you expect that only 
 * one remote at a time will be used then library
 * should be polled at least once per ~150ms to ensure
 * that no command is missed.
 */
int RC5_NewCommandReceived(void);

#endif

