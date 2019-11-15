
class GPIO_PROCESSOR_INTF
{
private:
    /* data */
public:
    GPIO_PROCESSOR_INTF(){}

    virtual ~GPIO_PROCESSOR_INTF(){}

    virtual int readGPIO(uint8_t pinNum) = 0;

};

