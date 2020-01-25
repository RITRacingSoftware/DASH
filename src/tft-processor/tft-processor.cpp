#include "tft-processor.h"


TFT_PROCESSOR::TFT_PROCESSOR(DASH_CONTROLLER_INTF *dashController)
: myDashController(dashController), myDisplay(10,9)
{


}

void TFT_PROCESSOR::updateScreen()
{
    myDisplay.updateScreen();
}

void TFT_PROCESSOR::initializeCallbacks()
{
    //create callbacks and then register them
    //Create elements and send pointer to addElemnt to register it
}
