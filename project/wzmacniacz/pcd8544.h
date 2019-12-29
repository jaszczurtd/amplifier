
/*
 * Biblioteka dla LCD Nokia 3310 i innych
 * opartych o sterownik pcd8544.h
 *
 */

#ifndef _PCD8544_H_
#define _PCD8544_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


#include "utils.h"

// --------------- Wartoci zwrotne
#define OK                         0
#define OUT_OF_BORDER              1
#define OK_WITH_WRAP               2


#define LCD_X_RES                  84    /* rozdzielczosc  x */
#define LCD_Y_RES                  48    /* rozdzielczosc  y */
#define EMPTY_SPACE_BARS           2
#define BAR_X                      5
#define BAR_Y                      38

// --------------- LCD Port
#define LCD_PORT                   PORTB
#define LCD_DDR                    DDRB

// --------------- ATMega32 podłaczenie pinów

#define LCD_DC_PIN                 PB1  // DC
#define LCD_CE_PIN                 PB4  // CE
#define SPI_MOSI_PIN               PB5  // MOSI
#define SPI_CLK_PIN                PB7  // CLK

// --------------- Pamięc Podręczna dla LCD ( 84 * 48 ) / 8 = 504 bytes
#define LCD_CACHE_SIZE             ( ( LCD_X_RES * LCD_Y_RES ) / 8)

// --------------- Definicje Typów
typedef unsigned char              byte;

// --------------- Enumeratorki
typedef enum {
    LCD_CMD  = 0,
    LCD_DATA = 1
    
} LcdCmdData;

typedef enum {
    PIXEL_OFF =  0,
    PIXEL_ON  =  1,
    PIXEL_XOR =  2
    
} LcdPixelMode;

typedef enum {
    FONT_1X = 1,
    FONT_2X = 2
    
} LcdFontSize;

// --------------- Prototypy funkcji
void PCD_Ini        ( void );
void PCD_Clr        ( void );
void PCD_Upd        ( void );
void PCD_Img        ( const byte *imageData );
void PCD_Contr      ( byte contrast);
byte PCD_GotoXYFont ( byte x, byte y );
byte PCD_Chr        ( LcdFontSize size, byte ch );
byte PCD_Str        ( LcdFontSize size, byte dataArray[] );
byte PCD_FStr       ( LcdFontSize size, const byte *dataPtr );
byte PCD_print      ( LcdFontSize size, byte *dataPtr );
byte PCD_Pixel      ( byte x, byte y, LcdPixelMode mode );
byte PCD_Line       ( byte x1, byte x2, byte y1, byte y2, LcdPixelMode mode );
byte PCD_Rect       ( byte x1, byte x2, byte y1, byte y2, LcdPixelMode mode );
byte PCD_SBar       ( byte baseX, byte baseY, byte height, byte width, LcdPixelMode mode );
byte PCD_Bars       ( byte data[], byte numbBars, byte width, byte multiplier );



#endif  /*  _PCD8544_H_ */
