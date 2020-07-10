//
//  clockPart.c
//  Index
//
//  Created by Marcin Kielesiński on 27/01/2020.
//

#include "clockPart.h"

#define ITEM_VISIBILITY_CYCLES_COUNTER 20

static PCF_DateTime pcfDateTime;
static bool clockSetMode = false;

static unsigned char activeItem = 0;
static bool itemVisible = true;
static int itemVisibilityCounter = 0;

static char day[4] = {0};
static char month[4] = {0};
static char year[5] = {0};
static char hour[4] = {0};
static char minute[4] = {0};
static char second[4] = {0};
static char weekday[14] = {0};

#define CLOCK_SETTABLE_ITEMS 7
static const char *items[CLOCK_SETTABLE_ITEMS] = {
    day, month, year, hour, minute, second, weekday
};
                
const char *getDayString(void) {
    char *weekday = "";
    switch(pcfDateTime.weekday) {
        case 0:
            weekday = "Poniedzialek";
            break;
        case 1:
            weekday = "Wtorek";
            break;
        case 2:
            weekday = "Sroda";
            break;
        case 3:
            weekday = "Czwartek";
            break;
        case 4:
            weekday = "Piatek";
            break;
        case 5:
            weekday = "Sobota";
            break;
        case 6:
            weekday = "Niedziela";
            break;
    }
    return weekday;
}

const char *getItem(unsigned char number) {
    if(activeItem == number) {
        return itemVisible ? items[number] : "  ";
    }
    return items[number];
}

void setItemVisible(void) {
    itemVisible = true;
    itemVisibilityCounter = 0;
}

void setItem(bool increase) {
    switch(activeItem) {
        case 0:
            if(increase) {
                if(pcfDateTime.day < 31) {
                    pcfDateTime.day++;
                } else {
                    pcfDateTime.day = 1;
                }
            } else {
                if(pcfDateTime.day > 1) {
                    pcfDateTime.day--;
                } else {
                    pcfDateTime.day = 31;
                }
            }
            break;
            
        case 1:
            if(increase) {
                if(pcfDateTime.month < 12) {
                    pcfDateTime.month++;
                } else {
                    pcfDateTime.month = 1;
                }
            } else {
                if(pcfDateTime.month > 1) {
                    pcfDateTime.month--;
                } else {
                    pcfDateTime.month = 12;
                }
            }
            break;
            
        case 2:
            if(increase) {
                if(pcfDateTime.year < PCF_MAX_YEAR) {
                    pcfDateTime.year++;
                } else {
                    pcfDateTime.year = PCF_MIN_YEAR;
                }
            } else {
                if(pcfDateTime.year > PCF_MIN_YEAR) {
                    pcfDateTime.year--;
                } else {
                    pcfDateTime.year = PCF_MAX_YEAR;
                }
            }
            break;
            
        case 3:
            if(increase) {
                if(pcfDateTime.hour < 23) {
                    pcfDateTime.hour++;
                } else {
                    pcfDateTime.hour = 0;
                }
            } else {
                if(pcfDateTime.hour > 0) {
                    pcfDateTime.hour--;
                } else {
                    pcfDateTime.hour = 23;
                }
            }
            break;
            
        case 4:
            if(increase) {
                if(pcfDateTime.minute < 59) {
                    pcfDateTime.minute++;
                } else {
                    pcfDateTime.minute = 0;
                }
            } else {
                if(pcfDateTime.minute > 0) {
                    pcfDateTime.minute--;
                } else {
                    pcfDateTime.minute = 59;
                }
            }
            break;
            
        case 5:
            if(increase) {
                if(pcfDateTime.second < 59) {
                    pcfDateTime.second++;
                } else {
                    pcfDateTime.second = 0;
                }
            } else {
                if(pcfDateTime.second > 0) {
                    pcfDateTime.second--;
                } else {
                    pcfDateTime.second = 59;
                }
            }
            break;
            
        case 6:
            if(increase) {
                if(pcfDateTime.weekday < 6) {
                    pcfDateTime.weekday++;
                } else {
                    pcfDateTime.weekday = 0;
                }
            } else {
                if(pcfDateTime.weekday > 0) {
                    pcfDateTime.weekday--;
                } else {
                    pcfDateTime.weekday = 6;
                }
            }
    }
    setItemVisible();
}

void setClockSetMode(bool enabled) {
    clockSetMode = enabled;
    if(clockSetMode) {
        activeItem = 0;
        setItemVisible();
    }
}

void getTime(void) {
#if REAL_HARDWARE
    PCF_GetDateTime(&pcfDateTime);
#endif
}

void printClockHour(unsigned char x, unsigned char y) {
    PCD_GotoXYFont(x, y);
    
    char second = ' ';
    if(pcfDateTime.second %2 == 0) {
        second = ':';
    }
    
    memset(s, 0, BUF_L);
    snprintf(s, BUF_L, "%02d%c%02d", pcfDateTime.hour, second, pcfDateTime.minute);
    PCD_print(FONT_2X, (unsigned char*)s);
    
    PCD_GotoXYFont(x + 10, y);
    memset(s, 0, BUF_L);
    snprintf(s, BUF_L, "%02d", pcfDateTime.second);
    PCD_print(FONT_1X, (unsigned char*)s);
}


void clockMainFunction(void) {

#if REAL_HARDWARE
    if(!clockSetMode) {
    
        getTime();
        
        if(rc5Code == RC5_CLOCK_SET && !powerIsOn) {
            setClockSetMode(true);
            setItemVisible();
            return;
        }
        
        PCD_GotoXYFont(2, 1);
        memset(s, 0, BUF_L);
        
        snprintf(s, BUF_L, "%02d-%02d %d", pcfDateTime.day, pcfDateTime.month, pcfDateTime.year);
        PCD_print(FONT_1X, (unsigned char*)s);
        
        printClockHour(2, 3);
        
        const char *weekday = getDayString();
        PCD_GotoXYFont((S_WIDTH - strlen(weekday)) / 2, 4);
        PCD_print(FONT_1X, (unsigned char*)weekday);
        
    } else {
        
        switch(rc5Code) {
            case RC5_MENU:
                waitForRC5(RC5_MENU);
                
                if(++activeItem > CLOCK_SETTABLE_ITEMS - 1) {
                    setClockSetMode(false);
                    
                    PCF_SetDateTime(&pcfDateTime);
                    return;
                }
                break;
                
            case RC5_MENU_PLUS:
                waitForRC5(RC5_MENU_PLUS);
                setItem(true);
                break;
                
            case RC5_MENU_MINUS:
                waitForRC5(RC5_MENU_MINUS);
                setItem(false);
                break;
        }
        
        snprintf(day, sizeof(day), "%02d", pcfDateTime.day);
        snprintf(month, sizeof(month), "%02d", pcfDateTime.month);
        snprintf(year, sizeof(year), "%d", pcfDateTime.year);
        snprintf(hour, sizeof(hour), "%02d", pcfDateTime.hour);
        snprintf(minute, sizeof(minute), "%02d", pcfDateTime.minute);
        snprintf(second, sizeof(second), "%02d", pcfDateTime.second);
        snprintf(weekday, sizeof(weekday), "%s", getDayString());
        
        if(itemVisibilityCounter++ > ITEM_VISIBILITY_CYCLES_COUNTER) {
            itemVisibilityCounter = 0;
            
            if(itemVisible) {
                itemVisible = false;
            } else {
                itemVisible = true;
            }
        }
        
        PCD_GotoXYFont(2, 1);
        memset(s, 0, BUF_L);
        snprintf(s, BUF_L, "%s-%s %s",
            getItem(0), getItem(1), getItem(2) );
        
        PCD_print(FONT_1X, (unsigned char*)s);
        
        PCD_GotoXYFont(3, 2);
        memset(s, 0, BUF_L);
        snprintf(s, BUF_L, "%s:%s %s",
                 getItem(3), getItem(4), getItem(5) );

        PCD_print(FONT_1X, (unsigned char*)s);
        
        PCD_GotoXYFont((S_WIDTH - strlen(getItem(6))) / 2, 3);
        PCD_print(FONT_1X, (unsigned char*)getItem(6));
    }
#else
    static int lastCMD = 0;
    static long increase = 0;

	int cmd = RC5_NewCommandReceived();
	if(lastCMD != cmd && cmd != 0) {
		lastCMD = cmd;
	}
	PCD_GotoXYFont(0, 0);
	PCD_print(FONT_1X, (unsigned char*)"debug");

	PCD_GotoXYFont(0, 1);
	snprintf(s, BUF_L, "rc5: %d %ld", lastCMD, increase++);
	PCD_print(FONT_1X, (unsigned char*)s);

#endif
    
}

static unsigned char lastSecondValue = -1;
static unsigned char secondsToCount = 0;
void startTimerForSeconds(unsigned char seconds) {
    secondsToCount = seconds;
    lastSecondValue = -1;
}

bool checkIfTimerReached(void) {
    if(secondsToCount == 0) {
        return true;
    }
    
    PCF_GetDateTime(&pcfDateTime);
    if(lastSecondValue == -1) {
        lastSecondValue = pcfDateTime.second;
        return false;
    }
    if(lastSecondValue != pcfDateTime.second) {
        lastSecondValue = pcfDateTime.second;
        secondsToCount--;
    }
    return false;
}
