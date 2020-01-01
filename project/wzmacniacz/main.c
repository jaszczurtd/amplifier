/**
 *
 * zadig driver installer
 * https://rlogiacco.wordpress.com/2016/09/01/usbasp-windows-10/
 * https://zadig.akeo.ie/
 
 libusbK
 usbasp
 *
 */

#define DS18B20_ENABLED true

#include "main.h"

static bool powerIsOn = false;
static int powerResCounter = 0;
static int speakersCounter = 0;

static bool tapeIsOn = false,
    radioIsOn = false,
    dacIsOn = false,
    genericIsOn = false,
    piezoIsOn = false;

bool generic_sw(void) {
    return bit_is_clear(PINB,PB0);
}

bool tape_sw(void) {
    return bit_is_clear(PINB,PB1);
}

bool piezo_sw(void) {
    return bit_is_clear(PINB,PB2);
}

bool radio_sw(void) {
    return bit_is_clear(PINB,PB3);
}

bool dac_sw(void) {
    return bit_is_clear(PINB,PB4);
}

bool power_sw(void) {
    return bit_is_clear(PINB,PB5);
}

void generic(bool x) {
    if(x)sbi(PORTA,PD0);
    else    cbi(PORTA,PD0);
}

void tape(bool x) {
    if(x)sbi(PORTA,PD1);
    else    cbi(PORTA,PD1);
}

void piezo(bool x) {
    if(x)sbi(PORTA,PD2);
    else    cbi(PORTA,PD2);
}

void radio(bool x) {
    if(x)sbi(PORTA,PD3);
    else    cbi(PORTA,PD3);
}

void dac(bool x) {
    if(x)sbi(PORTA,PD4);
    else    cbi(PORTA,PD4);
}

void power(bool x) {
    if(x)sbi(PORTA,PD5);
    else    cbi(PORTA,PD5);
}

void powerRes(bool x) {
    if(x)sbi(PORTA,PD6);
    else    cbi(PORTA,PD6);
}

void speakers(bool x) {
    if(x)sbi(PORTA,PD7);
    else    cbi(PORTA,PD7);
}

void resetInputs(void) {
    generic(false);
    tape(false);
    piezo(false);
    radio(false);
    dac(false);
}

void resetMemoryInputs(void) {
    genericIsOn = false;
    tapeIsOn = false;
    piezoIsOn = false;
    radioIsOn = false;
    dacIsOn = false;
}

void applyInputs(void) {
    generic(genericIsOn);
    tape(tapeIsOn);
    piezo(piezoIsOn);
    radio(radioIsOn);
    dac(dacIsOn);
}

void setup(void) {
    powerIsOn = false;
    powerResCounter = 0;
    
    sbi(DDRA,PA0);    //uniw
    sbi(DDRA,PA1);    //magn
    sbi(DDRA,PA2);    //gram
    sbi(DDRA,PA3);    //radio
    sbi(DDRA,PA4);    //dac
    sbi(DDRA,PD5);    //power
    sbi(DDRA,PA6);    //power 2
    sbi(DDRA,PA7);    //glosniki
    
    cbi(DDRB,PB0); //switch uniw
    cbi(DDRB,PB1);    //switch magn
    cbi(DDRB,PB2);    //switch gram
    cbi(DDRB,PB3);    //switch radio
    cbi(DDRB,PB4);    //switch dac
    cbi(DDRB,PB5);    //switch power
    
    power(false);
    powerRes(false);
    speakers(false);
    
    resetMemoryInputs();
    resetInputs();
    
    dacIsOn = true;    //default input
    
    sbi(DDRD,PA0);    //LED
}

static int temp[2] = {0, 0};


static int lastADC = 0;
static int lastVolume = 0;

static int eepromDelay = 0;

#define E_VOLUME 0

static bool eepromWrite = false;
static unsigned char EEPROM[10];

int main(void) {
    
    cli();

    wdt_enable( WDTO_1S );
    
    PCD_Ini();
    PCD_Contr(0x3f);
    PCD_Clr();
    PCD_Upd();

    /*
    PCD_GotoXYFont(0,0);

    PCD_FStr(FONT_1X,(unsigned char*)PSTR(" Czesc moje"));
    PCD_GotoXYFont(0,1);
    PCD_FStr(FONT_1X,(unsigned char*)PSTR("  koffanie "));
    PCD_GotoXYFont(0,2);
    PCD_FStr(FONT_1X,(unsigned char*)PSTR("--!@#$%^&*()--"));
    PCD_SBar ( 0, 25, 16, 12, PIXEL_ON);
     
    */
    
    RC5_Init();
    initDS1267();
    TWI_Init();
    PWM_Init(true, false);
    ADC_Init(true);
    init74150();
    
    Impulsator_Init(256);
    setImpulsatorStep(1);
    for(int a = 0; a < sizeof(EEPROM); a++) {
        EEPROM[a] = EEPROMread(a);
    }
    setImpulsatorValue((lastVolume = EEPROM[E_VOLUME]));

    sei();
    
    while(1) {
        wdt_reset();
        
        char s[120];
        
        PCD_Clr();
        PCD_GotoXYFont(0,0);
        
        int activeInput = read74150();
        
        int rc5 = RC5_NewCommandReceived();
        switch(rc5) {
            case RC5_VOLUME_UP:
                Impulsator_increase();
                eepromWrite = true;
                break;
            case RC5_VOLUME_DOWN:
                Impulsator_decrease();
                eepromWrite = true;
                break;
        }
        
        EEPROM[E_VOLUME] = getImpulsatorValue();
        if(lastVolume != EEPROM[E_VOLUME]) {
            lastVolume = EEPROM[E_VOLUME];
            eepromWrite = true;
        }
        
        int adc = getADCValue();
        if(lastADC != adc) {
            lastADC = adc;
            eepromWrite = true;
        }
        
#if DS18B20_ENABLED
        int *t = ds18b20_gettemp_decimal();
        temp[0] = t[0]; temp[1] = t[1];
#endif
        
        PWM_SetValue(true, false, EEPROM[E_VOLUME]);
        
        if(eepromWrite || activeInput != I_74150_NOT_ACTIVE) {
            ds18b20_delayResult();
        }
        
        memset(s, 0, sizeof(s));
        snprintf(s, sizeof(s), "%d %d", EEPROM[E_VOLUME], rc5);
        
        PCD_print(FONT_1X, (byte*)s);
        PCD_GotoXYFont(0,1);

        memset(s, 0, sizeof(s));
        snprintf(s, sizeof(s), "%d.%d %d %d", temp[0], temp[1], adc, activeInput);
        PCD_print(FONT_1X, (byte*)s);
        
        PCD_Upd();

        delay_ms(MAIN_DELAY_TIME);
        
        if(eepromDelay-- <= 0) {
            eepromDelay = WRITE_EEPROM_DELAY;
            
            if(eepromWrite) {
                for(int a = 0; a < sizeof(EEPROM); a++) {
                    EEPROMwrite(a, EEPROM[a]);
                }
                eepromWrite = false;
            }
        }
    }
    return 0;

    
    /*
    
    cli();
    wdt_enable( WDTO_1S );
    
    setup();
    sei();
    while (1) {
        
        wdt_reset();
        
        if(powerIsOn) {
            if(powerResCounter < POWER_RES_COUNTER) {
                powerResCounter++;
            } else {
                powerRes(true);
                applyInputs();
                if(speakersCounter < SPEAKERS_COUNTER) {
                    speakersCounter++;
                } else {
                    speakers(true);
                }
            }
            
            if(generic_sw()) {
                resetInputs();
                resetMemoryInputs();
                
                generic(genericIsOn = true);
                while(generic_sw()) {WR}
            }
            
            if(tape_sw()) {
                resetInputs();
                resetMemoryInputs();
                
                tape(tapeIsOn = true);
                while(tape_sw()) {WR}
            }
            
            if(piezo_sw()) {
                resetInputs();
                resetMemoryInputs();
                
                piezo(piezoIsOn = true);
                while(piezo_sw()) {WR}
            }
            
            if(radio_sw()) {
                resetInputs();
                resetMemoryInputs();
                
                radio(radioIsOn = true);
                while(radio_sw()) {WR}
            }
            
            if(dac_sw()) {
                resetInputs();
                resetMemoryInputs();
                
                dac(dacIsOn = true);
                while(dac_sw()) {WR}
            }
            
            if(power_sw()) {                //action power off
                while(power_sw()) {WR}
                
                speakersCounter = 0;
                speakers(false);
                delay_ms(10);
                resetInputs();
                delay_ms(100);
                
                power(powerIsOn = false);
                
                powerResCounter = 0;
                powerRes(false);
            }
            
        } else {
            
            if(power_sw()) {                //action power on
                while(power_sw()) {WR}
                
                power(powerIsOn = true);
            }
            
        }
        
        delay_ms(MAIN_DELAY_TIME);
        
    }
     */
}
