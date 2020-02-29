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

TFT_PROCESSOR::TFT_PROCESSOR(DASH_CONTROLLER_INTF *dashController) : myDashController(dashController), 
myDisplay(10,9),
faultText(0,0,0,RA8875_WHITE, RA8875_RED,"No Faults"), faultTextRect(0,0,85,20,RA8875_GREEN),
motorSpeed(0,90,0,RA8875_WHITE, RA8875_RED,"Motor Speed = 000"),  motorSpeedRect(87,0,150,20,RA8875_RED),
busVoltage(0,300,0,RA8875_WHITE, RA8875_RED,"Bus Voltage = 000"),busVoltRect(295,0,150,20,RA8875_BLUE),
outputVoltage(0,0,30,RA8875_WHITE, RA8875_RED,"Output Voltage = 000"), outputVoltageRect(0,28,140,20,RA8875_BLACK),
maxTemp(1,205,225,RA8875_WHITE, RA8875_RED,"Tmax: 000"), maxTempRect(295,223,60,40,RA8875_BLACK),
packVoltage(1,25,225,RA8875_WHITE, RA8875_RED,"Vtotal: 000"), packVoltageRect(168,223,60,40,RA8875_BLACK),
batteryPercentage(1,0,60,RA8875_WHITE, RA8875_RED,"Battery% = 100"), batteryPercentageRect(175,58,50,40,RA8875_BLACK),
lapNumber(1,250,60,RA8875_WHITE, RA8875_RED,"Lap: 0"), lapNumberRect(318,58,50,45,RA8875_BLACK),
batteryPerLap(2,0,0,RA8875_WHITE, RA8875_RED,"Bat/Lap: 0"), batteryPerLapRect(200,0,125,50,RA8875_BLACK),
waterTemp(0,150,100,RA8875_WHITE, RA8875_RED,"Twater: 0"), waterTempRect(148,98,175,20,RA8875_BLACK)
{ 
    this->lap = 0;
    this->batteryBeforeLap = 100;
    this->batteryPercent = 100;
 }

void TFT_PROCESSOR::initializeCallbacks()
{
    //create callbacks and then register them
    //Create elements and send pointer to addElemnt to register it

    // this->myDisplay.addElement(&faultText);
    // this->myDisplay.addElement(&faultTextRect);
    // this->myDisplay.addElement(&motorSpeed);
    // this->myDisplay.addElement(&motorSpeedRect);
    // this->myDisplay.addElement(&busVoltage);
    // this->myDisplay.addElement(&busVoltRect);
    // this->myDisplay.addElement(&outputVoltage);
    // this->myDisplay.addElement(&outputVoltageRect);
    
    this->myDisplay.addElement(&maxTemp);
    this->myDisplay.addElement(&maxTempRect);
    // this->myDisplay.addElement(&packVoltage);
    // this->myDisplay.addElement(&packVoltageRect);
    // this->myDisplay.addElement(&batteryPercentage);
    // this->myDisplay.addElement(&batteryPercentageRect);
    this->myDisplay.addElement(&lapNumber);
    this->myDisplay.addElement(&lapNumberRect);
    this->myDisplay.addElement(&batteryPerLap); //Instead of updating every cycle, try not adding to elements array but only update when get message?
    this->myDisplay.addElement(&batteryPerLapRect);
    this->myDisplay.addElement(&waterTemp);
    this->myDisplay.addElement(&waterTempRect);

    // batteryPerLapRect.updateElement(myDisplay.getDisplayDriver());
    // batteryPerLap.updateElement(myDisplay.getDisplayDriver());
    // lapNumberRect.updateElement(myDisplay.getDisplayDriver());
    // lapNumber.updateElement(myDisplay.getDisplayDriver());
    
    //Register callbacks. Callbacks must be registered in DASH_CONTROLLER::registerCallbacks
    //for callbacks to be called
    myDashController->registerCallback(); 

}

void TFT_PROCESSOR::updateFaultText(etl::array<uint8_t, 8> const &data) {
     
    char text[MAX_STRING_SIZE];
    uint16_t number = data[0] | (data[1] << 8) | (data[2] << 16);
    Serial.print("number = ");
    Serial.println(number);
    sprintf(text, "Fault = %d", number);
    //If accumulator temp is 16 bit value, send 16 bits ntoh function to corrtect endianess
    faultText.updateText(text); 
}

void TFT_PROCESSOR::MotorPositionInformation(etl::array<uint8_t, 8> const &data){
    char motorSpeedNum[MAX_STRING_SIZE];
    uint16_t number = data[2] | (data[3] << 8);
    if(data[0] == 0){
        motorSpeedRect.updateColor(RA8875_RED);
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

void TFT_PROCESSOR::AccumTemp(etl::array<uint8_t, 8> const &data){
    char maxTempNum[MAX_STRING_SIZE];
    uint16_t number = data[4];
    sprintf(maxTempNum, "Tmax: %d", number);
    maxTemp.updateText(maxTempNum);
}

void TFT_PROCESSOR::AccumVoltage(etl::array<uint8_t, 8> const &data){
    char packVoltageNum[MAX_STRING_SIZE];
    uint16_t number = data[0] | (data[1] << 8);
    sprintf(packVoltageNum, "Vtotal: %d", number);
    packVoltage.updateText(packVoltageNum);
}

void TFT_PROCESSOR::AccumCharge(etl::array<uint8_t, 8> const &data){
    char batteryPercentNum[MAX_STRING_SIZE];
    uint16_t number = data[0];
    batteryPercent = number; //Update current battery percentage for battery/lap
    sprintf(batteryPercentNum, "Battery  = %d", number);
    Serial.println(batteryPercentNum);
    batteryPercentage.updateText(batteryPercentNum);
}

void TFT_PROCESSOR::IncrementLap(etl::array<uint8_t, 8> const &data){
    lap += 1;
    char lapNum[MAX_STRING_SIZE];
    uint16_t number = lap;
    sprintf(lapNum, "Lap: %d", number);
    lapNumber.updateText(lapNum); 


    //Determine battery used last lap
    char batPerLapNum[MAX_STRING_SIZE];
    int difference = batteryPercent - batteryBeforeLap; //Calulate battery used
    Serial.print("Differnce");
    Serial.println(number);
    Serial.print("Percent");
    Serial.println(batteryPercent);
    Serial.print("old");
    Serial.println(batteryBeforeLap);
    sprintf(batPerLapNum, "Bat/Lap: %d", difference);
    batteryPerLap.updateText(batPerLapNum);
    batteryBeforeLap = batteryPercent; //Set new starting percentage
}

void TFT_PROCESSOR::waterTempInfo(etl::array<uint8_t, 8> const &data){
    char waterTempNum[MAX_STRING_SIZE];
    uint16_t number = data[0]; //Change
    sprintf(waterTempNum, "Twater: %d", number);
    waterTemp.updateText(waterTempNum);
}