/** \file delay.h \brief Funkcje op�niaj�ce. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** 
 \defgroup delay Funkcje op�niaj�ce
 
 Funkcje wstrzymuj�ce dzia�anie programu na okre�lony czas.
 
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
 Wstrzymuje dzia�anie programu na count*us
 Zakres od 1us do 65,535ms.
 \param count op�nienie w us 
*/
void delayus(u16 count); 

/**
 Wstrzymuje dzia�anie programu na 10us.
 Cz�sto u�ywane op�nienie - minimalizuje u�ycie 
 pami�ci programu z uwagi na nie przekazywanie parametr�w do funkcji.
*/
void delay10us(void);

/**
 Wstrzymuje dzia�anie programu na count*ms
 Zakres od 1ms do 65,535s.
 \param count op�nienie w ms 
*/
void delayms(u16 count);

/*@}*/

#endif // __DELAY_H__
