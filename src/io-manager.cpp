#include "io-manager.h"

#include <Arduino.h>

#define PIN_TOGGLE_SWITCH_01 0 //one toggle switch
#define PIN_ROTARY_SWITCH_01 A0 //one rotary switch

namespace IOManager{

    void init(){
        Serial.begin(9600);
        
        pinMode(PIN_TOGGLE_SWITCH_01, INPUT);
        int rotSwitchVal01;

    }

    void update(){

        if(digitalRead(PIN_TOGGLE_SWITCH_01) == HIGH){
            
        }
    }    
}