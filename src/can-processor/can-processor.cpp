#include "can-processor.h"
#include "../lib/FlexCAN_T4-master/FlexCAN_T4.h"

#define CAN_BAUD_RATE 500000
static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> FD;

CAN_PROCESSOR::CAN_PROCESSOR() {
  FD.begin();
  FD.setBaudRate(CAN_BAUD_RATE);
};

bool CAN_PROCESSOR::readCAN(CAN_MESSAGE &msg) {
  CAN_message_t nextMsg;
  bool messageRead = false;

  if (FD.read(nextMsg)) // If there is a CAN Message in the queue, set msg
                        // fields with it
  {

    Serial.println("Read message");
    Serial.print("ID = ");
    uint16_t bigVal = 0xABCD;
    uint8_t *val = (uint8_t*)&bigVal;
    char string_output[50];
    sprintf(string_output, "Byte 0=%02X, Byte 1=%02X\n", *val, val[1]);
    Serial.print(string_output);
    Serial.println(nextMsg.id,HEX);
    msg.id = nextMsg.id;         // Set the ID
    for (int i = 0; i <= 7; i++) // n to hl if need to change endianness
    {
      msg.data[7-i] = nextMsg.buf[i]; // Set the data
    }
    messageRead = true;
  }
  return messageRead;
}