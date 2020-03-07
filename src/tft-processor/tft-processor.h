#ifndef TFT_PROCESSOR_H_
#define TFT_PROCESSOR_H_

#include "../../include/tft-processor-intf.h"
#include "../tft-display/tft-display.h"
#include "../../include/dash-controller-intf.h"
//#include "../../include/display-interfaces/display-item-intf.h"
#include "../tft-display/text-item/tft-text-item.h"
#include "../tft-display/rectangle-item/tft-rectangle-item.h"
#include "../tft-display/tft-display-item.h"

// struct displayItem{
//     TFT_TEXT_ITEM text;
//     TFT_RECTANGLE_ITEM rectangle;
// };

class TFT_PROCESSOR : public TFT_PROCESSOR_INTF
{
private:
    int lap;
    int batteryBeforeLap;
    int batteryPercent;

    DASH_CONTROLLER_INTF *myDashController;
    TFT_DISPLAY myDisplay; //PROBLEM IS WITH THE DISPLAY

    TFT_DISPLAY_ITEM faults;
    // TFT_TEXT_ITEM faultText;
    // TFT_RECTANGLE_ITEM faultTextRect;

    TFT_DISPLAY_ITEM motorSpeed;
    // TFT_TEXT_ITEM motorSpeed;
    // TFT_RECTANGLE_ITEM motorSpeedRect;

    TFT_DISPLAY_ITEM busVoltage;
    // TFT_TEXT_ITEM busVoltage;
    // TFT_RECTANGLE_ITEM busVoltRect;

    TFT_DISPLAY_ITEM outputVoltage;
    // TFT_TEXT_ITEM outputVoltage;
    // TFT_RECTANGLE_ITEM outputVoltageRect;

    TFT_DISPLAY_ITEM maxTemp;
    // TFT_TEXT_ITEM maxTemp;
    // TFT_RECTANGLE_ITEM maxTempRect;

    TFT_DISPLAY_ITEM packVoltage;
    // TFT_TEXT_ITEM packVoltage;
    // TFT_RECTANGLE_ITEM packVoltageRect;

    TFT_DISPLAY_ITEM batteryPercentage;
    // TFT_TEXT_ITEM batteryPercentage;
    // TFT_RECTANGLE_ITEM batteryPercentageRect;

    TFT_DISPLAY_ITEM batteryPerLap;
    // TFT_TEXT_ITEM batteryPerLap;
    // TFT_RECTANGLE_ITEM batteryPerLapRect;

    TFT_DISPLAY_ITEM lapNumber;
    // TFT_TEXT_ITEM lapNumber;
    // TFT_RECTANGLE_ITEM lapNumberRect;

    TFT_DISPLAY_ITEM waterTemp;
    // TFT_TEXT_ITEM waterTemp;
    // TFT_RECTANGLE_ITEM waterTempRect;

public:
    TFT_PROCESSOR(DASH_CONTROLLER_INTF *dashController);
    ~TFT_PROCESSOR() = default;

    void updateScreen() override;

    void initializeCallbacks() override;

    void updateFaultText(etl::array<uint8_t, 8> const &data);

    void MotorPositionInformation(etl::array<uint8_t, 8> const &data);

    void VoltageInfo(etl::array<uint8_t, 8> const &data);

    void AccumTemp(etl::array<uint8_t, 8> const &data);

    void AccumVoltage(etl::array<uint8_t, 8> const &data);

    void AccumCharge(etl::array<uint8_t, 8> const &data);

    void IncrementLap(etl::array<uint8_t, 8> const &data);

    void waterTempInfo(etl::array<uint8_t, 8> const &data);

    void readyToDriveMessage(etl::array<uint8_t, 8> const &data);
};

#endif /* TFT_PROCESSOR_H_ */