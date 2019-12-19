/**

 */

#include "version.h" // Config file
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/wdt.h>

#include "utils.h"
#include "pcd8544.h"
#include "rc5.h"
#include "Impulsator.h"
#include "i2cEeprom.h"
#include "UART.h"

typedef unsigned char uint8_t;

#define MAIN_DELAY_TIME 10 //in ms
#define POWER_RES_COUNTER 100 //* MAIN_DELAY_TIME
#define SPEAKERS_COUNTER 20      //POWER_RES_COUNTER + (* MAIN_DELAY_TIME)

#ifndef MAIN_H
#define MAIN_H

#define WR wdt_reset();

#endif

