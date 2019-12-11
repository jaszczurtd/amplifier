/*
** ===================================================================
** Název souboru   : lcd_c.c
** Mikrokontroler  : ATmega16
** Popis           : Funkce pro ovládání lcd displeje
** Autor           : Peter Fleury ( http://homepage.hispeed.ch/peterfleury/ )
**                 : Tomáš Frýza ( fryza@feec.vutbr.cz )
**		   		   : Thomas Breining (breinint@esiee.fr)
** ===================================================================
*/


/*
** Hlavièkové soubory
*/
#include <avr/io.h>			// hlavièkový soubor pro mikrokontrolér ATmega16
#define F_CPU 16000000UL	// frekvence hodinového signálu (nutné pro delay.h)
#include <util/delay.h>     // hlavièkový soubor pro zpoždìní
#include "lcd_h.h"          // hlavièkový soubor pro obsluhu lcd


/*
** Globální promìnné
*/
unsigned char x,y ;			// pomocné souøadnice pozice kurzoru



/*
** ===================================================================
**     Název funkce  : lcd_clrscr
**
**     Popis         :
**         Smazání obsahu lcd displeje.
**     Parametry     : žádné
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_clrscr( void )
{
	lcd_command( 0x00000001 ) ;
	x = 0 ;
	y = 0 ;
	_delay_ms( 16 ) ;
}
/*
** ===================================================================
**     Název funkce  : lcd_command
**
**     Popis         :
**         Zápis 1bytové instrukce do lcd displeje.
**     Parametry     : data - instrukce pro lcd diplej
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_command( char data )
{
	lcd_write( data, 0 ) ;
}
/*
** ===================================================================
**     Název funkce  : lcd_data
**
**     Popis         :
**         Zápis 1bytového datového slova do lcd displeje.
**     Parametry     : data - data pro lcd displej
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_data( char data )
{
	lcd_write( data, 1 ) ;
}
/*
** ===================================================================
**     Název funkce  : lcd_write
**
**     Popis         :
**         Zápis instrukce nebo dat do lcd displeje.
**     Parametry     : data - instrukce/data pro lcd diplej
**                     rs   - 0: instrukce; 1: data
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_write( char data, char rs )
{
	unsigned char temp ;

    if( rs ) lcd_rs_high() ;
	else lcd_rs_low() ;

	temp = LCD_PORT & 0x0f ;
    LCD_PORT = temp | ( data & 0xf0 ) ;
    lcd_e_toggle() ;
    LCD_PORT = temp | (( data & 0x0f ) << 4 ) ;
    lcd_e_toggle() ;
    LCD_PORT = temp | 0xf0 ;
    _delay_ms( 2 ) ;
}
/*
** ===================================================================
**     Název funkce  : toggle_e
**
**     Popis         :
**         Realizace jednoho "enable" pulsu pro lcd displej.
**     Parametry     : žádné
**     Výst. hodnota : žádné
** ===================================================================
*/
void toggle_e( void )
{
    lcd_e_high() ;
    lcd_e_delay() ;
    lcd_e_low() ;
}
/*
** ===================================================================
**     Název funkce  : lcd_gotoxy
**
**     Popis         :
**         Pøesun kurzoru na pozici x,y lcd displeje.
**     Parametry     : x_,y_ - nové pozice kurzoru lcd displeje
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_gotoxy( char x_, char y_ )
{
	if( y_ == 0 )
    {	lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE1 + x_ ) ;
		y = 0 ;
	}
    else if( y_ == 1 )
    {	lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE2 + x_ ) ;
		y = 1 ;
	}
	else if( y_ == 2 )
    {	lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE3 + x_ ) ;
		y = 2 ;
	}
	else if( y_ == 3 )
    {	lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE4 + x_ ) ;
		y = 3 ;
	}
	x = x_ ;
}
/*
** ===================================================================
**     Název funkce  : lcd_putc
**
**     Popis         :
**         Zobrazení jednoho znaku na lcd displeji.
**     Parametry     : c - znak k zobrazení na lcd displeji
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_putc( char c )
{
    if (c=='\n')
		lcd_newline() ;
	else
	{
        if(( x == LCD_DISP_LENGTH ) && ( y == 0 ))
			lcd_secondline() ;
		else if(( x == LCD_DISP_LENGTH ) && ( y == 1 ))
			lcd_thirdline() ;
		else if(( x == LCD_DISP_LENGTH ) && ( y == 2 ))
			lcd_fourthline() ;
		else if(( x == LCD_DISP_LENGTH ) && ( y == 3 ))
			lcd_firstline() ;
	    lcd_data( c ) ;
		x++ ;
	}
}


/*
** ===================================================================
**     Název funkce  : lcd_puts
**
**     Popis         :
**         Zobrazení øetìzce znakù na lcd displeji.
**     Parametry     : *s - ukazatel na øetìzec znakù
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_puts( const char *s )
{
    register char c ;

    while(( c = *s++ ))
        lcd_putc( c ) ;
}


/*
** ===================================================================
**     Název funkce  : lcd_newline
**
**     Popis         :
**         Skok na nový øádek lcd displeje.
**     Parametry     : žádné
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_newline( void )
{
	if( y == 3 && x == 0)
		lcd_firstline() ;
	else if( y == 0 && x == 0)
		lcd_secondline() ;
	else if( y == 1 && x == 0)
		lcd_thirdline() ;
	else if( y == 2 && x == 0)
		lcd_fourthline() ;
}


/*
** ===================================================================
**     Název funkce  : lcd_firstline
**
**     Popis         :
**         Skok a vymazání prvního øádku lcd displeje.
**     Parametry     : žádné
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_firstline( void )
{
    lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE1 ) ;
	x = 0 ;
	y = 0 ;
	lcd_puts( "                " ) ;
    lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE1 ) ;
	x = 0 ;
	y = 0 ;
}


/*
** ===================================================================
**     Název funkce  : lcd_secondline
**
**     Popis         :
**         Skok a vymazání druhého øádku lcd displeje.
**     Parametry     : žádné
**     Výst. hodnota : žádné
** ===================================================================
*/
void lcd_secondline( void )
{
    lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE2 ) ;
	x = 0 ;
	y = 1 ;
	lcd_puts( "                " ) ;
    lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE2 ) ;
	x = 0 ;
	y = 1 ;
}

void lcd_thirdline( void )
{
    lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE3 ) ;
	x = 0 ;
	y = 2 ;
	lcd_puts( "                " ) ;
    lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE3 ) ;
	x = 0 ;
	y = 2 ;
}

void lcd_fourthline( void )
{
    lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE4 ) ;
	x = 0 ;
	y = 3 ;
	lcd_puts( "                " ) ;
    lcd_command(( 1<<LCD_DDRAM ) + LCD_START_LINE4 ) ;
	x = 0 ;
	y = 3 ;
}

void lcd_init( void )
{
	DDRA = 0xfe ;				
    _delay_ms( 16 ) ;			
	lcd_command( 0b00100000 ) ; 
	lcd_command( 0b00101000 ) ;
	lcd_command( 0b00000100 ) ;
	lcd_command( 0b00001100 ) ;
	lcd_clrscr() ;
}

/*
** Konec zdrojového souboru
*/
