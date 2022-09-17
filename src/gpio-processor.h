#include "../include/gpio-processor-intf.h"

#define FIRST_GPIO_PIN 0;
#define SECOND_GPIO_PIN 1;
#define THIRD_GPIO_PIN 2;
#define FOURTH_GPIO_PIN 3;

class GPIO_PROCESSOR : public GPIO_PROCESSOR_INTF
{
    public:
    GPIO_PROCESSOR(){}

    int readGPIO(uint8_t pinNum) override;

    void writeGPIO(uint8_t pinNum, bool val) override;
};
