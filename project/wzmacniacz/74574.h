//
//  74574.h
//  Index
//
//  Created by Marcin Kielesiński on 02/01/2020.
//

#ifndef _4574_h
#define _4574_h

#include <stdio.h>
#include "utils.h"

#define PORT_OUTPUTS 0
#define PORT_PROGRAMS 1

void init74574(void);
void clearPorts(void);
void clockPort(unsigned char portNumber, bool state);



#endif /* _4574_h */
