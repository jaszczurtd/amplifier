/*! \file global.h \brief Definicje globalne. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/** \defgroup global Definicje globalne
 
 Definicje globalne dla ca�ej biblioteki i programu napisanego z jej wykorzystaniem
 znajduj� si� w pliku \c "global.h" jednak \b NIE \b NALE�Y \b GO \b W��CZA�
 do programu bezpo�rednio poniewa� jest on w��czany przez dowolny plik nag��wkowy biblioteki.
 Zamiast tego nale�y w katalogu projektu nale�y umie�ci� plik \c "config.h" 
 z odpowiedni� zawarto�ci� modyfikuj�c� domy�lne ustawienia biblioteki.

 \ref config
*/
 
#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/*@{*/

#ifndef F_CPU
/**
 Cz�stotliwo�� zegara mikrokontrolera w Hz.
 Mo�na j� zmieni� w pliku "config.h" projektu.
*/
#define F_CPU		8000000
#endif

#ifndef UART_BAUD
/**
 Pr�dko�� transmisji przez UART w bit/s.
 Mo�na zmieni� w pliku "config.h" projektu.
*/
#define UART_BAUD	19200
#endif

#ifndef UART_MAX_GETSTR
/**
 Maksymalna d�ugo�� tekstu dla UART_getstr().
 Mo�na zmieni� w pliku "config.h" projektu.
*/
#define UART_MAX_GETSTR 8
#endif

#ifndef UART_CONST
/**
 Warto�� dla rejestru odpowiedzialnego za pr�dko�� transmisji przez UART.
 Zale�y od F_CPU i UART_BAUD.
*/
#define UART_CONST	(F_CPU/(UART_BAUD*16L)-1)
#endif

/**
 Je�li istnieje definicja \c WDT_ENABLE w pliku \c "config.h" projektu 
 informuj�ca bibliotek� o wykorzystaniu watchdoga to przypisuje do 
 \c WDR() funkcj� \c wdt_reset() zdefiniowan� w \c <avr/wdt.h>, 
 je�li nie to \c WDR() jest pustym ci�giem.
 W ca�ym projekcie nale�y u�ywa� makra \c WDR() do restartowania
 licznika watchdoga zamiast \c wdt_reset() - upraszcza to modyfikacje kodu.
*/
#ifdef WDT_ENABLE
#define WDR()	wdt_reset()
#else 
#define WDR()
#endif

/**
 Instrukcja maszynowa \b nop - nic nie robi tylko zu�ywa jeden cykl maszynowy.
 Czasami (a nawet cz�sto) bywa przydatna.
*/
#define NOP() __asm__ __volatile__ ("nop")

#ifndef CYCLES_PER_US
/**
 Liczba cykli CPU na mikrosekund�. Wykorzystywana przez funkcje op�niaj�ce.
 Zale�y od F_CPU.
*/
#define CYCLES_PER_US ((F_CPU+500000)/1000000)
#endif

#ifndef GCC_VERSION
/**
 Zwraca warto�� zale�n� od wersji kompilatora.
*/
#define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif

#ifndef cbi
/**
 Zmienia stan bitu BIT w porcie PORT na 0
*/
#define cbi(PORT, BIT) (_SFR_BYTE(PORT) &= ~_BV(BIT))
#endif

#ifndef sbi
/**
 Zmienia stan bitu BIT w porcie PORT na 1
*/
#define sbi(PORT, BIT) (_SFR_BYTE(PORT) |= _BV(BIT))
#endif

#ifndef tbi
/**
 Prze��cza stan bitu BIT w porcie PORT na przeciwny 1->0 ; 0->1
*/
#define tbi(PORT, BIT)	(_SFR_BYTE(PORT) ^= _BV(BIT))
#endif

/**
 Adres rejestru kierunkowego.
 Jako argument nale�y poda� nazw� rejestru wyj�ciowego portu PORTx.
*/
#define DDR(x) (_SFR_IO8(_SFR_IO_ADDR(x)-1))

/**
 Adres rejestru wejsciowego.
 Jako argument nale�y poda� nazw� rejestru wyj�ciowego portu PORTx.
*/
#define PIN(x) (_SFR_IO8(_SFR_IO_ADDR(x)-2))

/**
 Upraszcza definiowanie zmiennych w pami�ci EEPROM.
*/
#define EEPROM __attribute__((section(".eeprom")))

/**
 Tworzy �a�cuch w pami�ci EEPROM i zwraca wska�nik do niego.
*/
#define ESTR(s) ({static char __c[] EEPROM = (s); __c;})

#ifndef TRUE

/**
 Prawda logiczna.
*/
#define TRUE	1
#endif

#ifndef FALSE
/**
 Fa�sz logiczny.
*/
#define FALSE	0
#endif

/*
 * podmiana nazw rejestr�w dla r�nych uk�ad�w AVR
 */

// ---------------------------------------------------------------

/*
 * _UBRR_
 */
#ifdef	UBRR
#define _UBRR_	UBRR
#endif

#ifdef	UBRR0
#define _UBRR_	UBRR0
#endif

#ifdef	UBRR0L
#define _UBRR_	UBRR0L
#endif

// ---------------------------------------------------------------

/*
 * _UBRRH_
 */
#ifdef	UBRRH
#define _UBRRH_	UBRRH
#endif

#ifdef	UBRR0H
#define _UBRRH_	UBRR0H
#endif

#ifdef	UBRRHI
#define _UBRRH_	UBRRHI
#endif

// ---------------------------------------------------------------

/*
 * _UDR_
 */
#ifdef	UDR
#define _UDR_	UDR
#endif

#ifdef	UDR0
#define _UDR_	UDR0
#endif

// ---------------------------------------------------------------

/*
 * _UCR_
 */
#ifdef	UCR
#define _UCR_	UCR
#endif

#ifdef	UCSRB
#define _UCR_	UCSRB
#endif

#ifdef	UCSR0B
#define _UCR_	UCSR0B
#endif

// ---------------------------------------------------------------

/*
 * _USR_
 */
#ifdef	USR
#define _USR_	USR
#endif

#ifdef	UCSRA
#define _USR_	UCSRA
#endif

#ifdef	UCSR0A
#define _USR_	UCSR0A
#endif

// ---------------------------------------------------------------

#ifdef SIG_UART0_RECV
#define SIG_UART_RECV          SIG_UART0_RECV
#endif

#ifdef SIG_UART0_DATA
#define SIG_UART_DATA          SIG_UART0_DATA
#endif

#ifdef SIG_UART0_TRANS
#define SIG_UART_TRANS         SIG_UART0_TRANS
#endif

// ---------------------------------------------------------------

typedef unsigned char      u08;	///< Typ 8 bitowy bez znaku
typedef          char      s08;	///< Typ 8 bitowy ze znakiem
typedef unsigned short     u16;	///< Typ 16 bitowy bez znaku
typedef          short     s16;	///< Typ 16 bitowy ze znakiem
typedef unsigned long      u32;	///< Typ 32 bitowy bez znaku
typedef          long      s32;	///< Typ 32 bitowy ze znakiem
typedef unsigned long long u64;	///< Typ 64 bitowy bez znaku
typedef          long long s64;	///< Typ 64 bitowy ze znakiem

// ---------------------------------------------------------------

typedef unsigned char      UCHAR;	///< Typ 8 bitowy bez znaku
typedef unsigned short     WORD;	///< Typ 16 bitowy bez znaku
typedef unsigned long      DWORD;	///< Typ 32 bitowy bez znaku
typedef char*		     LPCTSTR;	///< Wska�nik do �a�cucha znak�w		
// ---------------------------------------------------------------

/// Typ przechowuj�cy czas
typedef struct	
{
  u08 second;   ///< Sekunda
  u08 minute;   ///< Minuta
  u08 hour;     ///< Godzina
  u08 day;	///< Dzie� miesi�ca
  u08 month;    ///< Miesi�c
  u16 year;     ///< Rok
} struct_time;      

// ---------------------------------------------------------------

/// Typ przechowuj�cy bajty s�owa 16 bitowego
struct u16bytes
{
	u08 low, high;
};

// ---------------------------------------------------------------

/// Unia dla u�atwienia konwersji bajt�w s�owa 16 bitowego
union u16convert
{
	u16 value;
	struct u16bytes bytes;
};

/*@}*/

#endif // __GLOBAL_H__

