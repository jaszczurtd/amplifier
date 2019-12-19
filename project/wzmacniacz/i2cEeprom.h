//
//  i2cEeprom.h
//  Index
//
//  Created by Marcin Kielesiński on 16/12/2019.
//

#ifndef i2cEeprom_h
#define i2cEeprom_h

#include <stdio.h>
#include <stdbool.h>

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "utils.h"


void UART_Init(void);
void TWI_Init(void);
void EEPROMwrite(unsigned char ucAddress, unsigned char ucData);
unsigned char EEPROMread(unsigned char ucAddress);

#endif /* i2cEeprom_h */
