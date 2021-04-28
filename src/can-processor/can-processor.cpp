#include "can-processor.h"
#include "../lib/FlexCAN_T4-master/FlexCAN_T4.h"

#define CAN_BAUD_RATE 1000000
static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> FD;

CAN_PROCESSOR::CAN_PROCESSOR()
{
  FD.begin();
  FD.setBaudRate(CAN_BAUD_RATE);
};

bool CAN_PROCESSOR::readCAN(CAN_MESSAGE &msg)
{
  CAN_message_t nextMsg;
  bool messageRead = false;

  if (FD.read(nextMsg)) // If there is a CAN Message in the queue, set msg
                        // fields with it
  {
    //Serial.printf("Message ID = %03X\n\r", nextMsg.id);
    msg.id = nextMsg.id;         // Set the ID
    for (int i = 0; i <= 7; i++) // n to hl if need to change endianness
    {
      //Serial.printf("Byte %d = %02X\n\r", i, nextMsg.buf[i]);
      msg.data[i] = nextMsg.buf[i]; // Set the data
    }
    messageRead = true;
  }
  return messageRead;
}