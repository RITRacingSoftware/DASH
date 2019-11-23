#include "can-processor.h"
#include "etl/array.h"
#include "FlexCan_T4.h"

//Structure for Can messages
struct CAN_MESSAGE{
    uint16_t id;
    etl::array<uint8_t, 8> data;
};

FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> FD;

/*
@brief Reads the CAN bus and populates a can data strucuture
@param msg The message to poplulate if there is an available CAN message
@return True if there is another can message available to read (in the queue), false otherwise
The intended use is that whatever class is using should use a loop to call the function until it returns false. Then
you know there are no CAN messages.
*/
bool CAN_PROCESSOR::readCAN(CAN_MESSAGE & msg)
{
    /*       NEED TO DO IN SETUP:                              */
    /*  have to call FD.begin() before calling any functions   */
    /*                                                         */
    CANFD_message_t nextMsg;
    FD.read(nextMsg);
    if(!queue.empty()){ //If there is a CAN Message in the queue, set msg fields with it
        CANFD_message_t nextMsg; 
        queue.pop_into(nextMsg); //Get next message in queue
        msg.id = nextMsg.id; //Set tje ID
        for(int i = 0; i <= 7; i++){
            msg.data[i] = nextMsg.buf[i]; //Set the data 
        }
    }
    return !queue.empty();
}
