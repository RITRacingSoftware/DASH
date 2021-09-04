#include "can-processor.h"
#include "../lib/FlexCAN_T4-master/FlexCAN_T4.h"

#define CAN_BAUD_RATE 1000000
static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> FD;

CAN_PROCESSOR::CAN_PROCESSOR()
{
  FD.begin();
  FD.setBaudRate(CAN_BAUD_RATE);
  // uint16_t id = 0x696;
  // uint32_t other = 0;
  // temp.insert({id,other});
  // for(uint16_t i = 0xA0;i<=0xAE; i++){
  //   id = i;

  //   //temp.insert({id,other});
  // }
  for(uint8_t i = 0; i<20;i++){
    numMessages.insert_at(i,0);
  }

};

bool CAN_PROCESSOR::readCAN(CAN_MESSAGE &msg)
{
  CAN_message_t nextMsg;
  bool messageRead = false;

  if (FD.read(nextMsg)) // If there is a CAN Message in the queue, set msg
                        // fields with it
  {
    uint8_t index = 0;
    switch(nextMsg.id){
      case 0x696 : index=0;
      break;
      case 0xA0 : index=1;
      break;
      case 0xA1 : index=2;
      break;
      case 0xA2 : index=3;
      break;
      case 0xA3 : index=4;
      break;
      case 0xA4 : index=5;
      break;
      case 0xA5 : index=6;
      break;
      case 0xA6 : index=7;
      break;
      case 0xA7 : index=8;
      break;
      case 0xA8 : index=9;
      break;
      case 0xA9 : index=10;
      break;
      case 0xAA : index=11;
      break;
      case 0xAB : index=12;
      break;
      case 0xAC : index=13;
      break;
      case 0xAD : index=14;
      break;
      case 0xAE : index=15;
      break;

    }
    uint32_t thisss = numMessages.at(index);
    thisss++;
    numMessages.insert_at(index,thisss);
    for(int i = 0;i<=15; i++){
      Serial.printf("%d     %d\n",i,numMessages.at(i));
    }
    // uint32_t numOccurances = temp.at(nextMsg.id);
    // //Serial.print("2");
    // numOccurances++;
    // //Serial.print("3");
    // Serial.printf("ID: %d   Ocurrances: %d \n", 0x696, temp.at(0x696));
    // //Serial.print("4");
    // for(uint16_t i = 0xA0;i<=0xAE; i++){
    //   Serial.printf("ID: %d   Ocurrances: %d \n", i, temp.at(i));
    //   //Serial.printf("%d");
    // }

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