#include "can-processor.h"
#include "../lib/FlexCAN_T4-master/FlexCAN_T4.h"
#include "../controller/dash-controller.h"

#define CAN_BAUD_RATE 1000000
static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> FD;

CAN_PROCESSOR::CAN_PROCESSOR()
{
  FD.begin();
  FD.setBaudRate(CAN_BAUD_RATE);
  FD.enableFIFO();
  FD.enableMBInterrupt(FIFO);
  FD.setFIFOFilter(REJECT_ALL);
  FD.setFIFOFilter(0, MC_FAULT_CODES_ID, MOTOR_POSITION_ID, STD);
  FD.setFIFOFilter(1, READY_TO_DRIVE_ID, BMS_FAULTS_ID, STD);
  FD.setFIFOFilter(2, BMS_CURRENT_ID, BMS_STATUS_ID, STD);
  FD.setFIFOFilter(3, BMS_VOLTAGES_ID, STD);
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