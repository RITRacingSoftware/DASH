#include <etl/array.h>

class CAN_PROCESSOR_INTF
{
    public:
    CAN_PROCESSOR_INTF(){}

    virtual ~CAN_PROCESSOR_INTF(){}

    virtual bool readCAN(uint16_t & id, etl::array<uint8_t, 8>) = 0;
};