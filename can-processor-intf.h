#include <array>

class CAN_PROCESSOR_INTF
{
    public:
    CAN_PROCESSOR_INTF(){}

    virtual ~CAN_PROCESSOR_INTF(){}

    virtual bool readCAN(uint16_t & id, std::array<uint8_t, 8>) = 0;
};