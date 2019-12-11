/*
** ===================================================================
** N�zev souboru   : lcd_c.c
** Mikrokontroler  : ATmega16
** Popis           : Funkce pro ovl�d�n� lcd displeje
** Autor           : Peter Fleury ( http://homepage.hispeed.ch/peterfleury/ )
**                 : Tom� Fr�za ( fryza@feec.vutbr.cz )
**		   		   : Thomas Breining (breinint@esiee.fr)
** ===================================================================
*/


/*
** Hlavi�kov� soubory
*/
#include <avr/io.h>			// hlavi�kov� soubor pro mikrokontrol�r ATmega16
#define F_CPU 16000000UL	// frekvence hodinov�ho sign�lu (nutn� pro delay.h)
#include <util/delay.h>     // hlavi�kov� soubor pro zpo�d�n�
#include "lcd_h.h"          // hlavi�kov� soubor pro obsluhu lcd


/*
** Glob�ln� prom�nn�
*/
unsigned char x,y ;			// pomocn� sou�adnice pozice kurzoru



/*
** ===================================================================
**     N�zev funkce  : lcd_clrscr
**
**     Popis         :
**         Smaz�n� obsahu lcd displeje.
**     Parametry     : ��dn�
**     V�st. hodnota : ��dn�
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
**     N�zev funkce  : lcd_command
**
**     Popis         :
**         Z�pis 1bytov� instrukce do lcd displeje.
**     Parametry     : data - instrukce pro lcd diplej
**     V�st. hodnota : ��dn�
** ===================================================================
*/
void lcd_command( char data )
{
	lcd_write( data, 0 ) ;
}
/*
** ===================================================================
**     N�zev funkce  : lcd_data
**
**     Popis         :
**         Z�pis 1bytov�ho datov�ho slova do lcd displeje.
**     Parametry     : data - data pro lcd displej
**     V�st. hodnota : ��dn�
** ===================================================================
*/
void lcd_data( char data )
{
	lcd_write( data, 1 ) ;
}
/*
** ===================================================================
**     N�zev funkce  : lcd_write
**
**     Popis         :
**         Z�pis instrukce nebo dat do lcd displeje.
**     Parametry     : data - instrukce/data pro lcd diplej
**                     rs   - 0: instrukce; 1: data
**     V�st. hodnota : ��dn�
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
**     N�zev funkce  : toggle_e
**
**     Popis         :
**         Realizace jednoho "enable" pulsu pro lcd displej.
**     Parametry     : ��dn�
**     V�st. hodnota : ��dn�
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
**     N�zev funkce  : lcd_gotoxy
**
**     Popis         :
**         P�esun kurzoru na pozici x,y lcd displeje.
**     Parametry     : x_,y_ - nov� pozice kurzoru lcd displeje
**     V�st. hodnota : ��dn�
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
**     N�zev funkce  : lcd_putc
**
**     Popis         :
**         Zobrazen� jednoho znaku na lcd displeji.
**     Parametry     : c - znak k zobrazen� na lcd displeji
**     V�st. hodnota : ��dn�
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
**     N�zev funkce  : lcd_puts
**
**     Popis         :
**         Zobrazen� �et�zce znak� na lcd displeji.
**     Parametry     : *s - ukazatel na �et�zec znak�
**     V�st. hodnota : ��dn�
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
**     N�zev funkce  : lcd_newline
**
**     Popis         :
**         Skok na nov� ��dek lcd displeje.
**     Parametry     : ��dn�
**     V�st. hodnota : ��dn�
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
**     N�zev funkce  : lcd_firstline
**
**     Popis         :
**         Skok a vymaz�n� prvn�ho ��dku lcd displeje.
**     Parametry     : ��dn�
**     V�st. hodnota : ��dn�
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
**     N�zev funkce  : lcd_secondline
**
**     Popis         :
**         Skok a vymaz�n� druh�ho ��dku lcd displeje.
**     Parametry     : ��dn�
**     V�st. hodnota : ��dn�
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
** Konec zdrojov�ho souboru
*/
