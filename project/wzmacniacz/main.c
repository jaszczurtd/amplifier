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

static bool speakersSequenceEnd = false;
static bool speakersFlag = false;
static int speakersCounter = 0;

static int powerResCounter = 0;
static bool powerResEnabled = false;

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
    powerResEnabled = false;
    speakersSequenceEnd = false;
    speakersFlag = false;
    
    speakersCounter = 0;

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
    
    restoreStatusFromEEPROM();
    restoreOutputs();
    
    setImpulsatorValue((lastVolume = MEM[E_VOLUME]));
    setDS1267(lastVolume, lastVolume);
    
    sei();
    
     /*
     PCD_GotoXYFont(0,0);
     
     PCD_FStr(FONT_1X,(unsigned char*)PSTR(" Czesc moje"));
     PCD_GotoXYFont(0,1);
     PCD_FStr(FONT_1X,(unsigned char*)PSTR("  koffanie "));
     PCD_GotoXYFont(0,2);
     PCD_FStr(FONT_1X,(unsigned char*)PSTR("--!@#$%^&*()--"));
     PCD_SBar ( 0, 25, 16, 12, PIXEL_ON);
     
     
     */
}

int main(void) {
    
    setup();
    
    while(1) {
        PCD_Clr();
        PCD_GotoXYFont(0,0);
        
        readCommands(0);

        if(powerIsOn) {
            
            if(!powerResEnabled) {
                if(powerResCounter < POWER_RES_COUNTER) {
                    powerResCounter++;
                } else {
                    setPowerRes(powerResEnabled = true);
                    
                    _delay_ms(DELAY_BETWEEN_STATES);
                    
                    restoreStatusFromEEPROM();
                    restoreOutputs();
                }
            } else {
                if(!speakersSequenceEnd) {
                    if(speakersCounter < SPEAKERS_COUNTER) {
                        speakersCounter++;
                    } else {
                        setSpeakers(speakersFlag = true);
                        
                        speakersSequenceEnd = true;
                    }
                }
            }
            
            if(power_sw()) {
                while (power_sw()) { RC(); }
                
                setSpeakers(speakersFlag = false);
                delay_ms(DELAY_BETWEEN_STATES);
                clearPorts();
                delay_ms(100);
                
                power(powerIsOn = false);
                setPowerRes(false);
                powerResEnabled = false;
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
                case RC5_MUTE:
                    waitForRC5(RC5_MUTE);
                    
                    if(speakersFlag) {
                        speakersFlag = false;
                    } else {
                        speakersFlag = true;
                    }
                    setSpeakers(speakersFlag);
                    break;
            }
            
            if(tape_sw()) {
                while (tape_sw()) { RC(); }
                setSpecifiedOutputDisableOthers(E_TAPE);
            }
            if(radio_sw()) {
                while (radio_sw()) { RC(); }
                setSpecifiedOutputDisableOthers(E_RADIO);
            }
            if(dac_sw()) {
                while (dac_sw()) { RC(); }
                setSpecifiedOutputDisableOthers(E_DAC);
            }
            if(piezo_sw()) {
                while (piezo_sw()) { RC(); }
                setSpecifiedOutputDisableOthers(E_PIEZO);
            }
            if(generic_sw()) {
                while (generic_sw()) { RC(); }
                setSpecifiedOutputDisableOthers(E_GENERIC);
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
            snprintf(s, BUF_L, "%d %d %d", adc, switchCode, checkIfTimerReached());
            PCD_print(FONT_1X, (unsigned char*)s);

            PCD_GotoXYFont((S_WIDTH - strlength((char*)getOutputDisplayString())) / 2, 2);
            memset(s, 0, BUF_L);
            strcpy(s, getOutputDisplayString());
            PCD_print(FONT_1X, (unsigned char*)s);

            PCD_Upd();
            storeStatusToEEPROM();
            
            if(powerLEDValue < POWER_LED_MAX_VALUE) {
                powerLEDValue++;
            }

        } else {             //power off mode

            if(power_sw()) {
                while (power_sw()) { readCommands(READ_COMMANDS_DELAY_TIME); }
                
                speakersSequenceEnd = speakersFlag = false;
                
                powerResCounter = 0;
                speakersCounter = 0;
                setPowerRes(powerResEnabled = false);
                
                power(powerIsOn = true);
                setClockSetMode(false);
                
                continue;
            }
            
            clockMainFunction();
            PCD_Upd();
            
            if(powerLEDValue > 0) {
                powerLEDValue--;
            }
        }
        PWM_SetValue(true, false, powerLEDValue);

        _delay_ms(MAIN_DELAY_TIME);
    }
    
    return 0;
}
