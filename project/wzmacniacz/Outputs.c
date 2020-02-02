//
//  Outputs.c
//  Index
//
//  Created by Marcin Kielesiński on 02/02/2020.
//

#include "Outputs.h"

void restoreOutputs(void) {
    setLoudness(MEM[E_LOUDNESS]);
    setRadio(MEM[E_RADIO]);
    setDAC(MEM[E_DAC]);
    setTape(MEM[E_TAPE]);
    setPiezo(MEM[E_PIEZO]);
}

void setLoudness(bool state) {
    clockPort(PORT_OUTPUTS, false);
    (state) ? sbi(PORTA, PA3) : cbi(PORTA, PA3);
    clockPort(PORT_OUTPUTS, true);
}

void setRadio(bool state) {
    clockPort(PORT_OUTPUTS, false);
    
    clockPort(PORT_OUTPUTS, true);
}

void setDAC(bool state) {
    clockPort(PORT_OUTPUTS, false);
    
    clockPort(PORT_OUTPUTS, true);
}

void setTape(bool state) {
    clockPort(PORT_OUTPUTS, false);
    
    clockPort(PORT_OUTPUTS, true);
}

void setPiezo(bool state) {
    clockPort(PORT_OUTPUTS, false);
    
    clockPort(PORT_OUTPUTS, true);
}

void setPowerRes(bool state) {
    clockPort(PORT_OUTPUTS, false);

    clockPort(PORT_OUTPUTS, true);
}

void setSpeakers(bool state) {
    clockPort(PORT_OUTPUTS, false);

    clockPort(PORT_OUTPUTS, true);
}
