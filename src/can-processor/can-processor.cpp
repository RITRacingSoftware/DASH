#include "can-processor.h"
#include "../lib/FlexCAN_T4-master/FlexCAN_T4.h"
#include "../controller/dash-controller.h"

#define CAN_BAUD_RATE 1000000
static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> myCan;

CAN_PROCESSOR::CAN_PROCESSOR()
{
  myCan.begin();
  myCan.setBaudRate(CAN_BAUD_RATE);
  myCan.enableFIFO();
  myCan.enableMBInterrupt(FIFO);
  myCan.setFIFOFilter(REJECT_ALL);
  myCan.setFIFOFilter(0, MC_FAULT_CODES_ID, MOTOR_POSITION_ID, STD);
  myCan.setFIFOFilter(1, READY_TO_DRIVE_ID, BMS_FAULTS_ID, STD);
  myCan.setFIFOFilter(2, BMS_CURRENT_ID, BMS_STATUS_ID, STD);
  myCan.setFIFOFilter(3, BMS_VOLTAGES_ID, STD);
};

bool CAN_PROCESSOR::readCAN(CAN_MESSAGE &msg)
{
  CAN_message_t nextMsg;
  bool messageRead = false;

  if (myCan.read(nextMsg)) // If there is a CAN Message in the queue, set msg
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