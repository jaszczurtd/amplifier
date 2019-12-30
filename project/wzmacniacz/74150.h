//
//  74150.h
//  Index
//
//  Created by Marcin Kielesiński on 30/12/2019.
//

#ifndef _4150_h
#define _4150_h

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "utils.h"

#define I_74150_NOT_ACTIVE -1

void init74150(void);
int read74150(void);

#endif /* _4150_h */
