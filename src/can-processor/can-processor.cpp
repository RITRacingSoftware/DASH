#include "can-processor.h"
#include "etl/array.h"
#include "FlexCan_T4.h"


bool CAN_PROCESSOR::readCAN(CAN_MESSAGE & msg) 
{
    CANFD_message_t nextMsg;
    FD.read(nextMsg);
    bool messageRead = false;
    if(FD.read(nextMsg)) //If there is a CAN Message in the queue, set msg fields with it
    { 
        CANFD_message_t nextMsg; 
        msg.id = nextMsg.id; //Set the ID
        for(int i = 0; i <= 7; i++) //n to hl if need to change endianness
        { 
            msg.data[i] = nextMsg.buf[i]; //Set the data 
        }
        messageRead = true;
    }
    return messageRead;
}
