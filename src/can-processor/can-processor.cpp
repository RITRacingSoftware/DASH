#include "can-processor.h"
#include "../lib/FlexCAN_T4-master/FlexCAN_T4.h"

static FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> FD;

CAN_PROCESSOR::CAN_PROCESSOR() 
{ 
  FD.begin(); 
  CANFD_timings_t config;
  config.clock = CLK_24MHz;
  config.baudrate = 512000;
  config.baudrateFD = 1024000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  FD.setRegions(64);
  FD.setBaudRate(config);
  FD.enableMBInterrupts();
  };

bool CAN_PROCESSOR::readCAN(CAN_MESSAGE &msg) {
  CANFD_message_t nextMsg;
  FD.read(nextMsg);
  bool messageRead = false;

  if (FD.read(nextMsg)) // If there is a CAN Message in the queue, set msg
                        // fields with it
  {
    Serial.print("message");
    digitalWrite(13, LOW);
    CANFD_message_t nextMsg;
    msg.id = nextMsg.id;         // Set the ID
    for (int i = 0; i <= 7; i++) // n to hl if need to change endianness
    {
      msg.data[i] = nextMsg.buf[i]; // Set the data
    }
    messageRead = true;
  }
  return messageRead;
}