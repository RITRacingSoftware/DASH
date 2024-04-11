#include "led_bar.h"
#include <Arduino.h>
#include <cstdint>
#include <queue>


namespace ledControl {
    void init(){
        Serial.printf("Initializing rgb\n");
        CRGB leds[numled];
        delay(5); //idk sseems to do weird things on esp32 without it could probs remove
        FastLED.addLeds<SK6812,LED_datapin, RGB>(leds, numled); //might be GBR says datasheet 
        //add some fancy initialization led behaviour for default/no 'cum 

    }

    void SOCsetColor(uint8_t spc, uint8_t limp) {
        //get the soc and limp and do shit 
    }

}