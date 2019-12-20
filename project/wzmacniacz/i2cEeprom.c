//
//  i2cEeprom.c
//  Index
//
//  Created by Marcin Kielesiński on 16/12/2019.
//

#include "i2cEeprom.h"

void TWI_Init(void) {
    TWSR &= (~((1<<TWPS1)|(1<<TWPS0)));        // Preskaler = 1  ->> TWPS1=0 TWPS0=0
    TWBR = (((F_CPU / 100000) - 16) / 2);      // ((Fclk/Ftwi)-16)/2
}

bool EEPROMwrite(unsigned char ucAddress, unsigned char ucData) {
    do {
        //Put Start Condition on TWI Bus
        TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

        //Poll Till Done
        while(!(TWCR & (1<<TWINT)));

        //Check status
        if((TWSR & 0xF8) != 0x08)
            return FALSE;

        //Now write SLA+W
        //EEPROM @ 00h
        TWDR=0b10100000;

        //Initiate Transfer
        TWCR=(1<<TWINT)|(1<<TWEN);

        //Poll Till Done
        while(!(TWCR & (1<<TWINT)));
    
    } while((TWSR & 0xF8) != 0x18);

    //Now write ADDRH
    TWDR=(ucAddress>>8);

    //Initiate Transfer
    TWCR=(1<<TWINT)|(1<<TWEN);

    //Poll Till Done
    while(!(TWCR & (1<<TWINT)));

    //Check status
    if((TWSR & 0xF8) != 0x28)
        return FALSE;

    //Now write ADDRL
    TWDR=(ucAddress);

    //Initiate Transfer
    TWCR=(1<<TWINT)|(1<<TWEN);

    //Poll Till Done
    while(!(TWCR & (1<<TWINT)));

    //Check status
    if((TWSR & 0xF8) != 0x28)
        return FALSE;

    //Now write DATA
    TWDR=(ucData);

    //Initiate Transfer
    TWCR=(1<<TWINT)|(1<<TWEN);

    //Poll Till Done
    while(!(TWCR & (1<<TWINT)));

    //Check status
    if((TWSR & 0xF8) != 0x28)
        return FALSE;

    //Put Stop Condition on bus
    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    
    //Wait for STOP to finish
    while(TWCR & (1<<TWSTO));

    //Wait untill Writing is complete
    _delay_ms(12);
    return TRUE;
}

unsigned char EEPROMread(unsigned char ucAddress) {
    
    uint8_t data;
    
    //Initiate a Dummy Write Sequence to start Random Read
    do {
        //Put Start Condition on TWI Bus
        TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

        //Poll Till Done
        while(!(TWCR & (1<<TWINT)));

        //Check status
        if((TWSR & 0xF8) != 0x08)
            return FALSE;

        //Now write SLA+W
        //EEPROM @ 00h
        TWDR=0b10100000;

        //Initiate Transfer
        TWCR=(1<<TWINT)|(1<<TWEN);

        //Poll Till Done
        while(!(TWCR & (1<<TWINT)));
    
    }while((TWSR & 0xF8) != 0x18);
        

    //Now write ADDRH
    TWDR=(ucAddress>>8);

    //Initiate Transfer
    TWCR=(1<<TWINT)|(1<<TWEN);

    //Poll Till Done
    while(!(TWCR & (1<<TWINT)));

    //Check status
    if((TWSR & 0xF8) != 0x28)
        return FALSE;

    //Now write ADDRL
    TWDR=(ucAddress);

    //Initiate Transfer
    TWCR=(1<<TWINT)|(1<<TWEN);

    //Poll Till Done
    while(!(TWCR & (1<<TWINT)));

    //Check status
    if((TWSR & 0xF8) != 0x28)
        return FALSE;

    //*************************DUMMY WRITE SEQUENCE END **********************


    
    //Put Start Condition on TWI Bus
    TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

    //Poll Till Done
    while(!(TWCR & (1<<TWINT)));

    //Check status
    if((TWSR & 0xF8) != 0x10)
        return FALSE;

    //Now write SLA+R
    //EEPROM @ 00h
    TWDR=0b10100001;

    //Initiate Transfer
    TWCR=(1<<TWINT)|(1<<TWEN);

    //Poll Till Done
    while(!(TWCR & (1<<TWINT)));

    //Check status
    if((TWSR & 0xF8) != 0x40)
        return FALSE;

    //Now enable Reception of data by clearing TWINT
    TWCR=(1<<TWINT)|(1<<TWEN);

    //Wait till done
    while(!(TWCR & (1<<TWINT)));

    //Check status
    if((TWSR & 0xF8) != 0x58)
        return FALSE;

    //Read the data
    data=TWDR;

    //Put Stop Condition on bus
    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    
    //Wait for STOP to finish
    while(TWCR & (1<<TWSTO));

    //Return TRUE
    return data;
}
