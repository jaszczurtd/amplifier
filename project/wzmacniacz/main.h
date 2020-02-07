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
#include "pcf8574a.h"
#include "ds1267.h"
#include "PCF8563.h"
#include "Inputs.h"
#include "Outputs.h"
#include "clockPart.h"

extern int rc5Code, switchCode;

extern unsigned char MEM[];
extern char s[];

extern void setVolumeChangerTimer(void);

#define MAIN_DELAY_TIME 10          //in ms
#define POWER_RES_COUNTER 100       //* MAIN_DELAY_TIME
#define SPEAKERS_COUNTER 20         //POWER_RES_COUNTER + (* MAIN_DELAY_TIME)
#define READ_COMMANDS_DELAY_TIME (MAIN_DELAY_TIME * 2)

#define POWER_LED_MAX_VALUE 160


#define DEFAULT_POWER_IS_ON true

#define VOL_CHANGE_TIME 55
#define VOL_BAR_HEIGHT 9
#define VOL_BAR_X 0
#define VOL_BAR_Y 20

#ifndef MAIN_H
#define MAIN_H

#endif

