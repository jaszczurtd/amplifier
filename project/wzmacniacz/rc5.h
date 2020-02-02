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

#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

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

#define RC5_STABILITY_VALUE 8

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

#define RC5_POWER           12300
#define RC5_VOLUME_UP       12304
#define RC5_VOLUME_DOWN     12305
#define RC5_CLOCK_SET       8250
#define RC5_MENU            8210
#define RC5_MENU_MINUS      8206
#define RC5_MENU_PLUS       8205

#define RC5_RADIO           12303
#define RC5_DAC             12348
#define RC5_GEN             12332
#define RC5_TAPE            12329
#define RC5_PIEZO           12331

#define RC5_PR_1            12289
#define RC5_PR_2            12290
#define RC5_PR_3            12291
#define RC5_PR_4            12292
#define RC5_PR_5            12293
#define RC5_PR_6            12294
#define RC5_PR_7            12295
#define RC5_PR_8            12296

#define RC5_LOUDNESS        12302

#define RC5_MUTE            12301

#define RC5_CHANNEL_PLUS    12320
#define RC5_CHANNEL_MINUS   12321


#endif

