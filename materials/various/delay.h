/** \file delay.h \brief Funkcje opóŸniaj¹ce. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** 
 \defgroup delay Funkcje opóŸniaj¹ce
 
 Funkcje wstrzymuj¹ce dzia³anie programu na okreœlony czas.
 
 \code
 #include "delay.h"
 \endcode 
*/
#ifndef __DELAY_H__
#define __DELAY_H__

#include "config.h"
#include "global.h"

/*@{*/

/**
 Wstrzymuje dzia³anie programu na count*us
 Zakres od 1us do 65,535ms.
 \param count opóŸnienie w us 
*/
void delayus(u16 count); 

/**
 Wstrzymuje dzia³anie programu na 10us.
 Czêsto u¿ywane opóŸnienie - minimalizuje u¿ycie 
 pamiêci programu z uwagi na nie przekazywanie parametrów do funkcji.
*/
void delay10us(void);

/**
 Wstrzymuje dzia³anie programu na count*ms
 Zakres od 1ms do 65,535s.
 \param count opóŸnienie w ms 
*/
void delayms(u16 count);

/*@}*/

#endif // __DELAY_H__
