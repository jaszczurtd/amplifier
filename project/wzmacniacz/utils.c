//
//  utils.c
//  Index
//
//  Created by Marcin Kielesiński on 07/12/2019.
//  Copyright © 2019 Commend. All rights reserved.
//

#include "utils.h"

void delay_ms(int ms) {
    while (0 < ms) {
        _delay_ms(1);
        --ms;
    }
}

int strlength(char *s) {
    unsigned char p = 0;
    
    while(*s++)p++;
    return p;
}

int binatoi(char *s) {
    int i,l=0,w=1;
    
    for(i=0; i < strlength(s); i++)
    {
        if (s [i]=='1')
        {
            l+=w;
            w*=2;
        }
        if(s [i]=='0')
        {
            w*=2;
        }
    }
    return(l);
}

