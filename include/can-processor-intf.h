#include <etl/array.h>

class CAN_PROCESSOR_INTF
{
    public:
    CAN_PROCESSOR_INTF(){}

    virtual ~CAN_PROCESSOR_INTF(){}

    /*
    @brief Reads the CAN bus and populates a can data strucuture
    @param msg The message to poplulate if there is an available CAN message
    @return True if a can message was read successfully, false otherwise
    The intended use is that whatever class is using should use a loop to call the function until it returns false. Then
    you know there are no CAN messages.
    */
    virtual bool readCAN(CAN_MESSAGE & msg) = 0;
};