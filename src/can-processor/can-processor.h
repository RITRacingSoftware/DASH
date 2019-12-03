#include "can-processor-intf.h"
#include "etl/queue.h"
#include "FlexCan_T4.h"

#define MAX_QUEUE_SIZE 80

class CAN_PROCESSOR : public CAN_PROCESSOR_INTF 
{
  private:
    etl::queue<CANFD_message_t, MAX_QUEUE_SIZE> queue;

    FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> FD;

  public:
    CAN_PROCESSOR() {FD.begin();};
    ~CAN_PROCESSOR(){};

    /*
    @brief Reads the CAN bus and populates a can data strucuture
    @param msg The message to poplulate if there is an available CAN message
    @return True if there is another can message available to read (in the queue), false otherwise
    The intended use is that whatever class is using should use a loop to call the function until it returns false. Then
    you know there are no CAN messages.
    */
    bool readCAN(CAN_MESSAGE & msg) override;
};