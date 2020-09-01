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
                                                                     myDisplay(10, 9),
                                                                     motorControllerFaults(TFT_TEXT_ITEM(0, 150, 125, RA8875_WHITE, RA8875_RED, "Motor Controller Faults: "), TFT_RECTANGLE_ITEM(148, 123, 175, 20, RA8875_BLACK)),
                                                                     motorSpeed(TFT_TEXT_ITEM(0, 90, 0, RA8875_WHITE, RA8875_RED, "Motor Speed = 000"), TFT_RECTANGLE_ITEM(87, 0, 150, 20, RA8875_RED)),
                                                                     busVoltage(TFT_TEXT_ITEM(0, 300, 0, RA8875_WHITE, RA8875_RED, "Bus Voltage = 000"), TFT_RECTANGLE_ITEM(295, 0, 150, 20, RA8875_BLUE)),
                                                                     outputVoltage(TFT_TEXT_ITEM(0, 0, 30, RA8875_WHITE, RA8875_RED, "Output Voltage = 000"), TFT_RECTANGLE_ITEM(0, 28, 140, 20, RA8875_BLACK)),
                                                                     maxTemp(TFT_TEXT_ITEM(1, 205, 225, RA8875_WHITE, RA8875_RED, "Tmax: 000"), TFT_RECTANGLE_ITEM(295, 223, 60, 40, RA8875_BLACK)),
                                                                     packVoltage(TFT_TEXT_ITEM(1, 25, 225, RA8875_WHITE, RA8875_RED, "Vtotal: 000"), TFT_RECTANGLE_ITEM(168, 223, 60, 40, RA8875_BLACK)),
                                                                     batteryPercentage(TFT_TEXT_ITEM(1, 0, 60, RA8875_WHITE, RA8875_RED, "Battery% = 100"), TFT_RECTANGLE_ITEM(175, 58, 50, 40, RA8875_BLACK)),
                                                                     lapNumber(TFT_TEXT_ITEM(1, 250, 60, RA8875_WHITE, RA8875_RED, "Lap: 0"), TFT_RECTANGLE_ITEM(318, 58, 50, 45, RA8875_BLACK)),
                                                                     batteryPerLap(TFT_TEXT_ITEM(2, 0, 0, RA8875_WHITE, RA8875_RED, "Bat/Lap: 0"), TFT_RECTANGLE_ITEM(200, 0, 125, 50, RA8875_BLACK)),
                                                                     waterTemp(TFT_TEXT_ITEM(0, 150, 100, RA8875_WHITE, RA8875_RED, "Twater: 0"), TFT_RECTANGLE_ITEM(148, 98, 175, 20, RA8875_BLACK)),
                                                                     BMSFaults(TFT_TEXT_ITEM(0, 150, 100, RA8875_WHITE, RA8875_RED, "BMS Faults: "), TFT_RECTANGLE_ITEM(148, 98, 175, 20, RA8875_BLACK))
{
    this->lap = 0;
    this->batteryBeforeLap = 100;
    this->batteryPercent = 100;
}

void TFT_PROCESSOR::initializeCallbacks()
{
    //create callbacks and then register them
    //Create elements and send pointer to addElemnt to register it

    this->myDisplay.addElement(&motorControllerFaults);
    // this->myDisplay.addElement(&faultTextRect);
    // this->myDisplay.addElement(&motorSpeed);
    // this->myDisplay.addElement(&motorSpeedRect);
    // this->myDisplay.addElement(&busVoltage);
    // this->myDisplay.addElement(&busVoltRect);
    // this->myDisplay.addElement(&outputVoltage);
    // this->myDisplay.addElement(&outputVoltageRect);

    this->myDisplay.addElement(&maxTemp);
    //this->myDisplay.addElement(&maxTempRect);
    // this->myDisplay.addElement(&packVoltage);
    // this->myDisplay.addElement(&packVoltageRect);
    // this->myDisplay.addElement(&batteryPercentage);
    // this->myDisplay.addElement(&batteryPercentageRect);
    this->myDisplay.addElement(&lapNumber);
    //this->myDisplay.addElement(&lapNumberRect);
    this->myDisplay.addElement(&batteryPerLap); //Instead of updating every cycle, try not adding to elements array but only update when get message?
    //this->myDisplay.addElement(&batteryPerLapRect);
    this->myDisplay.addElement(&waterTemp);
    //this->myDisplay.addElement(&waterTempRect);
    this->myDisplay.addElement(&BMSFaults);

    // batteryPerLapRect.updateElement(myDisplay.getDisplayDriver());
    // batteryPerLap.updateElement(myDisplay.getDisplayDriver());
    // lapNumberRect.updateElement(myDisplay.getDisplayDriver());
    // lapNumber.updateElement(myDisplay.getDisplayDriver());

    //Register callbacks. Callbacks must be registered in DASH_CONTROLLER::registerCallbacks
    //for callbacks to be called
    myDashController->registerCallback();
}

void TFT_PROCESSOR::updateMCFaultText(etl::array<uint8_t, 8> const &data)
{

    char text[MAX_STRING_SIZE];
    int numberFaults = 0;
    for (int i = 0; i <= 7; i++)
    {
        uint8_t thisByte = data[i];
        int thisBit = 0;
        while (thisBit <= 7)
        {
            if (thisByte & 0x01)
            {
                numberFaults++;
            }
            thisByte >> 1;
            thisBit++;
        }
    }
    if (numberFaults > 0)
    {
        //sprintf(text, "Number of Faults: ", numberFaults);
        // motorControllerFaults.updateText(text);
    }

    char faultsString[MAX_STRING_SIZE] = " ";

    //Use each byte of CAN data and List of fault messages to check all of the motor controller faults
    strncat(faultsString, checkMCFaults(data[0], MCByteZero), MAX_STRING_SIZE);
    strncat(faultsString, checkMCFaults(data[1], MCByteOne), MAX_STRING_SIZE);
    strncat(faultsString, checkMCFaults(data[2], MCByteTwo), MAX_STRING_SIZE);
    strncat(faultsString, checkMCFaults(data[3], MCByteThree), MAX_STRING_SIZE);
    strncat(faultsString, checkMCFaults(data[4], MCByteFour), MAX_STRING_SIZE);
    strncat(faultsString, checkMCFaults(data[5], MCByteFive), MAX_STRING_SIZE);
    strncat(faultsString, checkMCFaults(data[6], MCByteSix), MAX_STRING_SIZE);
    strncat(faultsString, checkMCFaults(data[7], MCByteSeven), MAX_STRING_SIZE);

    BMSFaults.updateText(faultsString);
    //If accumulator temp is 16 bit value, send 16 bits ntoh function to corrtect endianess
}

void TFT_PROCESSOR::MotorPositionInformation(etl::array<uint8_t, 8> const &data)
{
    char motorSpeedNum[MAX_STRING_SIZE];
    uint16_t number = data[2] | (data[3] << 8);
    if (data[0] == 0)
    {
        //motorSpeedRect.updateColor(RA8875_RED);
    }
    sprintf(motorSpeedNum, "Motor Speed = %d", number);
    motorSpeed.updateText(motorSpeedNum);
}

void TFT_PROCESSOR::VoltageInfo(etl::array<uint8_t, 8> const &data)
{
    char outputVoltageNum[MAX_STRING_SIZE];
    uint16_t number = data[2] | (data[3] << 8);
    sprintf(outputVoltageNum, "Output Voltage = %d", number);
    outputVoltage.updateText(outputVoltageNum);

    char busVoltageNum[MAX_STRING_SIZE];
    number = data[0] | (data[1] << 8);
    sprintf(busVoltageNum, "Bus Voltage = %d", number);
    busVoltage.updateText(busVoltageNum);
}

void TFT_PROCESSOR::AccumTemp(etl::array<uint8_t, 8> const &data)
{
    char maxTempNum[MAX_STRING_SIZE];
    uint16_t number = data[4];
    sprintf(maxTempNum, "Tmax: %d", number);
    maxTemp.updateText(maxTempNum);
}

void TFT_PROCESSOR::AccumVoltage(etl::array<uint8_t, 8> const &data)
{
    char packVoltageNum[MAX_STRING_SIZE];
    uint16_t number = data[0] | (data[1] << 8);
    sprintf(packVoltageNum, "Vtotal: %d", number);
    packVoltage.updateText(packVoltageNum);
}

void TFT_PROCESSOR::AccumCharge(etl::array<uint8_t, 8> const &data)
{
    char batteryPercentNum[MAX_STRING_SIZE];
    uint16_t number = data[0];
    batteryPercent = number; //Update current battery percentage for battery/lap
    sprintf(batteryPercentNum, "Battery  = %d", number);
    Serial.println(batteryPercentNum);
    batteryPercentage.updateText(batteryPercentNum);
}

void TFT_PROCESSOR::IncrementLap(etl::array<uint8_t, 8> const &data)
{
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

void TFT_PROCESSOR::waterTempInfo(etl::array<uint8_t, 8> const &data)
{
    char waterTempNum[MAX_STRING_SIZE];
    uint16_t number = data[0]; //Change
    sprintf(waterTempNum, "Twater: %d", number);
    waterTemp.updateText(waterTempNum);
}

void TFT_PROCESSOR::readyToDriveMessage(etl::array<uint8_t, 8> const &data)
{
    this->myDashController->readyToDrive();
}

boolean isFault(uint8_t status, uint8_t mask)
{
    return (status && mask) != 0;
}

void TFT_PROCESSOR::updateBMSFaults(etl::array<uint8_t, 8> const &data)
{
    // if(data[0] != 0)
    // {
    //     if (isFault(data[0], 0x01))
    //     {
    //         BMSFaults.addText("Master in Fault State");
    //     }
    //     //Didn't add bits 1-3, didn't know what they mean
    //     if(isFault(data[0], 0x04)){
    //         BMSFaults.addText("Relay Fault");
    //     }
    // }

    //Don't know what exactly is in this byte
    // if(data[4] != 0)
    // {
    //     if(isFault(data[4], 0x01))
    //     {
    //         BMSFaults.addText("Driving while pligged in");
    //     }
    //     if(isFault(data[4], 0x02)){
    //         BMSFaults.addText("Interlock tripped");
    //     }
    //     if(isFault(data[4], 0x04))
    //     {
    //         BMSFaults.addText("Communication fault with cell");
    //     }
    //     if(isFault(data[4], 0x08))
    //     {
    //         BMSFaults.addText("Charge overcurrent");
    //     }
    //     if(isFault(data[4], 0x05))
    //     {
    //         BMSFaults.addText("Driving while pligged in");
    //     }
    // }

    etl::string<MAX_STRING_SIZE> BMSFaultsString = etl::string<MAX_STRING_SIZE>(" ");

    checkBMSFaults(data[0], stateOfSystem);
    //Don't know how to decode Fault Codes byte
    checkBMSFaults(data[4], faultFlags);
    //Didn't add warnings
}

//Takes a byte of data and a list of 8 messages, and if a bit is set adds the corresponding message to the fault list
void TFT_PROCESSOR::checkBMSFaults(uint8_t data, etl::array<char[MAX_STRING_SIZE], 8> messages)
{
    //Only check each bit if one is set
    if (data != 0)
    {
        if (data && 0x01)
        {
            BMSFaults.addText(messages[0]);
        }
        if (data && 0x02)
        {
            BMSFaults.addText(messages[1]);
        }
        if (data && 0x04)
        {
            BMSFaults.addText(messages[2]);
        }
        if (data && 0x08)
        {
            BMSFaults.addText(messages[3]);
        }
        if (data && 0x10)
        {
            BMSFaults.addText(messages[4]);
        }
        if (data && 0x20)
        {
            BMSFaults.addText(messages[5]);
        }
        if (data && 0x40)
        {
            BMSFaults.addText(messages[6]);
        }
        if (data && 0x80)
        {
            BMSFaults.addText(messages[7]);
        }
    }
}

//Takes a byte of data and a list of 8 messages, and if a bit is set adds the corresponding message to the fault list
char *TFT_PROCESSOR::checkMCFaults(uint8_t data, etl::array<char[MAX_STRING_SIZE], 8> messages)
{
    char faults[MAX_STRING_SIZE];
    //Only check each bit if one is set
    if (data != 0)
    {
        if (data && 0x01)
        {
            strncat(faults, messages[0], MAX_STRING_SIZE);
            strncat(faults, ", ", MAX_STRING_SIZE);
        }
        if (data && 0x02)
        {
            strncat(faults, messages[1], MAX_STRING_SIZE);
            strncat(faults, ", ", MAX_STRING_SIZE);
        }
        if (data && 0x04)
        {
            strncat(faults, messages[2], MAX_STRING_SIZE);
            strncat(faults, ", ", MAX_STRING_SIZE);
        }
        if (data && 0x08)
        {
            strncat(faults, messages[3], MAX_STRING_SIZE);
            strncat(faults, ", ", MAX_STRING_SIZE);
        }
        if (data && 0x10)
        {
            strncat(faults, messages[4], MAX_STRING_SIZE);
            strncat(faults, ", ", MAX_STRING_SIZE);
        }
        if (data && 0x20)
        {
            strncat(faults, messages[5], MAX_STRING_SIZE);
            strncat(faults, ", ", MAX_STRING_SIZE);
        }
        if (data && 0x40)
        {
            strncat(faults, messages[6], MAX_STRING_SIZE);
            strncat(faults, ", ", MAX_STRING_SIZE);
        }
        if (data && 0x80)
        {
            strncat(faults, messages[7], MAX_STRING_SIZE);
            strncat(faults, ", ", MAX_STRING_SIZE);
        }
    }
    return faults;
}