#include "tft-processor.h"
#include "../../include/display-interfaces/display-item-intf.h"
#include "../tft-display/rectangle-item/tft-rectangle-item.h"
#include "../tft-display/text-item/tft-text-item.h"
#include "../tft-display/tft-display.h"
#include "etl/cstring.h"
#include "Arduino.h"

void TFT_PROCESSOR::updateScreen()
{
    Serial.println("thisUpdate");
    myDisplay.updateScreen();
}

TFT_PROCESSOR::TFT_PROCESSOR(DASH_CONTROLLER_INTF *dashController) : accumText(25,10,10,RA8875_WHITE, RA8875_RED,"data = 000"), 
myDashController(dashController), myDisplay(10,9){}

void TFT_PROCESSOR::initializeCallbacks()
{
    //create callbacks and then register them
    //Create elements and send pointer to addElemnt to register it
    this->myDisplay.addElement(&accumText);


    etl::string<10> textString = "data = 000"; //Temporary
    accumText.updateText(textString); //Temporary


    myDashController->registerCallback(0xAB, [this] (etl::array<uint8_t, 8> const &data) {
        // char text[8];
        // for(int i = 0; i <=7; i++){
        //     text[i] = data[i];
        //     text[i] = i;
        // }
        etl::string<14> textString = "data = changed";

        //sprintf(textString, "data = ", text);
        

        accumText.updateText(textString);});

}
