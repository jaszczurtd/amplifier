//
//  pcf8574a.c
//  Index
//
//  Created by Marcin Kielesiński on 07/02/2020.
//

#include "pcf8574a.h"

void pcf8574writeByte(unsigned char outputs, unsigned char value) {
    
    TWI_Start();
    
    switch(outputs) {
        case PORT_OUTPUTS:
            TWI_Write(PCF8574_WRITE_ADDR_A);
            break;
        case PORT_PROGRAMS:
            TWI_Write(PCF8574_WRITE_ADDR_B);
            break;
        case PORT_VOLUME:
            TWI_Write(PCF8574_WRITE_ADDR_C);
            break;
    }
    TWI_Write(value);
    TWI_Stop();
}

void clearPorts(void) {
    pcf8574writeByte(PORT_OUTPUTS, 0);
    pcf8574writeByte(PORT_PROGRAMS, 0);
    pcf8574writeByte(PORT_VOLUME, 0);
}

