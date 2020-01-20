//
//  74574.h
//  Index
//
//  Created by Marcin Kielesiński on 02/01/2020.
//

#ifndef _4574_h
#define _4574_h

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"

void init74574(void);
void clearPorts(void);
void setPower(bool state);

#endif /* _4574_h */
