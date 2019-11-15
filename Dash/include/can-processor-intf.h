#include <array>

class CAN_PROCESSOR_INTF

{

public:
    CAN_PORCESSOR_INTF(){}

    virtual ~CAN_PROCESSOR_INTF(){}

    virtual bool readCAN(uint16_t & id, std::array<unit8_t, 8>) = 0;
    
};

