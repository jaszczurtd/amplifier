//
//  utils.h
//  Index
//
//  Created by Marcin Kielesiński on 07/12/2019.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

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

void delay_ms(int ms);
int strlength(char *s);
int binatoi(char *s);
char *decToBinary(int n);

#endif /* utils_h */
