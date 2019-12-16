//
//  tmf_impulsator.c
//  Index
//
//  Created by ministerq on 16/12/2019.
//  Copyright © 2019 Commend. All rights reserved.
//

#include <stdio.h>

void tmf_impulsator(void)
{
   uint8_t i=0;
   static uint8_t enc_last;

    if(bit_is_clear(PINC,4)) i++;//sprawdzenie czy impulsator się ruszył
    if(bit_is_clear(PINC,5)) i^=3; // convert gray to binary- tak samo jak na bramce XOR
   i-=enc_last;
    enc_last+=i;

    if(i & 1)
    { // Na bicie 0 sprawdzamy czy coś się zmieniło //bit 0 = value (1)
       if(i & 2)// na bicie 1 sprawdzamy kierunek//bit 1 = direction (+/-)
      {
      NoOfPulses++;
      if (NoOfPulses>92) NoOfPulses=0;//pozycji jest 24 a potem od pozycji 0
      PORTB=0xfe;//sygnalizacja kierunku- prawo
       }
      else
      {
      NoOfPulses--;
      if (NoOfPulses<0) NoOfPulses=92-NoOfPulses;//a jak w lewo to też ma podawać pozycję
      PORTB=0xfd;//sygnalizacja kierunku- lewo
       }
   }
} 
