/*
  file i2c.h brief Programowa obs�uga magistrali i2c w trybie master. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/* defgroup i2c Magistrala I2C
 
 Programowa obs�uga magistrali i2c w trybie master.
 
  code
 #include "i2c.h"
  endcode 
 
 Magistrala I2C jest standardow� magistral� szeregow� u�ywan� do komunikacji pomi�dzy uk�adami scalonymi. 
 Pozwala na du�e uproszczenie uk�adu - tylko dwie linie SDA - dane i SCL - zegar.
 Tylko niekt�re uk�ady AVR posiadaj� sprz�towe wspomaganie dla tej magistrali.
 Biblioteka ta powsta�a aby umo�liwi� pozosta�ym uk�adom AVR na korzystanie
 z mn�stwa uk�ad�w komunikuj�cych si� ze �wiatem zewn�trznym za pomoc� tej magistrali
 (pami�ci EEPROM, RAM, zegary czasu rzeczywistego, przetworniki AC i CA itp.). 
 
  ref config:
 
  W pliku "config.h" znajduj�cym si� w katalogu projektu nale�y umie�ci� nast�puj�c� zawarto��:
    	     
  code
#define I2C_PORT	PORTC	//port do kt�rego s� pod��czone linie magistrali
#define I2C_SDA		1	//bit powy�szego portu u�ywany jako linia SDA
#define I2C_SCL 	0	//bit powy�szego portu u�ywany jako linia SCL
  endcode
 
  note Warto�ci z #define mo�na zmienia� w celu dostosowania 
  biblioteki do tworzonego urz�dzenia i oprogramowania.
 
  par Wykorzytywane biblioteki:
 
  ref delay
*/

#ifndef __I2C_H__
#define __I2C_H__

#include <avr/io.h>
#include "config.h"
#include "global.h"
#include "delay.h"

#define I2C_PORT_O	I2C_PORT
#define	I2C_PORT_D	DDR(I2C_PORT)
#define I2C_PORT_I	PIN(I2C_PORT)

#define I2C_SDA_WR()	sbi(I2C_PORT_D,I2C_SDA)
#define I2C_SDA_RD()	cbi(I2C_PORT_D,I2C_SDA)
#define I2C_SCL_WR()	sbi(I2C_PORT_D,I2C_SCL)
#define I2C_SCL_RD()	cbi(I2C_PORT_D,I2C_SCL)

#define I2C_SDA_H()	sbi(I2C_PORT_O,I2C_SDA)
#define I2C_SDA_L()	cbi(I2C_PORT_O,I2C_SDA)
#define I2C_SCL_H()	sbi(I2C_PORT_O,I2C_SCL)
#define I2C_SCL_L()	cbi(I2C_PORT_O,I2C_SCL)


/***************************************************/
/*	      Procedury obs�ugi interfejsu I2C		   */
/***************************************************/

/*
 Wysy�a bajt na magistral�
  param:  b bajt do wys�ania
  return: potwierdzenie z magistrali (warto�� >0) lub 0 w przypadku niepowodzenia
*/
u08 I2C_write(u08 b);

/*
 Czyta bajt z magistrali i wysy�a potwierdzenie (ACK) na magistral�
  param:  ack warto�� bitu potwierdzenia (1 lub 0)
  return: odczytany bajt z magistrali
*/
u08 I2C_read(u08 ack);

/*
 Wysy�a sekwencj� START na magistral�
*/
void I2C_start(void);

/*
 Wysy�a sekwencj� STOP na magistral�
*/
void I2C_stop(void);


#endif // __I2C_H__
