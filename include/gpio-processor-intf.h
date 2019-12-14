#include "stdint.h"

class GPIO_PROCESSOR_INTF
{
    public:
    GPIO_PROCESSOR_INTF(){}

    virtual ~GPIO_PROCESSOR_INTF(){}

    virtual int readGPIO(uint8_t pinNum) = 0;

    virtual void writeGPIO(uint8_t pinNum, bool val) = 0;
};