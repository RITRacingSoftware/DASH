#include "gpio-processor.h"
#include "Arduino.h"

int GPIO_PROCESSOR::readGPIO(uint8_t pinNum)
{
    //Callback
    return digitalRead(pinNum);
} 

void GPIO_PROCESSOR::writeGPIO(uint8_t pinNum, bool val)
{
    if(val == true)
    {
        digitalWrite(pinNum, LOW);
    }
    else
    {
        digitalWrite(pinNum, HIGH);
    }
}

