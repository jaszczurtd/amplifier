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

static int lastVolume = 0;

static int powerLEDDelay = 0;
static unsigned char powerLEDValue = 0;

static int speakersCounter = 0;
void speakers(bool x) {
    if(x)sbi(PORTA,PD7);
    else    cbi(PORTA,PD7);
}

static int powerResCounter = 0;

bool powerIsOn = false;
void power(bool x) {
    if(x)sbi(PORTB,PB2);
    else    cbi(PORTB,PB2);
}

void setup(void) {
    cli();
    
    wdt_enable( WDTO_1S );
    
    initInputs();
    
    power(powerIsOn = DEFAULT_POWER_IS_ON);

    powerLEDDelay = 0;
    powerLEDValue = 0;
    
    powerResCounter = 0;

    init74574();
    RC5_Init();
    initDS1267();
    TWI_Init();
    PWM_Init(true, false);
    ADC_Init(true);
    init74150();
    PCF_Init(PCF_TIMER_INTERRUPT_ENABLE);
    Impulsator_Init(255);
    setImpulsatorStep(1);

    PCD_Ini();
    PCD_Contr(0x40);
    PCD_Clr();
    PCD_Upd();
    
    setImpulsatorValue((lastVolume = MEM[E_VOLUME]));
    setDS1267(lastVolume, lastVolume);
    
    sei();
    
    setLoudness(MEM[E_LOUDNESS]);
    

    
    
     /*
     PCD_GotoXYFont(0,0);
     
     PCD_FStr(FONT_1X,(unsigned char*)PSTR(" Czesc moje"));
     PCD_GotoXYFont(0,1);
     PCD_FStr(FONT_1X,(unsigned char*)PSTR("  koffanie "));
     PCD_GotoXYFont(0,2);
     PCD_FStr(FONT_1X,(unsigned char*)PSTR("--!@#$%^&*()--"));
     PCD_SBar ( 0, 25, 16, 12, PIXEL_ON);
     
     
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
     
     */
}


int main(void) {
    
    setup();
    
    while(1) {
        PCD_Clr();
        PCD_GotoXYFont(0,0);
        
        readCommands(0);

        if(powerIsOn) {
            
            if(power_sw()) {
                while (power_sw()) { RC(); }
                power(powerIsOn = false);
                clearPorts();
                continue;
            }
            
            if(loudness_sw()) {
                while (loudness_sw()) { RC(); }
                setLoudness(EORValue(E_LOUDNESS));
            }
            
            switch(rc5Code) {
                case RC5_VOLUME_UP:
                    Impulsator_increase();
                    setStoreStatusFlag();
                    break;
                case RC5_VOLUME_DOWN:
                    Impulsator_decrease();
                    setStoreStatusFlag();
                    break;
            }
            
            MEM[E_VOLUME] = getImpulsatorValue();
            if(lastVolume != MEM[E_VOLUME]) {
                lastVolume = MEM[E_VOLUME];
                
                setDS1267(lastVolume, lastVolume);
                
                setStoreStatusFlag();
            }
            
            int adc = getADCValue();
            
            memset(s, 0, BUF_L);
            snprintf(s, BUF_L, "%d %d", MEM[E_VOLUME], rc5Code);
            PCD_print(FONT_1X, (unsigned char*)s);
            
            PCD_GotoXYFont(0,1);
            memset(s, 0, BUF_L);
            snprintf(s, BUF_L, "%d %d", adc, switchCode);
            PCD_print(FONT_1X, (unsigned char*)s);


            
            PCD_Upd();
            storeStatusToEEPROM();
            
            if(powerLEDValue < POWER_LED_MAX_VALUE) {
                if(powerLEDDelay++ > POWER_LED_DELAY_ON){
                    powerLEDDelay = 0;
                    powerLEDValue++;
                }
            }

        } else {             //power off mode

            if(power_sw()) {
                while (power_sw()) { readCommands(READ_COMMANDS_DELAY_TIME); }
                power(powerIsOn = true);
                setClockSetMode(false);
                restoreStatusFromEEPROM();
                restoreOutputs();
                continue;
            }
            
            clockMainFunction();
            PCD_Upd();
            
            if(powerLEDValue > 0) {
                if(powerLEDDelay++ > POWER_LED_DELAY_OFF){
                    powerLEDDelay = 0;
                    powerLEDValue--;
                }
            }
        }
        PWM_SetValue(true, false, powerLEDValue);

        _delay_ms(MAIN_DELAY_TIME);
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
