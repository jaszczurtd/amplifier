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

#define WRITE_EEPROM_DELAY 10

#define EEPROM_SIZE 15

#define E_RADIO     0
#define E_DAC       1
#define E_GENERIC   2
#define E_TAPE      3
#define E_PIEZO     4

#define E_VOLUME    5
#define E_LOUDNESS  6

#define E_PR_1      7
#define E_PR_2      8
#define E_PR_3      9
#define E_PR_4      10
#define E_PR_5      11
#define E_PR_6      12
#define E_PR_7      13
#define E_PR_8      14

bool EEPROMwrite(unsigned char ucAddress, unsigned char ucData);
unsigned char EEPROMread(unsigned char ucAddress);

extern void setStoreStatusFlag(void);
extern bool EORValue(unsigned char address);

void storeStatusToEEPROM(void);
void restoreStatusFromEEPROM(void);

#endif /* i2cEeprom_h */
