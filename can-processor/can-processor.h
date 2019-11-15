#include "can-processor-intf.h"

class CAN_PROCESSOR : public CAN_PROCESSOR_INTF
{
    public:
    CAN_PROCESSOR() {}

    bool readCAN(uint16_t & id, std::array<uint8_t, 8>) override;
};