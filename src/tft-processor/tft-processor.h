#ifndef TFT_PROCESSOR_H_
#define TFT_PROCESSOR_H_

#include "../../include/tft-processor-intf.h"
#include "../tft-display/tft-display.h"
#include "../../include/dash-controller-intf.h"
//#include "../../include/display-interfaces/display-item-intf.h"
#include "../tft-display/text-item/tft-text-item.h"
#include "../tft-display/rectangle-item/tft-rectangle-item.h"

class TFT_PROCESSOR : public TFT_PROCESSOR_INTF {
    private:
    
    TFT_TEXT_ITEM accumText;
    TFT_TEXT_ITEM motorSpeed;
    TFT_TEXT_ITEM busVoltage;
    TFT_RECTANGLE_ITEM busVoltRect;
    TFT_TEXT_ITEM outputVoltage;
    TFT_RECTANGLE_ITEM greenRect;
    TFT_RECTANGLE_ITEM whiteRect;
    DASH_CONTROLLER_INTF *myDashController;
    TFT_DISPLAY myDisplay;


    public:

    TFT_PROCESSOR(DASH_CONTROLLER_INTF *dashController);
    ~TFT_PROCESSOR() = default;

    void updateScreen() override;

    void initializeCallbacks() override;

    void updateAcumText(etl::array<uint8_t, 8> const &data);

    void MotorPositionInformation(etl::array<uint8_t, 8> const &data);

    void VoltageInfo(etl::array<uint8_t, 8> const &data);


};

#endif /* TFT_PROCESSOR_H_ */