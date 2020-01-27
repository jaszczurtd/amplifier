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
#include "twi_i2c.h"
#include "pcd8544.h"
#include "rc5.h"
#include "Impulsator.h"
#include "i2cEeprom.h"
#include "UART.h"
#include "ds18b20.h"
#include "adc.h"
#include "pwm.h"
#include "74150.h"
#include "74574.h"
#include "ds1267.h"
#include "PCF8563.h"


typedef unsigned char uint8_t;

#define MAIN_DELAY_TIME 10 //in ms
#define POWER_RES_COUNTER 100 //* MAIN_DELAY_TIME
#define SPEAKERS_COUNTER 20      //POWER_RES_COUNTER + (* MAIN_DELAY_TIME)

#define WRITE_EEPROM_DELAY 10

#ifndef MAIN_H
#define MAIN_H

#define WR wdt_reset

#define E_VOLUME 0

#endif

