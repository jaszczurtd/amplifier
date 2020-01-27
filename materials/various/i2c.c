#include "i2c.h"


//*********************************************
//*  Podprogram inicjowania magistrali I2C    *
//*********************************************
void I2C_start(void)
{
  I2C_SCL_H();
  I2C_SDA_H();
  I2C_SDA_WR();
  I2C_SCL_WR();	 //inicjalizacja kierunków portów
  _delay_us(10);
  I2C_SDA_L();
  _delay_us(10);
  I2C_SCL_L();
  _delay_us(10);
}


//*********************************************
//*  Podprogram zatrzymania magistrali I2C    *
//*********************************************
void I2C_stop(void)
{
  I2C_SDA_WR();		// SDA na zapis
  I2C_SCL_H();
  _delay_us(10);
  I2C_SDA_H();
  _delay_us(10);
}


//*********************************************
//*    Podprogram odczytu z magistrali I2C    *
//*********************************************
u08 I2C_read(u08 ack)
{
  u08 i;
  u08 b = 0;
  I2C_SDA_RD();		// SDA na odczyt
  I2C_SDA_H();		// podci¹ganie SDA
  _delay_us(10);
  for (i=0; i<8; i++)	// czytanie 8 bitów
  {
    I2C_SCL_H();	// SCL w stan wysoki
    _delay_us(10);
    b <<= 1;		// przesuñ o 1 bit
    if (bit_is_set(I2C_PORT_I,I2C_SDA)) // jeœli SDA=1 
      b |= 1;		// dodaj odczytany bit z magistrali
    I2C_SCL_L();	// SCL w stan niski
    _delay_us(10);
  }
  I2C_SDA_WR();		// SDA na zapis
  if (ack == 0)	    // ustaw bit ACK na okreœlon¹ wartoœæ
    I2C_SDA_L();
  else
    I2C_SDA_H();
  _delay_us(10);
  I2C_SCL_H();
  _delay_us(10);
  I2C_SCL_L();
  _delay_us(10);
  I2C_SDA_L();
  _delay_us(10);
  return(b);
}


//*********************************************
//*    Podprogram zapisu na magistralê I2C    *
//*********************************************
u08 I2C_write(u08 b)
{
  u08 i;
  I2C_SDA_WR();		  // SDA na zapis
  for (i=0; i<8; i++) // zapis 8 bitów
  {
    if (b & 0x80)
      I2C_SDA_H();
    else
      I2C_SDA_L();
    _delay_us(10);
    I2C_SCL_H();
    _delay_us(10);
    I2C_SCL_L();
    b <<= 1;		  // przesuñ o 1 bit
  }
  I2C_SDA_RD();		  // w³¹cz czytanie SDA
  I2C_SDA_H();		  // podci¹gnij SDA
  _delay_us(10);
  I2C_SCL_H();		  // SCL=1
  _delay_us(10);
  i=0xFF;
  do
  {
    if (bit_is_clear(I2C_PORT_I,I2C_SDA)) break; // je¿eli jest potwierdzenie
    _delay_us(10);
  }
  while(--i>0);
  I2C_SCL_L();		  // SCL=0
  _delay_us(10);
  return(i);
}

