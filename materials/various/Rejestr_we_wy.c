//*******************************************************************
//* Rejestr_we_wy.c                                                 *
//*                                                                 *
//* Kompilator: WinAVR (AVR Studio 4)                               *
//* Procesor:   ATMEGA32 (8MHz zewn.)                               *
//* Autor:      Janusz Ginter, Gdañsk, Poland                       *
//*******************************************************************
//* Opis:                                                           *
//* Odczyt czujników otwarcia drzwi do mieszkania (podwójnych)      *
//* i zapis daty i czasu zdarzenia na karcie SD.                    *
//* Odczyt bie¿¹cej daty i czasu z PCF8563 jest wyœwietlany na LCD. *
//* Mo¿liwe jest ustawianie bie¿¹cego czasu i daty (naciskaj¹c S3   *
//* przejœæ do zmiany rok/miesi¹c/dzieñ/godzina/minuta, naciskaj¹c  *
//* S2 ustawiæ wartoœæ, naciœniêcie S1 zapisuje ustawienia).        *
//*******************************************************************

// Zdarzenia:
//
// WE - wejœcie z zewn¹trz do mieszkania,
// WY - wyjœcie na zewn¹trz z mieszkania,
// OZ - otwarcie i zamkniêcie pary drzwi z zewn¹trz,
// OW - otwarcie i zamkniêcie pary drzwi z wewn¹trz,
// OX - otwarcie i zamkniêcie tylko drzwi zewnêtrznych,
// OY - otwarcie i zamkniêcie tylko drzwi wewnêtrznych.

// Ustawienie fusebitów (zeg. zewn. 8MHz):
//
// b0=9F, b1=9D, b2=98, b3=9B
// L: Bodlev=1, Boden=1, Sut1=1, Sut0=1 CKsel3=1, CKsel2=1, CKsel1=1, CKsel0=1  (FF)
// H: OCDN=1 JTAGEN=1, SPIEN=0, CKOPT=1, EGSAV=1 Bootsz1=0, Bootsz0=0, Bootrst=1 (D9)
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>  //_delay_ms
#include "hd44780.c"
#include <avr/interrupt.h>
#include "i2c.h"
#include "PCF8563.h"
#include "PCF8563.c"
#include "SPI_routines.h" 
#include "SD_routines.h" 
#include "FAT32.h"

#define buzzer PA0		//buzer na PA0

// zapal LED_G i LED_R
#define SET_LED_G     PORTA |= (1<<PA1)
#define SET_LED_R     PORTA |= (1<<PA2)

// zgaœ LED_G i LED_R
#define CLR_LED_G    PORTA  &= ~(1<<PA1)
#define CLR_LED_R    PORTA  &= ~(1<<PA2)

// W tablicy bêd¹ formowane komunikaty tekstowe wysy³ane do wyœwietlacza
char str[17];

int dr_z, dr_w;		//aktualny stan drzwi zewn. i wewn.
//0 - zamkniête,
//1 - otwarte,
//2 - zamkniêto wczeœniej otwarte

int we_wy;          //status wejœcia/wyjœcia:
// 0 brak otwarcia - nie zapisywaæ rekordu
// 1 czyli WE - wejœcie z zewn¹trz do mieszkania,
// 2 czyli WY - wyjœcie na zewn¹trz z mieszkania,
// 3 czyli OZ - otwarcie i zamkniêcie pary drzwi z zewn¹trz,
// 4 czyli OW - otwarcie i zamkniêcie pary drzwi z wewn¹trz,
// 5 czyli OX - otwarcie i zamkniêcie tylko drzwi zewnêtrznych,
// 6 czyli OY - otwarcie i zamkniêcie tylko drzwi wewnêtrznych.

int kolejnosc_otw; 	//kolejnoœæ otwarcia drzwi
//0 - wartoœæ pocz¹tkowa
//1 - otwarto najpierw drzwi zewnêtrzne
//2 - otwarto najpierw drzwi wewnêtrzne

int kolejnosc_zam; 	//kolejnoœæ zamkniêcia drzwi
//0 - wartoœæ pocz¹tkowa
//1 - zamkniêto najpierw drzwi zewnêtrzne
//2 - zamkniêto najpierw drzwi wewnêtrzne

int tryb_zapisu; 	//wskaŸnik czy zapisywaæ na karcie
//0 - nie zapisywaæ na kartê,
//1 - zapisaæ na kartê (wartoœæ pocz¹tkowa).

int tryb_ustawiania;  //wykorzystywany przy ustawianiu daty i czasu
//0 - rok,
//1 - mc,
//2 - dzieñ,
//3 - godzina,
//4 - minuta.

int i=0, funkcja=0, ustaw=0;
u08 sek,minuta,godzina,dzien,miesiac,sek_p;   	// aktualny czas 
u08 rok;  //rok 00 - 99 (14-30)

int zmiana;
//, otwarty_plik;

//Zmienne globalne - wykorzystywane w PCF8563.c
u08 PCF8563_status_CS1;

int porta = 0x01; //buzzer

int nr_inf;                                       //nr wyœwietlanej informacji
char zdarzenie[8];                                //skrót zdarzenia

unsigned char rob;

// deklaracja nazwy pliku i zmiennej na kopiê z uwagi na zmianê zawartoœci przez FAT32
unsigned char fileNameZ[13]="WE_WY.TXT";
unsigned char fileName[13];

unsigned char rekord[25];  //bufor rekordu do zapisu na SD

//***********************************************************
//*            Podprogram inicjowania portów                *
//* ustawienie buzzera, wskaŸników LED, klawiszy, czujników *
//***********************************************************
void port_init(void)
{
  PORTA = 0x00;
  DDRA=0x07;  	  // PA0 - buzzer, PA1 i PA2 - wyjœcia LED
	 			  // PA3 - kl. S3, PA4 - kl. S2, PA5 - kl. S1
  PORTB = 0xEF;
  DDRB  = 0xBF;   //MISO line i/p, rest o/p

  PORTC = 0x00;
  DDRC  = 0x00;

  PORTD = 0x00;
  DDRD  = 0x06;   //PD1 - cz.d.zewn.  PD2 - cz.d.wewn.
}

//***********************************************************
//*     Podprogram inicjowania wszystkich peryferalow       *
//***********************************************************
void init_devices(void)
{
  cli();  //all interrupts disabled
  port_init();
  spi_init();

  MCUCR = 0x00;
  GICR  = 0x00;
  TIMSK = 0x00; //timer interrupt sources
  sbi(PORTD,1);     // "podci¹gnij" do logicznej 1 liniê PD1 (cz.d.z.)
  sbi(PORTD,2);     // "podci¹gnij" do logicznej 1 liniê PD2 (cz.d.w.)

  sbi(PORTA,3);     // "podci¹gnij" do logicznej 1 liniê PA3 (S3)
  sbi(PORTA,4);     // "podci¹gnij" do logicznej 1 liniê PA4 (S2)
  sbi(PORTA,5);     // "podci¹gnij" do logicznej 1 liniê PA5 (S1)

  LCD_Initalize();  //inicjowanie LCD

  _delay_ms(200);

}

//********************************************************
//*       Podprogram wys³ania liczby value na LCD        *
//********************************************************
void lcd_putint(int value, u08 radix)	
// wysy³a na port szeregowy tekst 
// przedstawiaj¹cy value z podstaw¹ radix
{
  char string[18];			  // bufor na wynik funkcji itoa
  itoa(value, string, radix); // zamiana liczby na string
  LCD_WriteText(string);	  // wyœlij string na LCD
}

//********************************************************
//*         Podprogram wys³ania na LCD godz:min          *
//********************************************************
void lcd_put_godz_min() 
{
   if (godzina<10) LCD_WriteText("0");
   lcd_putint(godzina,10);
   LCD_WriteText(":");
   if (minuta<10) LCD_WriteText("0");
   lcd_putint(minuta,10);
}	

//********************************************************
//* Podprogram odczytu czasu z PCF8563 i wys³ania na LCD *
//********************************************************
void lcd_put_time()
{
   PCF8563_get_time(&godzina,&minuta,&sek);
   lcd_put_godz_min();
   LCD_WriteText(":");
   if (sek<10) LCD_WriteText("0");
   lcd_putint(sek,10);
}

//********************************************************
//*         Podprogram wys³ania na LCD dzien/miesiac/rok *
//********************************************************
void lcd_put_dzien_mc() 
{
   if (dzien<10) LCD_WriteText("0");
   lcd_putint(dzien,10);
   LCD_WriteText("/");
   if (miesiac<10) LCD_WriteText("0");
   lcd_putint(miesiac,10);
}	

//********************************************************
//*  Podprogram odczytu daty z PCF8563 i wys³ania na LCD *
//********************************************************
void lcd_put_date()
{
   PCF8563_get_date(&dzien,&miesiac,&rok);
   lcd_put_dzien_mc();
   LCD_WriteText("/20");
   if (rok>30) rok=14;  //===============
   lcd_putint(rok,10);
}

//**********************************************************
//* Podprogr. wyœw. daty DD/MM/ROK na LCD i czasu GG:MM:SS *
//**********************************************************
void obsluga_PCF8563()      
{
   _delay_ms(100);
   PCF8563_get_time(&godzina,&minuta,&sek);
   PCF8563_get_date(&dzien,&miesiac,&rok);

   if (!(sek==sek_p))
   {
     sek_p=sek;
     LCD_Clear(); 
	 LCD_WriteText("Data: ");
	 lcd_put_date();

	 LCD_GoTo(0,1);
     LCD_WriteText("Czas: ");
     lcd_put_time();  //wyœwietlenie czasu
     LCD_GoTo(14,1);
     if (tryb_zapisu==1)        //gdy zapis w³¹czony wyœwietl X, jeœli nie to wyœwietl O
      {
         LCD_WriteText(" X");
      }
      else 
      {
         LCD_WriteText(" O");
      };
   };
}

//********************************************************
//*            Podprogram pojedyñczego dŸwiêku           *
//********************************************************
void beep() 
{
  const int TBEEP = 150;
  u08 i;
  for(i=0; i<100; i++)
  {
    PORTA |= (1<<buzzer);  //ustaw 1 na buzzer
    _delay_us(TBEEP);
    PORTA  &= ~(1<<buzzer);	//wyzeruj buzzer 
    _delay_us(TBEEP);
  }
}

//********************************************************
//*             Podprogram Trzykrotny dŸwiêk             *
//********************************************************
void beep_3()
{
  beep();
  _delay_ms(100);
  beep();
  _delay_ms(100);
  beep();
}

//********************************************************
//*           Podprogram wyœiwtlenia informacji          *
//********************************************************
void wyswietl_inf()
{
  LCD_Clear(); 
  switch (nr_inf)
  {
    case 1:
	   _delay_ms(500);
       LCD_WriteText("AVR-DOS v ");
       LCD_GoTo(2,1);
       LCD_WriteText("Rozmiar ");
	   _delay_ms(2000);
       LCD_Clear();
       LCD_WriteText("Wolne " );
       LCD_GoTo(2,1);
       LCD_WriteText("System OK");
	   break;

    case 2:
       obsluga_PCF8563(); 
       LCD_GoTo(14,1);
       if (tryb_zapisu==1)        //gdy zapis w³¹czony wyœwietl X, jeœli nie to wyœwietl O
        {
         LCD_WriteText(" X");
        }
        else 
        {
         LCD_WriteText(" O");
		};
	   break;

    case 3:
	   break;

	case 4:
       LCD_WriteText("Ustawianie godz.");
       LCD_GoTo(2,1);
       LCD_WriteText("Godzina=");
       lcd_putint(godzina,10);
	   break;

	case 5:
       LCD_WriteText("Ustawianie minut");
       LCD_GoTo(2,1);
       LCD_WriteText("Minuta=");
       lcd_putint(minuta,10);
	   break;

	case 6:
       LCD_WriteText("Ustawianie dnia");
       LCD_GoTo(2,1);
       LCD_WriteText("Dzien=");
       lcd_putint(dzien,10);
	   break;

	case 7:
       LCD_WriteText("Ustawianie mies.");
       LCD_GoTo(2,1);
       LCD_WriteText("Miesiac=");
       lcd_putint(miesiac,10);
	   break;

	case 8:
       LCD_WriteText("Us. zapis SD/MMC");
       LCD_GoTo(0,1);
	   switch (tryb_zapisu)
	   {                               //wyœwietlenie ustawionego trybu zapisu
        case 0 : LCD_WriteText("Zapisz buf i wyl");
        case 1 : LCD_WriteText("Zap. do buf i wl");
	   }
	   break;

	case 9:
       LCD_WriteText("Zapis: ");
	   LCD_WriteText(zdarzenie);
	   break;

	case 10:
	   break;

	case 11:
	   break;

	case 12:
       LCD_WriteText("Ustawianie roku");
       LCD_GoTo(2,1);
       LCD_WriteText("Rok=20");
       lcd_putint(rok,10);
	   break;

  }; //switch
};

//********************************************************
//*             Podprogram ustawiania zegara             *
//********************************************************
void ust_zegara()
{
  nr_inf=12;
  wyswietl_inf();
  while (1)
  {
    switch (tryb_ustawiania)
    {
      case 0: //rok
        if(!(PINA & 0x20)) // przycisk S1 - koniec ustawiania
        {  
	      beep();
          while(!(PINA & 0x20)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      return;  //wyjœcie z podprogramu
        };
        if(!(PINA & 0x10)) // przycisk S2 - zwiêkszenie rok o 1
        {  
	      beep();
          while(!(PINA & 0x10)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      nr_inf=12;
          zmiana=2;
          rok++;     
          if (rok<14)
            {rok=14;};                              //ustawianie roku od 2014 do 2030
          if (rok>30) 
            {rok=14;};
	      wyswietl_inf();
		  break;  //wyjœcie z case
        };
        if(!(PINA & 0x08)) // przycisk S3 - zwiêkszenie trybu ustawiania o 1
        {  
	      beep();
          while(!(PINA & 0x08)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
		  tryb_ustawiania=1;
		  nr_inf=7;
		  wyswietl_inf();
		  break;  //wyjœcie z case
        };
		break;

      case 1: //miesi¹c
        if(!(PINA & 0x20)) // przycisk S1 - koniec ustawiania
        {  
	      beep();
          while(!(PINA & 0x20)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      return;  //wyjœcie z podprogramu
        };
        if(!(PINA & 0x10)) // przycisk S2 - zwiêkszenie miesiac o 1
        {  
	      beep();
          while(!(PINA & 0x10)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      nr_inf=7;
          zmiana=2;
          miesiac++;     
          if (miesiac>12) 
            {miesiac=1;};
	      wyswietl_inf();
		  break;  //wyjœcie z case
        };
        if(!(PINA & 0x08)) // przycisk S3 - zwiêkszenie trybu ustawiania o 1
        {  
	      beep();
          while(!(PINA & 0x08)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
		  tryb_ustawiania=2;
		  nr_inf=6;
		  wyswietl_inf();
		  break;  //wyjœcie z case
        };
		break;

      case 2: //dzieñ
        if(!(PINA & 0x20)) // przycisk S1 - koniec ustawiania
        {  
	      beep();
          while(!(PINA & 0x20)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      return;  //wyjœcie z podprogramu
        };
        if(!(PINA & 0x10)) // przycisk S2 - zwiêkszenie dzien o 1
        {  
	      beep();
          while(!(PINA & 0x10)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      nr_inf=6;
          zmiana=2;
          dzien++;     
          if (dzien>31) 
            {dzien=1;};
	      wyswietl_inf();
		  break;  //wyjœcie z case
        };
        if(!(PINA & 0x08)) // przycisk S3 - zwiêkszenie trybu ustawiania o 1
        {  
	      beep();
          while(!(PINA & 0x08)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
		  tryb_ustawiania=3;
		  nr_inf=4;
		  wyswietl_inf();
		  break;  //wyjœcie z case
        };
		break;

      case 3: //godzina
        if(!(PINA & 0x20)) // przycisk S1 - koniec ustawiania
        {  
	      beep();
          while(!(PINA & 0x20)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      return;  //wyjœcie z podprogramu
        };
        if(!(PINA & 0x10)) // przycisk S2 - zwiêkszenie dzien o 1
        {  
	      beep();
          while(!(PINA & 0x10)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      nr_inf=4;
          zmiana=1;
          godzina++;     
          if (godzina>23) 
            {godzina=0;};
	      wyswietl_inf();
		  break;  //wyjœcie z case
        };
        if(!(PINA & 0x08)) // przycisk S3 - zwiêkszenie trybu ustawiania o 1
        {  
	      beep();
          while(!(PINA & 0x08)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
		  tryb_ustawiania=4;
		  nr_inf=5;
		  wyswietl_inf();
		  break;  //wyjœcie z case
        };
		break;

      case 4: //minuta
        if(!(PINA & 0x20)) // przycisk S1 - koniec ustawiania
        {  
	      beep();
          while(!(PINA & 0x20)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      return;  //wyjœcie z podprogramu
        };
        if(!(PINA & 0x10)) // przycisk S2 - zwiêkszenie dzien o 1
        {  
	      beep();
          while(!(PINA & 0x10)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
	      nr_inf=5;
          zmiana=1;
          minuta++;     
          if (minuta>59) 
            {minuta=0;};
	      wyswietl_inf();
		  break;  //wyjœcie z case
        };
        if(!(PINA & 0x08)) // przycisk S3 - zwiêkszenie trybu ustawiania o 1
        {  
	      beep();
          while(!(PINA & 0x08)) {}; //czekanie na puszczenie klawisza
          _delay_ms(50);
		  tryb_ustawiania=0;
		  nr_inf=12;
		  wyswietl_inf();
		  break;  //wyjœcie z case
        };
		break;
    };
  };
};



//********************************************************
//*       Podprogram ustawiania trybu zapisu             *
//********************************************************
void ust_zapisu()
{
  while (1)
  {
    nr_inf=8;
    wyswietl_inf();
	while (1)
	{
      if(!(PINA & 0x20)) 				// przycisk S1
       {  
         beep();
         while(!(PINA & 0x20)) {}; 	//czekanie na puszczenie klawisza
         _delay_ms(50);
		 return;  //zakoñczenie ustawiania
       };
      if(!(PINA & 0x08)) 			// przycisk S3
       {  
         beep();
         while(!(PINA & 0x08)) {}; 	//czekanie na puszczenie klawisza
         _delay_ms(50);

         tryb_zapisu++;     //ustwiania trybu zapisu
         if(tryb_zapisu>1)
         {
	       tryb_zapisu=0;
         };
		 wyswietl_inf();
       };
	};
  };
};


//********************************************************
//*     Podprogram sprawdzania czujników drzwi           *
//********************************************************
void sprawdzenie_drzwi()
{
  //sprawdzenie, czy drzwi wewnêtrzne s¹ otwarte
  if (!(PIND & 0x02))  //drzwi wew. s¹ zamkniête
   {
     CLR_LED_G;  //zgaœ zielony LED
     if (dr_w==1)
	 {           //drzwi wewn. poprzednio by³y otwarte
       if (kolejnosc_zam==0)
	   {                    //gdy pierwsze zamkniêcie drzwi
          kolejnosc_zam=2; //ustaw wsk. kolejnoœci zamykania
       };
     dr_w=2;  //ustaw wsk. zamkniêcia wczeœniej otwartych drzwi wewn.
     };
   }
   else                            //drzwi wewn. s¹ otwarte
   {
     SET_LED_G;               //zapal zielony LED
     dr_w=1;                  //ustaw wsk. otwartych drzwi wewnêtrznych
     if (kolejnosc_otw==0) 
	 {                        //gdy pierwsze otwarcie drzwi
       kolejnosc_otw=2;    //ustaw wsk. kolejnoœci otwierania
     };
   };

  //sprawdzenie, czy drzwi zewnêtrzne s¹ otwarte
  if (!(PIND & 0x04))   //drzwi zewn. s¹ zamkniête
   {
     CLR_LED_R; //zgaœ czerwony LED
     if (dr_z==1)
	 {               //drzwi zewn. poprzednio by³y otwarte
       if (kolejnosc_zam==0)
	   {   //gdy pierwsze zamkniêcie drzwi
          kolejnosc_zam=1;      //ustaw wsk. kolejnoœci zamykania
       }
     dr_z=2;  //ustaw wsk. zamkniêcia wczeœniej otwartych drzwi zewn.
     };
   }
   else   //drzwi zewn. otwarte
   {
     SET_LED_R;   //zapal czerwony LED
     dr_z=1;     //ustaw wsk. otwartych drzwi zewnêtrznych
     if (kolejnosc_otw==0)
	 {     //gdy pierwsze otwarcie drzwi
        kolejnosc_otw=1;       //ustaw wsk. kolejnoœci otwierania
     };
    };

   //sprawdzenie, czy s¹ zamkniête drzwi a poprzednio by³y otwarte
   if (!(kolejnosc_zam==0))
   {          //by³y wczeœniej otwierane drzwi
      if (!(dr_w==1))
	  {  
	     if (!(dr_z==1))
		 {     //drzwi wewn. i zewn. s¹ ju¿ zamkniête
            if (dr_w==2)
		     {     //zamkniêto wczeœniej otwarte drzwi wewn.
               if (dr_z==2)
			    {    //zamkniêto wczeœniej otwarte drzwi zewn.
                  if (kolejnosc_otw==1)
				   {    //otwarto najpierw drzwi zewnêtrzne
                     if (kolejnosc_zam==1)
					  {   //zamkniêto najpierw drzwi zewnêtrzne
                        we_wy=1;     //WE
					  }
                      else  
					  {       //zamkniêto najpierw drzwi wewnêtrzne
                        we_wy=3;     //OZ
					  };
				   }
                   else 
				   {     //otwarto najpierw drzwi wewnêtrzne:
                     if (kolejnosc_zam==1)
					  {    //zamkniêto najpierw drzwi zewnêtrzne
                        we_wy=4;     //OW
					  }
                      else
					  {       //zamkniêto najpierw drzwi wewnêtrzne
                        we_wy=2;     //WY
                      };
                   };
				}
                else 
				{      //otwarto i zamkniêto tylko drzwi wewnêtrzne
                  we_wy=6;        //OY
                };
			 }
             else     //otwarto i zamkniêto tylko drzwi zewnêtrzne
			 {
               we_wy=5;          //OX
             };

            dr_z=0;
            dr_w=0;
            kolejnosc_otw=0;
            kolejnosc_zam=0;

         };
      };
   };
}


//********************************************************
//*             Podprogram inicjowania SD                *
//********************************************************
void inicjuj_SD()
{
  unsigned char error, FAT32_active;
  unsigned int i;
  cardType = 0;
  for (i=0; i<10; i++)
  {
    error = SD_init();
    if(!error) break;
  };

  if(error)
  {
    if(error == 1) 
    {
       LCD_Clear();
       LCD_WriteText("Blad!!!" );
       LCD_GoTo(0,1);
       LCD_WriteText("Brak karty SD");
    };

    if(error == 2) 
    {
       LCD_Clear();
       LCD_WriteText("Blad!!!");
       LCD_GoTo(0,1);
       LCD_WriteText("Inicjalizacja");
  
    };
  _delay_ms(2000);

  while(1);  //wait here forever if error in SD init 
  }

  switch (cardType)
  {
    case 1:
         LCD_Clear();
         LCD_WriteText("Karta SD typu");
         LCD_GoTo(0,1);
         LCD_WriteText("ver. 1.x");
  		 break;
  case 2:
         LCD_Clear();
         LCD_WriteText("Karta typu");
         LCD_GoTo(0,1);
         LCD_WriteText("High SD Capacity");
  		 break;
  case 3:
         LCD_Clear();
         LCD_WriteText("Karta SD typu");
         LCD_GoTo(0,1);
         LCD_WriteText("ver. 2.x");
  		 break;
  default:
         LCD_Clear();
         LCD_WriteText("Karta SD typu");
         LCD_GoTo(0,1);
         LCD_WriteText("nieznanego");
  		 break; 
  }
  _delay_ms(2000);

  SPI_HIGH_SPEED;	//SCK - 4 MHz
  _delay_ms(1);   //some delay

//=================
  FAT32_active = 1;
  error = getBootSectorData (); //read boot sector and keep necessary data in global variables
  if(error) 	
  {
    LCD_Clear();
    LCD_WriteText("Blad partycji.");
    LCD_GoTo(0,1);
    LCD_WriteText("Brak FAT32!");
    FAT32_active = 0;
  };

}


//************************************************************
//* Podprogram dopisania rekordu i ewentualnego wyœwietlenia *
//* komunikatu b³êdu                                         *
//************************************************************
void dopisz_rekord(unsigned char *rek)
{
  unsigned char blad;

  for (rob=0; rob<9; rob++)
    fileName[rob]=fileNameZ[rob];

  blad=writeFile (fileName,rek); 
// funkcja writeFile zwraca:
// 0: dopisano rekord
// 1: nieudane utworzenie pliku
// 2: brak wolnych klastrow
// 3: b³¹d zapisu klastra
// 4: b³êdna nazwa pliku

  _delay_ms(200);              
  if (!(blad==0))
  {
    switch (blad)
    {
      case 1:
         LCD_Clear();
         LCD_WriteText("Blad: nieudane");
         LCD_GoTo(0,1);
         LCD_WriteText("utworzenie pliku");
		 _delay_ms(2000);
  		 break;

      case 2:
         LCD_Clear();
         LCD_WriteText("Blad:brak woln.");
         LCD_GoTo(0,1);
         LCD_WriteText("klastrow");
		 _delay_ms(2000);
  		 break;

      case 3:
         LCD_Clear();
         LCD_WriteText("Blad zapisu");
         LCD_GoTo(0,1);
         LCD_WriteText("klastra");
 		 _delay_ms(2000);
 		 break;

      case 4:
         LCD_Clear();
         LCD_WriteText("Bledna nazwa");
         LCD_GoTo(0,1);
         LCD_WriteText("pliku");
		 _delay_ms(2000);
  		 break;
    };
  };
}

//********************************************************
//*     Podprogram redagowania i zapisu rekordu na SD    *
//********************************************************
void czas_data_do_rekordu()
{
  char string[18];			  // bufor na wynik funkcji itoa

      itoa(dzien, string, 10); // zamiana dnia na string
	  if (dzien>9)
	   {
	     rekord[0]=string[0];
	     rekord[1]=string[1];
       }
	   else
	   {
	     rekord[0]='0';
	     rekord[1]=string[0];
	   };
	  rekord[2]='/';

      itoa(miesiac, string, 10); // zamiana miesi¹ca na string
	  if (miesiac>9)
	   {
	     rekord[3]=string[0];
	     rekord[4]=string[1];
       }
	   else
	   {
	     rekord[3]='0';
	     rekord[4]=string[0];
	   };
	  rekord[5]='/';

      itoa(rok, string, 10); // zamiana roku na string
	  rekord[6]=string[0];
	  rekord[7]=string[1];
	  rekord[8]=string[2];
	  rekord[9]=string[3];
	  rekord[10]=' ';

      itoa(godzina, string, 10); // zamiana godziny na string
	  if (godzina>9)
	   {
	     rekord[11]=string[0];
	     rekord[12]=string[1];
       }
	   else
	   {
	     rekord[11]='0';
	     rekord[12]=string[0];
	   };
	  rekord[13]=':';

      itoa(minuta, string, 10); // zamiana minuty na string
	  if (minuta>9)
	   {
	     rekord[14]=string[0];
	     rekord[15]=string[1];
       }
	   else
	   {
	     rekord[14]='0';
	     rekord[15]=string[0];
	   };
	  rekord[16]=':';

      itoa(sek, string, 10); // zamiana sekundy na string
	  if (sek>9)
	   {
	     rekord[17]=string[0];
	     rekord[18]=string[1];
       }
	   else
	   {
	     rekord[17]='0';
	     rekord[18]=string[0];
	   };
	  rekord[19]=' ';
}

//********************************************************
//*     Podprogram redagowania i zapisu rekordu na SD    *
//********************************************************
void zapisz_SD()
{
  czas_data_do_rekordu();
  LCD_Clear();
  LCD_WriteText("Zapis rekordu");
  LCD_GoTo(0,1);

  switch (we_wy)
  {
    case 1: //ustawiæ WE - wejœcie z zewn¹trz do mieszkania 
	  rekord[20]='W';
	  rekord[21]='E';
      LCD_WriteText("WE");
	  break;

    case 2: //ustawiæ WY - wyjœcie na zewn¹trz z mieszkania
	  rekord[20]='W';
	  rekord[21]='Y';
      LCD_WriteText("WY");
	  break;

    case 3: //ustawiæ OZ - otwarcie i zamkniêcie pary drzwi z zewn¹trz
	  rekord[20]='O';
	  rekord[21]='Z';
      LCD_WriteText("OZ");
	  break;

    case 4: //ustawiæ OW - otwarcie i zamkniêcie pary drzwi z wewn¹trz
	  rekord[20]='O';
	  rekord[21]='W';
      LCD_WriteText("OW");
	  break;

    case 5: //ustawiæ OX - otwarcie i zamkniêcie tylko drzwi zewnêtrznych
	  rekord[20]='O';
	  rekord[21]='X';
      LCD_WriteText("OX");
	  break;

    case 6: //ustawiæ OY - otwarcie i zamkniêcie tylko drzwi wewnêtrznych
	  rekord[20]='O';
	  rekord[21]='Y';
      LCD_WriteText("OY");
	  break;
  }

  dopisz_rekord(rekord);

  _delay_ms(2000);

  we_wy=0;

//  beep_3();
}

//********************************************************
//*  Podprogram sprawdzania czy zapisywaæ rekord na SD   *
//********************************************************
void spr_czy_zapis_rekordu()
{
  if (tryb_zapisu==1)
  {
     if (!(we_wy==0))
	 {
        zapisz_SD(); //zredagowanie i dopisanie rekordu do SD
     };
  };
}

//========================================================
//=====              Pocz¹tek programu               =====
//========================================================
int main(void)
{
  init_devices(); 	//inicjowanie peryfera³ów
  inicjuj_SD();

  beep_3();

  PCF8563_init();	// inicjowanie ukladu PCF8563

  tryb_zapisu=1;  

  while(1)
  {
	if (!(zmiana==0))
	{ 
	  zmiana=0;
      PCF8563_set_time(godzina,minuta,0);
	  PCF8563_set_date(dzien,miesiac,rok);
	  beep_3();
	};

    if(!(PINA & 0x08)) // przycisk S3
     {  
	    beep();
        while(!(PINA & 0x08)) {}; //czekanie na puszczenie klawisza
        _delay_ms(50);
		tryb_ustawiania=0; //rok
	    ust_zegara();
     };

    if(!(PINA & 0x10)) // przycisk S2
     {  
	    beep();
        while(!(PINA & 0x10)) {}; //czekanie na puszczenie klawisza
        _delay_ms(50);
	    ust_zapisu();
     };

    if(zmiana==0)
	{
       obsluga_PCF8563();    //wyœwietlanie czasu GG:MM:SS i daty DD/MM/ROK na LCD
    };
    sprawdzenie_drzwi();
    spr_czy_zapis_rekordu();

  } //while
} //main






