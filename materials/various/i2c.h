/*
  file i2c.h brief Programowa obs³uga magistrali i2c w trybie master. */
/*
  Copyright (c) 2004 Robert Krysztof
  robert.krysztof@wp.pl
  http://www.avr-gcc.prv.pl
*/
/* defgroup i2c Magistrala I2C
 
 Programowa obs³uga magistrali i2c w trybie master.
 
  code
 #include "i2c.h"
  endcode 
 
 Magistrala I2C jest standardow¹ magistral¹ szeregow¹ u¿ywan¹ do komunikacji pomiêdzy uk³adami scalonymi. 
 Pozwala na du¿e uproszczenie uk³adu - tylko dwie linie SDA - dane i SCL - zegar.
 Tylko niektóre uk³ady AVR posiadaj¹ sprzêtowe wspomaganie dla tej magistrali.
 Biblioteka ta powsta³a aby umo¿liwiæ pozosta³ym uk³adom AVR na korzystanie
 z mnóstwa uk³adów komunikuj¹cych siê ze œwiatem zewnêtrznym za pomoc¹ tej magistrali
 (pamiêci EEPROM, RAM, zegary czasu rzeczywistego, przetworniki AC i CA itp.). 
 
  ref config:
 
  W pliku "config.h" znajduj¹cym siê w katalogu projektu nale¿y umieœciæ nastêpuj¹c¹ zawartoœæ:
    	     
  code
#define I2C_PORT	PORTC	//port do którego s¹ pod³¹czone linie magistrali
#define I2C_SDA		1	//bit powy¿szego portu u¿ywany jako linia SDA
#define I2C_SCL 	0	//bit powy¿szego portu u¿ywany jako linia SCL
  endcode
 
  note Wartoœci z #define mo¿na zmieniaæ w celu dostosowania 
  biblioteki do tworzonego urz¹dzenia i oprogramowania.
 
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
/*	      Procedury obs³ugi interfejsu I2C		   */
/***************************************************/

/*
 Wysy³a bajt na magistralê
  param:  b bajt do wys³ania
  return: potwierdzenie z magistrali (wartoœæ >0) lub 0 w przypadku niepowodzenia
*/
u08 I2C_write(u08 b);

/*
 Czyta bajt z magistrali i wysy³a potwierdzenie (ACK) na magistralê
  param:  ack wartoœæ bitu potwierdzenia (1 lub 0)
  return: odczytany bajt z magistrali
*/
u08 I2C_read(u08 ack);

/*
 Wysy³a sekwencjê START na magistralê
*/
void I2C_start(void);

/*
 Wysy³a sekwencjê STOP na magistralê
*/
void I2C_stop(void);


#endif // __I2C_H__
