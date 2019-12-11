/*
** ===================================================================
** N�zev souboru   : lcd_c.c
** Mikrokontroler  : ATmega16
** Popis           : Hlavi�kov� soubor pro komunikaci s lcd displejem
** Autor           : Peter Fleury ( http://homepage.hispeed.ch/peterfleury/ )
**                 : Tom� Fr�za ( fryza@feec.vutbr.cz )
**		   		   : Thomas Breining (breinint@esiee.fr)
** ===================================================================
*/
#ifndef _LCD_H
#define _LCD_H 1


/*
** Konstanty
*/
#define LCD_PORT		PORTA	// v�stupn� port pro lcd displej
#define LCD_RW_PIN		2		// r/w vstup displeje
#define LCD_E_PIN		3		// enable vstup displeje
#define LCD_RS_PIN		1		// rs vstup displeje
#define LCD_DISP_LENGTH	16		// po�et znak� na ��dek displeje

#define LCD_DDRAM		7
#define LCD_START_LINE1	0x00	// adresa prvn�ho znaku na prvn�m ��dku
#define LCD_START_LINE2	0x40	// adresa prvn�ho znaku na druh�m ��dku
#define LCD_START_LINE3 0x10
#define LCD_START_LINE4	0x50

#define DDR( x )		( *(&x - 1) )	// adresa sm�rov�ho registru portu
#define lcd_rw_high()	LCD_PORT |=  _BV( LCD_RW_PIN )
#define lcd_rw_low()	LCD_PORT &= ~_BV( LCD_RW_PIN )
#define lcd_e_delay()	__asm__ __volatile__( "nop\n nop\n nop" )
#define lcd_e_high()	LCD_PORT  |=  _BV( LCD_E_PIN )
#define lcd_e_low()		LCD_PORT  &= ~_BV( LCD_E_PIN )
#define lcd_e_toggle()	toggle_e()
#define lcd_rs_high()	LCD_PORT |=  _BV( LCD_RS_PIN )
#define lcd_rs_low()	LCD_PORT &= ~_BV( LCD_RS_PIN )


/*
** Prototypy funkc�
*/
void lcd_init( void ) ;			// inicializace lcd displeje
void lcd_clrscr( void ) ;		// vymaz�n� obsahu displeje
void lcd_command( char ) ;		// z�pis 1bytov� instrukce do lcd displeje
void lcd_data( char ) ;			// z�pis 1bytov�ho datov�ho slova do lcd displeje
void lcd_write( char, char ) ;	// z�pis instrukce nebo dat do lcd displeje
void toggle_e( void ) ;			// realizace jednoho "enable" pulsu pro lcd displej
void lcd_gotoxy( char, char ) ;	// p�esun kurzoru na pozici x,y lcd displeje
void lcd_putc( char c ) ;		// zobrazen� jednoho znaku na lcd displeji
void lcd_puts( const char *s ) ;// zobrazen� �et�zce znak� na lcd displeji
void lcd_newline( void ) ;		// skok na nov� ��dek lcd displeje
void lcd_firstline( void ) ;	// skok a vymaz�n� prvn�ho ��dku lcd displeje
void lcd_secondline( void ) ;	// skok a vymaz�n� druh�ho ��dku lcd displeje
void lcd_thirdline( void ) ;
void lcd_fourthline( void ) ;

#endif


/*
** Konec hlavi�kov�ho souboru
*/
