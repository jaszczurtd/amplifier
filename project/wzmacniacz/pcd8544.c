
/*
 * Biblioteka dla LCD Nokia 3310 i innych
 * opartych o sterownik pcd8544.h
 *
 * SunRiver
 *
 * przygotowane dla toolchaina ATMEL 3.4.x
 *
 */

#include "pcd8544.h"

char s[BUF_L];

static void PCD_Snd    ( unsigned char data, LcdCmdData cd );
static void Delay      ( void );

// --------------------------  bufor cache w SRAM 84*48 bits or 504 bytes
static unsigned char  LcdCache [ LCD_CACHE_SIZE ];
static int   LcdCacheIdx;
static int   LoWaterMark;
static int   HiWaterMark;
static bool  UpdateLcd;

// Tablica znaków 5x7 .....

static const unsigned char PROGMEM FontLookup [][5] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 },   /* spacja */
    { 0x00, 0x00, 0x2f, 0x00, 0x00 },   /* ! */
    { 0x00, 0x07, 0x00, 0x07, 0x00 },   /* " */
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   /* # */
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   /* $ */
    { 0xc4, 0xc8, 0x10, 0x26, 0x46 },   /* % */
    { 0x36, 0x49, 0x55, 0x22, 0x50 },   /* & */
    { 0x00, 0x05, 0x03, 0x00, 0x00 },   /* ' */
    { 0x00, 0x1c, 0x22, 0x41, 0x00 },   /* ( */
    { 0x00, 0x41, 0x22, 0x1c, 0x00 },   /* ) */
    { 0x14, 0x08, 0x3E, 0x08, 0x14 },   /* * */
    { 0x08, 0x08, 0x3E, 0x08, 0x08 },   /* + */
    { 0x00, 0x00, 0x50, 0x30, 0x00 },   /* , */
    { 0x10, 0x10, 0x10, 0x10, 0x10 },   /* - */
    { 0x00, 0x60, 0x60, 0x00, 0x00 },   /* . */
    { 0x20, 0x10, 0x08, 0x04, 0x02 },   /* / */
    { 0x3E, 0x51, 0x49, 0x45, 0x3E },   /* 0 */
    { 0x00, 0x42, 0x7F, 0x40, 0x00 },   /* 1 */
    { 0x42, 0x61, 0x51, 0x49, 0x46 },   /* 2 */
    { 0x21, 0x41, 0x45, 0x4B, 0x31 },   /* 3 */
    { 0x18, 0x14, 0x12, 0x7F, 0x10 },   /* 4 */
    { 0x27, 0x45, 0x45, 0x45, 0x39 },   /* 5 */
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   /* 6 */
    { 0x01, 0x71, 0x09, 0x05, 0x03 },   /* 7 */
    { 0x36, 0x49, 0x49, 0x49, 0x36 },   /* 8 */
    { 0x06, 0x49, 0x49, 0x29, 0x1E },   /* 9 */
    { 0x00, 0x36, 0x36, 0x00, 0x00 },   /* : */
    { 0x00, 0x56, 0x36, 0x00, 0x00 },   /* ; */
    { 0x08, 0x14, 0x22, 0x41, 0x00 },   /* < */
    { 0x14, 0x14, 0x14, 0x14, 0x14 },   /* = */
    { 0x00, 0x41, 0x22, 0x14, 0x08 },   /* > */
    { 0x02, 0x01, 0x51, 0x09, 0x06 },   /* ? */
    { 0x32, 0x49, 0x59, 0x51, 0x3E },   /* @ */
    { 0x7E, 0x11, 0x11, 0x11, 0x7E },   /* A */
    { 0x7F, 0x49, 0x49, 0x49, 0x36 },   /* B */
    { 0x3E, 0x41, 0x41, 0x41, 0x22 },   /* C */
    { 0x7F, 0x41, 0x41, 0x22, 0x1C },   /* D */
    { 0x7F, 0x49, 0x49, 0x49, 0x41 },   /* E */
    { 0x7F, 0x09, 0x09, 0x09, 0x01 },   /* F */
    { 0x3E, 0x41, 0x49, 0x49, 0x7A },   /* G */
    { 0x7F, 0x08, 0x08, 0x08, 0x7F },   /* H */
    { 0x00, 0x41, 0x7F, 0x41, 0x00 },   /* I */
    { 0x20, 0x40, 0x41, 0x3F, 0x01 },   /* J */
    { 0x7F, 0x08, 0x14, 0x22, 0x41 },   /* K */
    { 0x7F, 0x40, 0x40, 0x40, 0x40 },   /* L */
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   /* M */
    { 0x7F, 0x04, 0x08, 0x10, 0x7F },   /* N */
    { 0x3E, 0x41, 0x41, 0x41, 0x3E },   /* O */
    { 0x7F, 0x09, 0x09, 0x09, 0x06 },   /* P */
    { 0x3E, 0x41, 0x51, 0x21, 0x5E },   /* Q */
    { 0x7F, 0x09, 0x19, 0x29, 0x46 },   /* R */
    { 0x46, 0x49, 0x49, 0x49, 0x31 },   /* S */
    { 0x01, 0x01, 0x7F, 0x01, 0x01 },   /* T */
    { 0x3F, 0x40, 0x40, 0x40, 0x3F },   /* U */
    { 0x1F, 0x20, 0x40, 0x20, 0x1F },   /* V */
    { 0x3F, 0x40, 0x38, 0x40, 0x3F },   /* W */
    { 0x63, 0x14, 0x08, 0x14, 0x63 },   /* X */
    { 0x07, 0x08, 0x70, 0x08, 0x07 },   /* Y */
    { 0x61, 0x51, 0x49, 0x45, 0x43 },   /* Z */
    { 0x00, 0x7F, 0x41, 0x41, 0x00 },   /* [ */
    { 0x55, 0x2A, 0x55, 0x2A, 0x55 },   /* "Yen" */
    { 0x00, 0x41, 0x41, 0x7F, 0x00 },   /* ] */
    { 0x04, 0x02, 0x01, 0x02, 0x04 },   /* ^ */
    { 0x40, 0x40, 0x40, 0x40, 0x40 },   /* _ */
    { 0x00, 0x01, 0x02, 0x04, 0x00 },   /* ' */
    { 0x20, 0x54, 0x54, 0x54, 0x78 },   /* a */
    { 0x7F, 0x48, 0x44, 0x44, 0x38 },   /* b */
    { 0x38, 0x44, 0x44, 0x44, 0x20 },   /* c */
    { 0x38, 0x44, 0x44, 0x48, 0x7F },   /* d */
    { 0x38, 0x54, 0x54, 0x54, 0x18 },   /* e */
    { 0x08, 0x7E, 0x09, 0x01, 0x02 },   /* f */
    { 0x0C, 0x52, 0x52, 0x52, 0x3E },   /* g */
    { 0x7F, 0x08, 0x04, 0x04, 0x78 },   /* h */
    { 0x00, 0x44, 0x7D, 0x40, 0x00 },   /* i */
    { 0x20, 0x40, 0x44, 0x3D, 0x00 },   /* j */
    { 0x7F, 0x10, 0x28, 0x44, 0x00 },   /* k */
    { 0x00, 0x41, 0x7F, 0x40, 0x00 },   /* l */
    { 0x7C, 0x04, 0x18, 0x04, 0x78 },   /* m */
    { 0x7C, 0x08, 0x04, 0x04, 0x78 },   /* n */
    { 0x38, 0x44, 0x44, 0x44, 0x38 },   /* o */
    { 0x7C, 0x14, 0x14, 0x14, 0x08 },   /* p */
    { 0x08, 0x14, 0x14, 0x18, 0x7C },   /* q */
    { 0x7C, 0x08, 0x04, 0x04, 0x08 },   /* r */
    { 0x48, 0x54, 0x54, 0x54, 0x20 },   /* s */
    { 0x04, 0x3F, 0x44, 0x40, 0x20 },   /* t */
    { 0x3C, 0x40, 0x40, 0x20, 0x7C },   /* u */
    { 0x1C, 0x20, 0x40, 0x20, 0x1C },   /* v */
    { 0x3C, 0x40, 0x30, 0x40, 0x3C },   /* w */
    { 0x44, 0x28, 0x10, 0x28, 0x44 },   /* x */
    { 0x0C, 0x50, 0x50, 0x50, 0x3C },   /* y */
    { 0x44, 0x64, 0x54, 0x4C, 0x44 },   /* z */
    { 0x00, 0x08, 0x36, 0x41, 0x00 }, // 7b {
    { 0x00, 0x00, 0x7f, 0x00, 0x00 }, // 7c |
    { 0x00, 0x41, 0x36, 0x08, 0x00 }, // 7d }
    { 0x10, 0x08, 0x08, 0x10, 0x08 }, // 7e ->
    { 0x78, 0x46, 0x41, 0x46, 0x78 }, // 7f <-

};

void PCD_Ini ( void ) {
    
    //reset display
    sbi(DDRA, PA7);
    sbi(PORTA, PA7);
    Delay();
    cbi(PORTA, PA7);
    Delay();
    sbi(PORTA, PA7);

    // Włączenie SPI:
    // Bez przerwań , MSBit jak pierwszy, Master mode, CPOL->0, CPHA->0, Clk/4
    // Ustawienie pinów
    LCD_DDR |= _BV( LCD_DC_PIN ) | _BV( LCD_CE_PIN ) | _BV( SPI_MOSI_PIN ) | _BV( SPI_CLK_PIN );

    Delay();
    
    SPCR = 0x50;
    
    // Wyłaczenie kontrolera LCD
    LCD_PORT |= _BV( LCD_CE_PIN );
    
    PCD_Snd( 0x21, LCD_CMD ); //* Rozszerzone rozkazy LCD.
    PCD_Snd( 0xC8, LCD_CMD ); //* Ustawienie LCD Vop (Kontrast).
    PCD_Snd( 0x06, LCD_CMD ); //* Ustawienie stabilizacji Temp.
    PCD_Snd( 0x13, LCD_CMD ); //* LCD tryb bias 1:48.
    PCD_Snd( 0x20, LCD_CMD ); //* LCD Standard Commands,Horizontal addressing mode
    PCD_Snd( 0x0C, LCD_CMD ); //* LCD w tryb normal mode.
    
    
    LoWaterMark = LCD_CACHE_SIZE;
    HiWaterMark = 0;
    
    // Czyszczenie ekranu przed użyciem
    PCD_Clr();
    PCD_Upd();
}

// PCD_Contr
// Ustawia kontrast LCD
// Wartosc  w zakresie 0x00 do 0x7F.

void PCD_Contr ( unsigned char contrast ) {
    PCD_Snd( 0x21, LCD_CMD );
    
    // Ustawienie poziomu kontrastu
    PCD_Snd( 0x80 | contrast, LCD_CMD );
    
    // Tryb - horizontal addressing mode.
    PCD_Snd( 0x20, LCD_CMD );
}

// PCD_Clr
// Czyci LCD
void PCD_Clr ( void ) {
    memset(LcdCache, 0x00, sizeof(LcdCache));
    
    LoWaterMark = 0;
    HiWaterMark = LCD_CACHE_SIZE - 1;
    
    // Ustawienie flagi na true
    UpdateLcd = TRUE;
}

// PCD_GotoXYFont
// Ustawienie kursora z uwzglednieniem bazowej czcionki 5x7
unsigned char PCD_GotoXYFont ( unsigned char x, unsigned char y ) {
    if( x > 14)
        return OUT_OF_BORDER;
    if( y > 6)
        return OUT_OF_BORDER;
    // Kalkulacja indexu.
    
    LcdCacheIdx = (x * 6) + (y * 84);
    return OK;
}

// PCD_Chr
// Wyswietla znaki

unsigned char PCD_Chr ( LcdFontSize size, unsigned char ch ) {
    unsigned char i, c;
    unsigned char b1, b2;
    int  tmpIdx;
    
    if ( LcdCacheIdx < LoWaterMark ) {
        
        LoWaterMark = LcdCacheIdx;
    }
    
    if ( (ch < 0x20) || (ch > 0x7b) ) {
        // konwersja znaku do wyswietlenia
        ch = 92;
    }
    
    if ( size == FONT_1X ) {
        for ( i = 0; i < 5; i++ ) {
            // kopiowanie tablicy z Flash ROM do LcdCache
            LcdCache[LcdCacheIdx++] = pgm_read_byte(&( FontLookup[ ch - 32 ][ i ] ) ) << 1;
        }
    }
    else if ( size == FONT_2X ) {
        tmpIdx = LcdCacheIdx - 84;
        
        if ( tmpIdx < LoWaterMark ) {
            LoWaterMark = tmpIdx;
        }
        
        if ( tmpIdx < 0 ) return OUT_OF_BORDER;
        
        for ( i = 0; i < 5; i++ ) {
            /* Copy lookup table from Flash ROM to temporary c */
            c = pgm_read_byte(&(FontLookup[ch - 32][i])) << 1;
            // duży obraz
            // pierwsza częsc
            b1 =  (c & 0x01) * 3;
            b1 |= (c & 0x02) * 6;
            b1 |= (c & 0x04) * 12;
            b1 |= (c & 0x08) * 24;
            
            c >>= 4;
            // druga czesc
            b2 =  (c & 0x01) * 3;
            b2 |= (c & 0x02) * 6;
            b2 |= (c & 0x04) * 12;
            b2 |= (c & 0x08) * 24;
            
            /* kopiowanie obu czesci do LcdCache */
            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx + 82] = b2;
            LcdCache[tmpIdx + 83] = b2;
        }
        
        // aktualizacja połozenia X
        LcdCacheIdx = (LcdCacheIdx + 11) % LCD_CACHE_SIZE;
    }
    
    if ( LcdCacheIdx > HiWaterMark ) {
        HiWaterMark = LcdCacheIdx;
    }
    
    LcdCache[LcdCacheIdx] = 0x00;
    if(LcdCacheIdx == (LCD_CACHE_SIZE - 1) ) {
        LcdCacheIdx = 0;
        return OK_WITH_WRAP;
    }
    // Inkrementacja indexu
    LcdCacheIdx++;
    return OK;
}

// PCD_Str

unsigned char PCD_Str ( LcdFontSize size, unsigned char dataArray[] ) {
    unsigned char tmpIdx=0;
    unsigned char response;
    while( dataArray[ tmpIdx ] != '\0' ) {
        // wysłanie znaku
        response = PCD_Chr( size, dataArray[ tmpIdx ] );
        // OUT_OF_BORDER
        if( response == OUT_OF_BORDER)
            return OUT_OF_BORDER;
        tmpIdx++;
    }
    return OK;
}

// PCD_FStr
// Wyswietla String
// Przykład uzycia :  LcdFStr(FONT_1X, PSTR("Witaj"));

unsigned char PCD_FStr ( LcdFontSize size, const unsigned char *dataPtr ) {
    unsigned char c;
    unsigned char response;
    for ( c = pgm_read_byte( dataPtr ); c; ++dataPtr, c = pgm_read_byte( dataPtr ) ) {
        
        response = PCD_Chr( size, c );
        if(response == OUT_OF_BORDER)
            return OUT_OF_BORDER;
    }
    return OK;
}

unsigned char PCD_print ( LcdFontSize size, unsigned char *dataPtr ) {
    unsigned char c;
    unsigned char response;
    unsigned char a = 0;
    
    while ((c = dataPtr[a++]) != '\0') {
        response = PCD_Chr( size, c );
        if(response == OUT_OF_BORDER)
            return OUT_OF_BORDER;
    }
    
    return OK;
}


// PCD_Pixel
// Wyswietla pixel o zadanych współrzędnych X, Y

unsigned char PCD_Pixel ( unsigned char x, unsigned char y, LcdPixelMode mode ) {
    int  index;
    unsigned char  offset;
    unsigned char  data;
    
    // obliczenie ramek
    if ( x > LCD_X_RES ) return OUT_OF_BORDER;
    if ( y > LCD_Y_RES ) return OUT_OF_BORDER;
    
    // rekalkulacja ofsetu
    index = ( ( y / 8 ) * 84 ) + x;
    offset  = y - ( ( y / 8 ) * 8 );
    
    data = LcdCache[ index ];
    
    // Czyszczenie
    if ( mode == PIXEL_OFF ) {
        data &= ( ~( 0x01 << offset ) );
    }
    
    // tryb Właczony
    else if ( mode == PIXEL_ON ) {
        data |= ( 0x01 << offset );
    }
    
    // Tryb Xor
    else if ( mode  == PIXEL_XOR ) {
        data ^= ( 0x01 << offset );
    }
    
    // kopiowanie rezultatu do cache
    LcdCache[ index ] = data;
    
    if ( index < LoWaterMark ) {
        LoWaterMark = index;
    }
    
    if ( index > HiWaterMark ) {
        HiWaterMark = index;
    }
    return OK;
}


// PCD_Line
// Pozwala na rysowanie lini  o zadanych współrzędnych

unsigned char PCD_Line ( unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2, LcdPixelMode mode ) {
    int dx, dy, stepx, stepy, fraction;
    unsigned char response;
    
    dy = y2 - y1;
    dx = x2 - x1;
    
    if ( dy < 0 ) {
        dy    = -dy;
        stepy = -1;
    } else {
        stepy = 1;
    }
    
    // DX negatyw
    if ( dx < 0 ) {
        dx    = -dx;
        stepx = -1;
    } else {
        stepx = 1;
    }
    
    dx <<= 1;
    dy <<= 1;
    
    // rysowanie na pozycji
    response = PCD_Pixel( x1, y1, mode );
    if(response)
        return response;
    
    // zmiana lub koniec
    if ( dx > dy ) {
        //frakcja
        fraction = dy - ( dx >> 1);
        while ( x1 != x2 ) {
            if ( fraction >= 0 ) {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;
            
            // rysowanie punktu
            response = PCD_Pixel( x1, y1, mode );
            if(response)
                return response;
            
        }
    } else {
        //frakcja
        fraction = dx - ( dy >> 1);
        while ( y1 != y2 ) {
            if ( fraction >= 0 ) {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;
            
            //rysowanie punktu
            response = PCD_Pixel( x1, y1, mode );
            if(response)
                return response;
        }
    }
    
    //ustawienie flagi
    
    UpdateLcd = TRUE;
    return OK;
}


// PCD_SBar
// Pozwala na rysowanie słupka

unsigned char PCD_SBar ( unsigned char baseX, unsigned char baseY, unsigned char width, unsigned char height, LcdPixelMode mode ) {
    unsigned char tmpIdxX,tmpIdxY;
    
    unsigned char response;
    
    // Sprawdzenie ramek
    if ( ( baseX > LCD_X_RES ) || ( baseY > LCD_Y_RES ) ) return OUT_OF_BORDER;
    
    // Rysowanie lini
    for ( tmpIdxY = baseY; tmpIdxY < (baseY + height); tmpIdxY++ ) {
        for ( tmpIdxX = baseX; tmpIdxX < (baseX + width); tmpIdxX++ )  {
            response = PCD_Pixel( tmpIdxX, tmpIdxY, mode );
            if(response) {
                return response;
            }
        }
    }
    
    // Ustawienie flagi
    UpdateLcd = TRUE;
    return OK;
}


// PCD_Bars
// Pozwala na rysowanie wielu słupków

unsigned char PCD_Bars ( unsigned char data[], unsigned char numbBars, unsigned char width, unsigned char multiplier ) {
    unsigned char b;
    unsigned char tmpIdx = 0;
    unsigned char response;
    
    for ( b = 0;  b < numbBars ; b++ ) {
        // obliczenie ramek (LCD_X_RES)
        if ( tmpIdx > LCD_X_RES ) return OUT_OF_BORDER;
        
        // kalkulacja osi x
        tmpIdx = ((width + EMPTY_SPACE_BARS) * b) + BAR_X;
        
        // Rysowanie słupka
        response = PCD_SBar( tmpIdx, BAR_Y, data[ b ] * multiplier, width, PIXEL_ON);
        if(response == OUT_OF_BORDER)
            return response;
    }
    
    // Ustawienie flagi na True
    UpdateLcd = TRUE;
    return OK;
    
}

// PCD_Rect
// Rysuje prostokąt o zadanych parametrach

unsigned char PCD_Rect ( unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2, LcdPixelMode mode ) {
    unsigned char tmpIdxX,tmpIdxY;
    unsigned char response;
    
    // Sprawdzenie ramek
    if ( ( x1 > LCD_X_RES ) ||  ( x2 > LCD_X_RES ) || ( y1 > LCD_Y_RES ) || ( y2 > LCD_Y_RES ) )
        // jesli osiagnieto ramke -- powrót
        return OUT_OF_BORDER;
    
    if ( ( x2 > x1 ) && ( y2 > y1 ) ) {
        for ( tmpIdxY = y1; tmpIdxY < y2; tmpIdxY++ ) {
            // Rysowanie lini poziomej
            for ( tmpIdxX = x1; tmpIdxX < x2; tmpIdxX++ ) {
                // rysowanie pixeli
                response = PCD_Pixel( tmpIdxX, tmpIdxY, mode );
                if(response)
                    return response;
            }
        }
        
        // ustawienie flagi
        UpdateLcd = TRUE;
    }
    return OK;
}

// PCD_Img
// Wyswietla bitmape

void PCD_Img ( const unsigned char *imageData ) {
    
    memcpy_P(LcdCache,imageData,LCD_CACHE_SIZE);
    
    LoWaterMark = 0;
    HiWaterMark = LCD_CACHE_SIZE - 1;
    
    UpdateLcd = TRUE;
}

// PCD_Upd
// Kopiuje zawartosc cache do pamieci RAM wyswietlacza

void PCD_Upd ( void ) {
    int i;
    
    if ( LoWaterMark < 0 )
        LoWaterMark = 0;
    else if ( LoWaterMark >= LCD_CACHE_SIZE )
        LoWaterMark = LCD_CACHE_SIZE - 1;
    
    if ( HiWaterMark < 0 )
        HiWaterMark = 0;
    else if ( HiWaterMark >= LCD_CACHE_SIZE )
        HiWaterMark = LCD_CACHE_SIZE - 1;
    
    PCD_Snd( 0x80 | ( LoWaterMark % LCD_X_RES ), LCD_CMD );
    PCD_Snd( 0x40 | ( LoWaterMark / LCD_X_RES ), LCD_CMD );
    
    for ( i = LoWaterMark; i <= HiWaterMark; i++ ) {
        PCD_Snd( LcdCache[ i ], LCD_DATA );
    }
    
    LoWaterMark = LCD_CACHE_SIZE - 1;
    HiWaterMark = 0;
    
    UpdateLcd = FALSE;
}


// PCD_Snd
// Wysyła dane do wyswietlacza

static void PCD_Snd ( unsigned char data, LcdCmdData cd ) {
    // Właczenie kontrolera - aktywny przy LOW
    LCD_PORT &= ~( _BV( LCD_CE_PIN ) );
    
    if ( cd == LCD_DATA ) {
        LCD_PORT |= _BV( LCD_DC_PIN );
    } else {
        LCD_PORT &= ~( _BV( LCD_DC_PIN ) );
    }
    
    // Wysłanie danych do kontrolera
    SPDR = data;
    
    // Oczekiwanie na koniec
    while ( (SPSR & 0x80) != 0x80 );
    
    // wyłaczenie kontrolera LCD
    LCD_PORT |= _BV( LCD_CE_PIN );
}

// PCD_DrwBrd
// Rysuje ramke dookoła ekranu
/*
 void PCD_DrwBrd ( void )
 {
 unsigned char i, j;
 
 for(i=0; i<7; i++)
 {
 LCD_gotoXY (0,i);
 
 for(j=0; j<84; j++)
 {
 if(j == 0 || j == 83)
 LCD_writeData (0xff);        // first and last column solid fill to make line
 else if(i == 0)
 LCD_writeData (0x08);        // row 0 is having only 5 bits (not 8)
 else if(i == 6)
 LCD_writeData (0x04);        // row 6 is having only 3 bits (not 8)
 else
 LCD_writeData (0x00);
 }
 }
 }
 */
// Delay
// konieczne opóźnienie dla LCD
static void Delay ( void ) {
    int i;
    for ( i = -32000; i < 32000; i++ );
}
