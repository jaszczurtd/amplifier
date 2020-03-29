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

static int lastVolume = -1;

static unsigned char powerLEDValue = 0;

static bool speakersSequenceEnd = false;
bool speakersFlag = false;
static int speakersCounter = 0;

static int powerResCounter = 0;
static bool powerResEnabled = false;

static int volumeChangeTimer = 0;

#if DEBUG_VAL
int debugval = 0;
#endif

bool powerIsOn = false;
void setPower(bool x) {
    if(x)sbi(PORTC, PC5);
    else    cbi(PORTC, PC5);
}

void setup(void) {
    cli();
    
    wdt_enable( WDTO_1S );
    
    sbi(DDRC, PC5);

    initInputs();
    
    powerLEDValue = 0;

    powerResCounter = 0;
    powerResEnabled = false;
    speakersSequenceEnd = false;
    speakersFlag = false;
    
    speakersCounter = 0;
    volumeChangeTimer = 0;

    setPower(powerIsOn = DEFAULT_POWER_IS_ON);
    
    RC5_Init();
    initDS1267();
    TWI_Init();
    PWM_Init(true, false);
    ADC_Init(true);
    init74150();
    PCF_Init(PCF_TIMER_INTERRUPT_ENABLE);
    Impulsator_Init(MAX_VOLUME);

    clearPorts();
    
    PCD_Ini();
    PCD_Contr(0x40);
    PCD_Clr();
    PCD_Upd();
    
    restoreStatusFromEEPROM();
    clearOutputs();
    
    setImpulsatorValue((lastVolume = MEM[E_VOLUME]));
    setDS1267(lastVolume, lastVolume);
    
    sei();
}

inline void setVolumeChangerTimer(void) {
    volumeChangeTimer = VOL_CHANGE_TIME;
}

void setVolume(bool toZero) {
    unsigned char p = 1;
    
    if(!toZero) {
        p = (getImpulsatorValue() + 1);
    }
    setDS1267(p, p);
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
                delay_ms(POWER_OFF_DELAY);
                clearOutputs();
                disableAllPrograms();
                delay_ms(POWER_OFF_DELAY);
                
                setPower(powerIsOn = false);
                setPowerRes(false);
                powerResEnabled = false;
                continue;
            }
            
            if(loudness_sw()) {
                while (loudness_sw()) { RC(); }
                setLoudness(EORBit(BIT_LOUDNESS));
            }
            
            switch(rc5Code) {
                case RC5_VOLUME_UP:
                    Impulsator_increase(2);
                    setVolumeChangerTimer();
                    break;
                case RC5_VOLUME_DOWN:
                    Impulsator_decrease(2);
                    setVolumeChangerTimer();
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
            
            readSelectorInputs();
            readRadioInputs();
            
            MEM[E_VOLUME] = getImpulsatorValue();
            if(lastVolume != MEM[E_VOLUME]) {
                lastVolume = MEM[E_VOLUME];
                setStoreStatusFlag(true);
                setVolume(false);
            }

            if(volumeChangeTimer <= 0) {
                PCD_GotoXYFont(0, 0);
                if(speakersFlag) {
                    PCD_print(FONT_1X, (unsigned char*)getVolumeText());
                } else {
                    PCD_print(FONT_1X, (unsigned char*)"Brak dzwieku");
                }
                
                PCD_GotoXYFont(0, 1);
                if(getLoudness()) {
                    PCD_print(FONT_1X, (unsigned char*)"Korektor wl.");
                } else {
                    PCD_print(FONT_1X, (unsigned char*)"Korektor wyl.");
                }
                
                if(checkIfOutputIsActive(BIT_RADIO)) {
                    unsigned char x1 = 2, x2 = 10;
                    
                    if(getActiveFreq() == AM) {
                        x1 = 3;
                        x2 = 10;
                    } else {
                        if(strlen(getFrequencyValueString()) > 4) {
                            x1 = 1;
                            x2 = 11;
                        }
                    }
                    
                    PCD_GotoXYFont(x1, 4);
                    PCD_print(FONT_2X, (unsigned char*)getFrequencyValueString());
                    
                    PCD_GotoXYFont(x2, 4);
                    PCD_print(FONT_1X, (unsigned char*)getFrequencyString());

                } else {
                    getTime();
                    printClockHour(1, 4);
                }
#if DEBUG_VAL
                int adc = getADCValue();
                PCD_GotoXYFont(0, 5);
                memset(s, 0, BUF_L);
                snprintf(s, BUF_L, "%d %d %d %d", adc, switchCode, rc5Code, debugval);
                PCD_print(FONT_1X, (unsigned char*)s);
#endif
                
            } else {
                volumeChangeTimer--;

                PCD_GotoXYFont(0, 1);
                PCD_print(FONT_1X, (unsigned char*)getVolumeText());

                float percent = (getImpulsatorValue() * 100) / getImpulsatorMaxValue();
                unsigned char width = ((percent / 100) * (LCD_X_RES - 2));
                
                PCD_Line(VOL_BAR_X, LCD_X_RES, VOL_BAR_Y, VOL_BAR_Y, PIXEL_ON);
                PCD_Line(VOL_BAR_X, LCD_X_RES, VOL_BAR_Y + VOL_BAR_HEIGHT, VOL_BAR_Y + VOL_BAR_HEIGHT, PIXEL_ON);
                PCD_Line(VOL_BAR_X, VOL_BAR_X, VOL_BAR_Y, VOL_BAR_Y + VOL_BAR_HEIGHT, PIXEL_ON);
                PCD_Line(VOL_BAR_X + LCD_X_RES - 1, VOL_BAR_X + LCD_X_RES - 1, VOL_BAR_Y, VOL_BAR_Y + VOL_BAR_HEIGHT, PIXEL_ON);

                PCD_SBar (VOL_BAR_X + 1, VOL_BAR_Y, width, VOL_BAR_HEIGHT, PIXEL_ON);
            }
            
            PCD_Upd();
            
            if(powerLEDValue < POWER_LED_MAX_VALUE) {
                PWM_SetValue(true, false, ++powerLEDValue);
            }

        } else {             //power off mode

            if(power_sw()) {
                while (power_sw()) { readCommands(READ_COMMANDS_DELAY_TIME); }
                
                speakersSequenceEnd = speakersFlag = false;
                
                powerResCounter = 0;
                speakersCounter = 0;
                volumeChangeTimer = 0;
                
                setPowerRes(powerResEnabled = false);
                
                setPower(powerIsOn = true);
                setClockSetMode(false);
                
                lastVolume = -1;
                lastLoudness = -1;
                
                setStoreStatusFlag(true);
                
                continue;
            }
            
            clockMainFunction();
            PCD_Upd();
            
            if(powerLEDValue > 0) {
                PWM_SetValue(true, false, --powerLEDValue);
            }
        }

        if(!storeStatusToEEPROM()) {
            _delay_ms(MAIN_DELAY_TIME);
        }
    }
    
    return 0;
}
