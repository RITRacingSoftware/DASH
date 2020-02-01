#ifndef TFT_PROCESSOR_H_
#define TFT_PROCESSOR_H_

#include "../../include/tft-processor-intf.h"
#include "../tft-display/tft-display.h"
#include "../../include/dash-controller-intf.h"
//#include "../../include/display-interfaces/display-item-intf.h"
#include "../tft-display/text-item/tft-text-item.h"

class TFT_PROCESSOR : public TFT_PROCESSOR_INTF {
    private:
    
    TFT_TEXT_ITEM accumText;
    DASH_CONTROLLER_INTF *myDashController;
    TFT_DISPLAY myDisplay;


    public:

    TFT_PROCESSOR(DASH_CONTROLLER_INTF *dashController);
    ~TFT_PROCESSOR() = default;

    void updateScreen() override;

    void initializeCallbacks() override;

};

#endif /* TFT_PROCESSOR_H_ */