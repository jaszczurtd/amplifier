#include "PCF8563.h"
#include "i2c.h"
#include "i2c.c"

extern u08 PCF8563_status_CS1;

//*********************************************
//*    Podprogram konwersji BCD na liczbê     *
//*********************************************
u08 bcd2bin(u08 bcd)                    
{
#ifdef OPTIMIZE_SPEED
  return (10*(bcd>>4)|(bcd&0x0f));
#else
  u08 Temp = bcd & 0x0F; 
  while (bcd>=0x10) 
  { 
    Temp += 10; 
    bcd -= 0x10; 
  } 
  return Temp; 
#endif
}


//*********************************************
//*    Podprogram konwersji liczby na BCD     *
//*********************************************
u08 bin2bcd(u08 bin)                    
{
#ifdef OPTIMIZE_SPEED
  return (((bin/10)<<4)|(bin%10));
#else
  u08 Temp = 0; 
  while(bin>9) 
  { 
    Temp += 0x10; 
    bin-=10; 
  } 
  return Temp+bin; 
#endif
}


//*********************************************
//*  Podprogram odczytu statusu CS1 z PCF8563 *
//*********************************************
u08 PCF8563_get_status_CS1(void)
{
  PCF8563_status_CS1=PCF8563_read(CONTROL_STATUS_1);
  return PCF8563_status_CS1;
}


//*********************************************
//*    Podprogram odczytu bajtu z PCF8563     *
//*********************************************
u08 PCF8563_read(u08 address)
{
  u08 a;
  I2C_start();
  I2C_write(SLAVE_ADDR_WR); //slave adres zapisu równy A2
  I2C_write(address);
  I2C_start();
  I2C_write(SLAVE_ADDR_RD); //slave adres odczytu równy A3
  a=I2C_read(1);
  I2C_stop();
  return a;
}


//*********************************************
//*     Podprogram startu uk³adu PCF8563      *
//*********************************************
void PCF8563_start(void)
{
  PCF8563_get_status_CS1();  //odczyt CS1
  PCF8563_status_CS1&=0xdf;  //b5=0
  PCF8563_write(CONTROL_STATUS_1,PCF8563_status_CS1);
}


//*********************************************
//*   Podprogram zatrzymania uk³adu PCF8563   *
//*********************************************
void PCF8563_stop(void)
{
  PCF8563_get_status_CS1();
  PCF8563_status_CS1|=0x20;  //b5=1
  PCF8563_write(CONTROL_STATUS_1,PCF8563_status_CS1);
}


//*********************************************
//*    Podprogram zapisu bajtu do PCF8563     *
//*********************************************
void PCF8563_write(u08 address,u08 data)
{
  I2C_start();
  I2C_write(SLAVE_ADDR_WR);  //slave adres zapisu równy A2
  I2C_write(address);
  I2C_write(data);
  I2C_stop();
}


//*********************************************
//*     Podprogram zapisu BCD na PCF8563      *
//*********************************************
void PCF8563_write_bcd(u08 address,u08 data)
{
  PCF8563_write(address,bin2bcd(data));
}


//*********************************************
//*    Podprogram inicjacji uk³adu PCF8563    *
//*********************************************
void PCF8563_init(void)
{
  PCF8563_status_CS1=PCF8563_CS1;
  PCF8563_write(CONTROL_STATUS_1,PCF8563_CS1);
  PCF8563_write(CONTROL_STATUS_2,PCF8563_CS2);
  PCF8563_write(CLKOUT_CONTROL,PCF8563_CLKOUT);
  PCF8563_write(TIMER_CONTROL,PCF8563_TIMER);
}


//*********************************************
//* Podprogram odczytu czasu z uk³adu PCF8563 *
//*********************************************
void PCF8563_get_time(u08 *hour,u08 *min,u08 *sec)
{
  u08 hour1, min1, sec1;

  sec1=PCF8563_read(VL_SECONDS); //rej.2 bity b0-b6
  *sec=bcd2bin(sec1&0x7f);

  min1=PCF8563_read(MINUTES);    //rej.3 bity b0-b6
  *min=bcd2bin(min1&0x7f);

  hour1=PCF8563_read(HOURS);      //rej.4 bity b0-b5
  *hour=bcd2bin(hour1&0x3f);
}


//*********************************************
//* Podprogram zapisu czasu do uk³adu PCF8563 *
//*********************************************
void PCF8563_set_time(u08 godz,u08 min,u08 sec)
{
  PCF8563_stop();
  PCF8563_write_bcd(VL_SECONDS,sec); //rej.2
  PCF8563_write_bcd(MINUTES,min);    //rej.3
  PCF8563_write_bcd(HOURS,godz);     //rej.4
  PCF8563_start();
}


//*********************************************
//* Podprogram odczytu daty z uk³adu PCF8563  *
//*********************************************
void PCF8563_get_date(u08 *dzien,u08 *mc, u08 *rok)
{
  u08 dzien1, mc1;

  dzien1=PCF8563_read(DAYS);    //rej.5
  *dzien=bcd2bin(dzien1&0x3f);  //dzieñ na bitach b0-b5 jako BCD

  mc1=PCF8563_read(C_MONTHS);   //rej.7
  *mc=bcd2bin(mc1&0x1f);        //miesiac na bitach b0-b4 jako BCD

  *rok=PCF8563_read(YEARS);     //rej.8 jako liczba od 00 do 99
}


//*********************************************
//* Podprogram zapisu daty do uk³adu PCF8563 *
//*********************************************
void PCF8563_set_date(u08 dzien, u08 mc, u08 rok)
{
  PCF8563_stop();
  PCF8563_write(YEARS,rok);
  PCF8563_write_bcd(DAYS,dzien);
  PCF8563_write_bcd(C_MONTHS,mc);
  PCF8563_start();
}



