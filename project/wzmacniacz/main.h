/**

 */

#include "version.h" // Config file
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


//lcd includes
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <avr/wdt.h>

#include "utils.h"
#include "pcd8544.h"
#include "rc5.h"

typedef unsigned char uint8_t;

#ifndef MAIN_H
#define MAIN_H

#define WR wdt_reset();

#endif

