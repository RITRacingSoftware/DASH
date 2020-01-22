#include "can-processor.h"
#include "../lib/FlexCAN_T4-master/FlexCAN_T4.h"

#define CAN_BAUD_RATE 500000
static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> FD;

<<<<<<< HEAD
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
=======
CAN_PROCESSOR::CAN_PROCESSOR() {
  FD.begin();
  FD.setBaudRate(CAN_BAUD_RATE);
};
>>>>>>> 6d9f02759078aafa1638d08f8b47bb8b061a86c0

bool CAN_PROCESSOR::readCAN(CAN_MESSAGE &msg) {
  CAN_message_t nextMsg;
  bool messageRead = false;

  if (FD.read(nextMsg)) // If there is a CAN Message in the queue, set msg
                        // fields with it
  {
<<<<<<< HEAD
    Serial.print("message");
    digitalWrite(13, LOW);
    CANFD_message_t nextMsg;
=======
    Serial.println("Read message");
>>>>>>> 6d9f02759078aafa1638d08f8b47bb8b061a86c0
    msg.id = nextMsg.id;         // Set the ID
    for (int i = 0; i <= 7; i++) // n to hl if need to change endianness
    {
      msg.data[i] = nextMsg.buf[i]; // Set the data
    }
    messageRead = true;
  }
  return messageRead;
}