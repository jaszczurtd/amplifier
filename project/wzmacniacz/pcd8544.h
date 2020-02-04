
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

#include "utils.h"

#define S_WIDTH 14
#define S_HEIGHT 6

#define BUF_L ((S_WIDTH + 1) * (S_HEIGHT + 1)) + 1

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
void PCD_Img        ( const unsigned char *imageData );
void PCD_Contr      ( unsigned char contrast);
unsigned char PCD_GotoXYFont ( unsigned char x, unsigned char y );
unsigned char PCD_Chr        ( LcdFontSize size, unsigned char ch );
unsigned char PCD_Str        ( LcdFontSize size, unsigned char dataArray[] );
unsigned char PCD_FStr       ( LcdFontSize size, const unsigned char *dataPtr );
unsigned char PCD_print      ( LcdFontSize size, unsigned char *dataPtr );
unsigned char PCD_Pixel      ( unsigned char x, unsigned char y, LcdPixelMode mode );
unsigned char PCD_Line       ( unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2, LcdPixelMode mode );
unsigned char PCD_Rect       ( unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2, LcdPixelMode mode );
unsigned char PCD_SBar       ( unsigned char baseX, unsigned char baseY, unsigned char height, unsigned char width, LcdPixelMode mode );
unsigned char PCD_Bars       ( unsigned char data[], unsigned char numbBars, unsigned char width, unsigned char multiplier );

#endif  /*  _PCD8544_H_ */
