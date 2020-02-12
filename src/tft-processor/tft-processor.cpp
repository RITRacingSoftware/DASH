#include "tft-processor.h"
#include "../../include/display-interfaces/display-item-intf.h"
#include "../tft-display/rectangle-item/tft-rectangle-item.h"
#include "../tft-display/text-item/tft-text-item.h"
#include "../tft-display/tft-display.h"
#include "etl/cstring.h"
#include "Arduino.h"
#include "../controller/dash-controller.h"

void TFT_PROCESSOR::updateScreen()
{
    //Serial.println("thisUpdate");
    myDisplay.updateScreen();
}

TFT_PROCESSOR::TFT_PROCESSOR(DASH_CONTROLLER_INTF *dashController) : accumText(0,0,0,RA8875_BLUE, RA8875_RED,"data = 000"),
motorSpeed(0,90,0,RA8875_RED, RA8875_RED,"Motor Speed = 000"), busVoltage(0,300,0,RA8875_WHITE, RA8875_RED,"Bus Voltage = 000"),
busVoltRect(300,0,200,20,RA8875_BLACK),
outputVoltage(0,0,30,RA8875_WHITE, RA8875_RED,"Output Voltage = 000"), greenRect(0,0,85,20,RA8875_GREEN), whiteRect(90,0,150,100,RA8875_CYAN),
myDashController(dashController), myDisplay(10,9){}

void TFT_PROCESSOR::initializeCallbacks()
{
    //create callbacks and then register them
    //Create elements and send pointer to addElemnt to register it

    Serial.printf("Original text address=%p, this=%p\n", &this->accumText, this);
    this->myDisplay.addElement(&accumText);
    this->myDisplay.addElement(&motorSpeed);
    this->myDisplay.addElement(&busVoltage);
    this->myDisplay.addElement(&busVoltRect);
    this->myDisplay.addElement(&outputVoltage);
    this->myDisplay.addElement(&greenRect);
    this->myDisplay.addElement(&whiteRect);
    


    Serial.printf("Address of accum text local=%p", &accumText);
    // myDashController->registerCallback(0xAB, [&] (etl::array<uint8_t, 8> const &data) {
    //     // char text[8];
    //     // for(int i = 0; i <=7; i++){
    //     //     text[i] = data[i];
    //     //     text[i] = i;
    //     // }

    //     char textString[50] = "data = changed";

    //     //sprintf(textString, "data = ", text);

    //     //Serial.println("here123");
    //     //If accumulator temp is 16 bit value, send 16 bits ntoh function to corrtect endianess
    //     Serial.printf("Address of accum text we are calling=%p\n", &accumText);
    //     accumText.updateText(textString);
    //     });
    
    myDashController->registerCallback();
    //NonMemberRegisterCallbacks(myDashController, this);

}

// void NonMemberRegisterCallbacks(DASH_CONTROLLER_INTF *dashController, TFT_PROCESSOR *processor){
//     dashController->registerCallback(0xAB, etl::delegate<void(etl::array<uint8_t, 8> const &)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::updateAcumText>(*processor));
// }

void TFT_PROCESSOR::updateAcumText(etl::array<uint8_t, 8> const &data) {
        //char text[8];
        // for(int i = 0; i <=7; i++){
        //     text[i] = data[i];
        //     text[i] = i;
        // }

        //char textString[50] = "data = changed";
        char text[MAX_STRING_SIZE];
        uint16_t number = data[0] | (data[1] << 8) | (data[2] << 16);
        //int num = number;
        Serial.print("number = ");
        Serial.println(number);

        sprintf(text, "data = %d", number);

        //sprintf(textString, "data = ", text);

        //Serial.println("here123");
        //If accumulator temp is 16 bit value, send 16 bits ntoh function to corrtect endianess
        Serial.printf("Address of accum text we are calling=%p\n", &accumText);
        accumText.updateText(text);
        }

void TFT_PROCESSOR::MotorPositionInformation(etl::array<uint8_t, 8> const &data){
    char motorSpeedNum[MAX_STRING_SIZE];
    uint16_t number = data[2] | (data[3] << 8);
    if(data[0] == 0){
        greenRect.updateColor(RA8875_RED);
    }
    sprintf(motorSpeedNum, "Motor Speed = %d", number);
    motorSpeed.updateText(motorSpeedNum);
}


void TFT_PROCESSOR::VoltageInfo(etl::array<uint8_t, 8> const &data){
    char outputVoltageNum[MAX_STRING_SIZE];
    uint16_t number = data[2] | (data[3] << 8);
    sprintf(outputVoltageNum, "Output Voltage = %d", number);
    outputVoltage.updateText(outputVoltageNum);
    char busVoltageNum[MAX_STRING_SIZE];
    number = data[0] | (data[1] << 8);
    sprintf(busVoltageNum, "Bus Voltage = %d", number);
    busVoltage.updateText(busVoltageNum);
}