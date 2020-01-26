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

#include "twi_i2c.h"
#include "utils.h"

#define EEPROM_ADDR_WR 0xa0
#define EEPROM_ADDR_RD 0xa1

bool EEPROMwrite(unsigned char ucAddress, unsigned char ucData);
unsigned char EEPROMread(unsigned char ucAddress);

#endif /* i2cEeprom_h */
