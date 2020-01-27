#ifndef PCF8563_H
#define PCF8563_H

#include "config.h"
#include "i2c.h"

//adresy rejestrów slave
#define SLAVE_ADDR_WR   0xA2 //adres rejestru aktywuj¹cego zapis 
#define SLAVE_ADDR_RD   0xA3 //adres rejestru aktywuj¹cego odczyt

//adresy rejestrów steruj¹cych
#define CONTROL_STATUS_1		0x00 //rejestr kontrolny i statusowy 1
#define CONTROL_STATUS_2  		0x01 //rejestr kontrolny i statusowy 2
#define CLKOUT_CONTROL			0x0D //rejestr CLKOUT i wyjscia zegarowego
#define TIMER_CONTROL			0x0E //rejestr kontrolny TIMER

//adresy rejestrów czasu i daty
#define VL_SECONDS				0x02 //rejestr zaniku zasilania (7 bit VL) i sekund (6-4bit dziesatki, 3-0bit jednosci)
                                                   //VL=0 - napiêcie w normie, VL=1 - nieprawid³owe napiêcie (za niskie)
#define MINUTES 				0x03 //rejestr minut (7bit nieuzywany, 6-4bit dziesiatki, 3-0bit jednosci)
#define HOURS					0x04 //rejestr godzin (7-6bit nieuzywane, 5-4bit dziesiatki, 3-0bit jednosci)
#define DAYS 					0x05 //rejestr dni (7-6bit nieuzywane, 5-4bit dziesiatki, 3-0bit jednosci)
#define WEEKDAYS				0x06 //rejestr dnia tygodnia 0 - 6 (7-3bit nieuzywane, 2-0bit jednosci)
                                                   //0 - niedziela, ..., 6 - sobota
#define C_MONTHS				0x07 //rejestr stulecia (7bit C, 6-5bit nieuzywane) i miesiecy 1-12 (4bit dziesiatki, 3-0 jednosci)
                                                   //C=0 dla roku 20xx, C=1 dla roku 19xx; mc=1 styczeñ, ..., mc=12 grudzieñ
#define YEARS					0x08 //rejestr roku 0 - 99 (7-4bit dziesiatki, 3-0bit jednosci)

#define PCF8563_CS1             0x00 //poczatkowa zawartosc rejestru konfiguracyjnego Control_status_1
#define PCF8563_CS2             0x00 //poczatkowa zawartosc rejestru konfiguracyjnego Control_status_2
#define PCF8563_CLKOUT          0x00 //poczatkowa zawartosc rejestru CLOKOUT 
#define PCF8563_TIMER			0x00 //poczatkowa zawartosc rejestru TIMER 

u08 bin2bcd(u08 bin);                   
u08 bcd2bin(u08 bcd);                    
u08 PCF8563_get_status_CS1(void);
u08 PCF8563_read(u08 address);
void PCF8563_start(void);
void PCF8563_stop(void);
void PCF8563_write(u08 address,u08 data);
void PCF8563_write_bcd(u08 address,u08 data);
void PCF8563_init(void);
void PCF8563_get_time(u08 *hour,u08 *min,u08 *sec);
void PCF8563_set_time(u08 godz,u08 min,u08 sec);
void PCF8563_get_date(u08 *dzien,u08 *mies, u08 *rok);
void PCF8563_set_date(u08 dzien, u08 mc, u08 rok);


#endif // PCF8563_H




