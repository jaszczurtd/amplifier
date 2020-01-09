//
//  utils.h
//  Index
//
//  Created by Marcin Kielesiński on 07/12/2019.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

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

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* compatibilty macros for old style */
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#ifndef bitClear
#define bitClear(dest, bit) dest &= ~(_BV(bit))
#endif

#ifndef bitSet
#define bitSet(dest, bit) dest |= _BV(bit)
#endif

#ifndef boolP
#define boolP(x) (x) ? "true" : "false"
#endif

void delay_ms(int ms);
int strlength(char *s);
int binatoi(char *s);
char *decToBinary(int n);

#endif /* utils_h */
