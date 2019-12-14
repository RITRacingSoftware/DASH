#include "../lib/FlexCAN_T4-master/FlexCAN_T4.h"
#include "Arduino.h"
#include <iostream>
using namespace std;

FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> FD;

const int ID_Filter = 0x0A5; //Motor Position Information message ID

void setup(void) {
  //Serial.begin(115200); 
  delay(500);
  pinMode(6, OUTPUT); digitalWrite(6, LOW);
  FD.begin();
  FD.setRegions(64);
  //    FD.setMB(MB6, RX);
  //    FD.setMB(MB11, RX);
  //    FD.setMB(MB22, RX);
  //    FD.setMB(MB31, RX);
  //      FD.setMB(MB45, TX);
}

void loop() {
  CANFD_message_t msg;
  /*msg.len = 8; msg.id = 0x321;
  msg.buf[0] = 1; msg.buf[1] = 2; msg.buf[2] = 3; msg.buf[3] = 4;
  msg.buf[4] = 5; msg.buf[5] = 6; msg.buf[6] = 7; msg.buf[7] = 8;
  FD.write(msg); */
  if ( FD.read(msg) ) {
    if(msg.id == ID_Filter){
      cout << "MB : " << msg.mb << "\n";
      cout <<"  ID: 0x" << msg.id << "\n";
      cout << "  EXT: " << msg.flags.extended << "\n";
      cout << "  LEN: " << msg.len << "\n";
      cout << " DATA: " << "\n";
      cout << " Motor Angle: " << msg.buf[0] << " " << msg.buf[1] << "\n";
      cout << " Motor Speed: " << msg.buf[2] << " " << msg.buf[3] << "\n";
      cout << " Electircal Output Frequency: " << msg.buf[4] << " " << msg.buf[5] << "\n";
      cout << " Delta Resolver Filtered: " << msg.buf[6] << " " << msg.buf[7] << "\n";
      /*for ( uint8_t i = 0; i < 8; i++ ) {
        Serial.print(msg.buf[i]); Serial.print(" ");
     }*/
      //Serial.print("  TS: "); Serial.println(msg.timestamp);
    }
    
  }
}
