#include "etl/delegate.h"
#include "etl/array.h"

class DATA_PROCESSOR_INTF
{
    public:
    DATA_PROCESSOR_INTF(){}

    virtual ~DATA_PROCESSOR_INTF(){}

    virtual bool registerCallback(uint16_t, etl::delegate<void(etl::array<uint8_t, 8> const &)> const
                       &callback);
};