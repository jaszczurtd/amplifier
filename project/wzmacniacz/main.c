/**
 *
 * zadig driver installer
 * https://rlogiacco.wordpress.com/2016/09/01/usbasp-windows-10/
 * https://zadig.akeo.ie/
 
 libusbK
 usbasp
 *
 */

#include "main.h"

#define MAIN_DELAY_TIME 10 //in ms
#define POWER_RES_COUNTER 100 //* MAIN_DELAY_TIME
#define SPEAKERS_COUNTER 20      //POWER_RES_COUNTER + (* MAIN_DELAY_TIME)

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

int main(void) {
    
    RC5_Init();
    
    PCD_Ini();
    PCD_Contr(0x3f);
    
    PCD_Clr();
    PCD_GotoXYFont(0,0);
    //PCD_FStr(FONT_1X,(unsigned char*)PSTR(" Czesc moje"));
    /*
    PCD_GotoXYFont(0,1);
    PCD_FStr(FONT_1X,(unsigned char*)PSTR("  koffanie "));
    PCD_GotoXYFont(0,2);
    PCD_FStr(FONT_1X,(unsigned char*)PSTR("--!@#$%^&*()--"));
    PCD_SBar ( 0, 25, 16, 12, PIXEL_ON);
    */
    PCD_Upd();
    
    sei();
    
    while(1) {
        char s[20];
        uint16_t command;
        
        PCD_Clr();
        PCD_GotoXYFont(0,0);
        
        if(RC5_NewCommandReceived(&command)) {
            RC5_Reset();
            
            memset(s, 0, sizeof(s));
            snprintf(s, sizeof(s), "wartosc: %d", command);
        } else {
            strncpy(s, "no RC5 code", sizeof(s));
        }
        
        PCD_print(FONT_1X, (byte*)s);
        PCD_Upd();
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
