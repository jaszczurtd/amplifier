//
//  ds1267.h
//  Index
//
//  Created by ministerq on 21/12/2019.
//  Copyright © 2019 Commend. All rights reserved.
//

#ifndef ds1267_h
#define ds1267_h

#include <stdio.h>
#include <stdbool.h>

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"

#define C_DELAY 4

#define DS_PORT PORTC
#define RST_PIN PC4
#define DQ_PIN PC2
#define CLK_PIN PC3

void initDS1267(void);
void setDS1267(uint8_t Stack, uint8_t Pot0, uint8_t Pot1);

#endif /* ds1267_h */
