#include "etl/delegate.h"
#include "etl/array.h"

class DASH_CONTROLLER_INTF
{
public:

    DASH_CONTROLLER_INTF() {}
    virtual ~DASH_CONTROLLER_INTF() {}

    virtual void initialize() = 0;
    virtual void updateView() = 0;
    virtual void updateModel() = 0;

    virtual void registerCallback(uint16_t const &id,
                   etl::delegate<void(etl::array<uint8_t, 8> const &)> const
                       &callback) = 0;
};