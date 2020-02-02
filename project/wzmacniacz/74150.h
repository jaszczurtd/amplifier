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

#define I_74150_POWER   5
#define I_74150_GEN     0
#define I_74150_TAPE    1
#define I_74150_PIEZO   2
#define I_74150_RADIO   3
#define I_74150_DAC     4

#define I_74150_LOUDNESS 7

#define I_74150_PR_1    8
#define I_74150_PR_2    9
#define I_74150_PR_3    10
#define I_74150_PR_4    11
#define I_74150_PR_5    12
#define I_74150_PR_6    13
#define I_74150_PR_7    14
#define I_74150_PR_8    15


void init74150(void);
int read74150(void);

#endif /* _4150_h */
