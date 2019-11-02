#include "data-processor-intf.h"

class DATA_PROCESSOR : public DATA_PROCESSOR_INTF
{
    public:
    DATA_PROCESSOR(){}

    bool register(uint16_t const & id, std::function<void(std::array<uint8_t,8> const & data)>) override;

    void processData();
};