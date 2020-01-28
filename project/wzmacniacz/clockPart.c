//
//  clockPart.c
//  Index
//
//  Created by Marcin Kielesiński on 27/01/2020.
//

#include "clockPart.h"

#define ITEM_VISIBILITY_CYCLES_COUNTER 20

#define waitFor(code) while (rc5Code == code) {  readCommands(READ_COMMANDS_DELAY_TIME); }

static PCF_DateTime pcfDateTime;
static bool clockSetMode = false;

static unsigned char activeItem = 0;
static bool itemVisible = true;
static int itemVisibilityCounter = 0;

static char s[120];
static char day[4] = {0};
static char month[4] = {0};
static char year[5] = {0};
static char hour[4] = {0};
static char minute[4] = {0};
static char second[4] = {0};

#define CLOCK_SETTABLE_ITEMS 6
static const char *items[CLOCK_SETTABLE_ITEMS] = {
    day, month, year, hour, minute, second
};

const char *getItem(unsigned char number) {
    if(activeItem == number) {
        return itemVisible ? items[number] : "  ";
    }
    return items[number];
}

void setClockSetMode(bool enabled) {
    clockSetMode = enabled;
    if(clockSetMode) {
        activeItem = 0;
        itemVisibilityCounter = 0;
        itemVisible = true;
    }
}

void clockMainFunction(void) {
    
    if(!clockSetMode) {
    
        PCF_GetDateTime(&pcfDateTime);
        
        if(rc5Code == RC5_CLOCK_SET && !powerIsOn) {
            setClockSetMode(true);
            return;
        }
        
        PCD_GotoXYFont(2, 1);
        memset(s, 0, sizeof(s));
        
        snprintf(s, sizeof(s), "%02d-%02d %d", pcfDateTime.day, pcfDateTime.month, pcfDateTime.year);
        PCD_print(FONT_1X, (unsigned char*)s);
        
        PCD_GotoXYFont(2, 3);
        memset(s, 0, sizeof(s));
        
        char second = ' ';
        if(pcfDateTime.second %2 == 0) {
            second = ':';
        }
        
        snprintf(s, sizeof(s), "%02d%c%02d", pcfDateTime.hour, second, pcfDateTime.minute);
        PCD_print(FONT_2X, (unsigned char*)s);
        
        PCD_GotoXYFont(12, 3);
        memset(s, 0, sizeof(s));
        snprintf(s, sizeof(s), "%02d", pcfDateTime.second);
        PCD_print(FONT_1X, (unsigned char*)s);
        
    } else {
        
        switch(rc5Code) {
            case RC5_MENU:
                waitFor(RC5_MENU);
                
                if(++activeItem > CLOCK_SETTABLE_ITEMS - 1) {
                    setClockSetMode(false);
                    return;
                }
                itemVisible = true;
                break;
                
            case RC5_MENU_PLUS:
                waitFor(RC5_MENU_PLUS);
                
                break;
                
            case RC5_MENU_MINUS:
                waitFor(RC5_MENU_MINUS);
                
                break;
        }
        
        snprintf(day, sizeof(day), "%02d", pcfDateTime.day);
        snprintf(month, sizeof(month), "%02d", pcfDateTime.month);
        snprintf(year, sizeof(year), "%d", pcfDateTime.year);
        snprintf(hour, sizeof(hour), "%02d", pcfDateTime.hour);
        snprintf(minute, sizeof(minute), "%02d", pcfDateTime.minute);
        snprintf(second, sizeof(second), "%02d", pcfDateTime.second);
        
        if(itemVisibilityCounter++ > ITEM_VISIBILITY_CYCLES_COUNTER) {
            itemVisibilityCounter = 0;
            
            if(itemVisible) {
                itemVisible = false;
            } else {
                itemVisible = true;
            }
        }
        
        PCD_GotoXYFont(2, 1);
        memset(s, 0, sizeof(s));
        snprintf(s, sizeof(s), "%s-%s %s",
            getItem(0), getItem(1), getItem(2) );
        
        PCD_print(FONT_1X, (unsigned char*)s);
        
        PCD_GotoXYFont(3, 2);
        memset(s, 0, sizeof(s));
        snprintf(s, sizeof(s), "%s:%s %s",
                 getItem(3), getItem(4), getItem(5) );
        
        PCD_print(FONT_1X, (unsigned char*)s);
    }

}
