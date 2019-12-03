#include <etl/array.h>

class CAN_PROCESSOR_INTF
{
    public:
    CAN_PROCESSOR_INTF(){}

    virtual ~CAN_PROCESSOR_INTF(){}

    virtual bool readCAN(CAN_MESSAGE & msg);
};