#include "gpio-processor-intf.h"

class GPIO_PROCESSOR : public GPIO_PROCESSOR_INTF
{
    public:
    GPIO_PROCESSOR(){}

    int readGPIO(uint8_t pinNum) override;
};
