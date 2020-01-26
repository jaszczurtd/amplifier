//
//  utils.c
//  Index
//
//  Created by Marcin Kielesiński on 07/12/2019.
//

#include "utils.h"

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

static char binaryNum[16 + 1];
char *decToBinary(int n) {
    // array to store binary number
    int a = 0, c, k;
    
    memset(binaryNum, 0, sizeof(binaryNum));
    
    for (c = 15; c >= 0; c--) {
        k = n >> c;
        
        if (k & 1) {
            binaryNum[a++] = '1';
        } else {
            binaryNum[a++] = '0';
        }
    }
    return binaryNum;
}
