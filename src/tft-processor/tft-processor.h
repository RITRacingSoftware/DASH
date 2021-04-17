#ifndef TFT_PROCESSOR_H_
#define TFT_PROCESSOR_H_

#include "../../include/tft-processor-intf.h"
#include "../tft-display/tft-display.h"
#include "../../include/dash-controller-intf.h"
//#include "../../include/display-interfaces/display-item-intf.h"
#include "../tft-display/text-item/tft-text-item.h"
#include "../tft-display/rectangle-item/tft-rectangle-item.h"
#include "../tft-display/tft-display-item.h"
#include "etl/list.h"
#include <string.h>
#include "../f29bms_dbc.h"

// struct displayItem{
//     TFT_TEXT_ITEM text;
//     TFT_RECTANGLE_ITEM rectangle;
// };

typedef struct
{
    struct f29bms_dbc_bms_status_t bms_status;
    struct f29bms_dbc_bms_fault_vector_t bms_fault_vector;
    struct f29bms_dbc_bms_fault_alert_t bms_fault_alert;
    struct f29bms_dbc_bms_voltages_t bms_voltages;
    struct f29bms_dbc_bms_thermistor_voltages_t bms_thermistor_voltages;
    struct f29bms_dbc_bms_temperatures_t bms_temperatures;
    struct f29bms_dbc_bms_drain_status_a_t bms_drain_status_a;
    struct f29bms_dbc_bms_drain_status_b_t bms_drain_status_b;
    struct f29bms_dbc_bms_current_t bms_current;
    struct f29bms_dbc_bms_charge_request_t bms_charge_request;
} CAN_BUS;

class TFT_PROCESSOR : public TFT_PROCESSOR_INTF
{
private:
    int lap;
    int batteryBeforeLap;
    // int batteryPercent;
    double maxCurrent;
    double maxVoltage;
    double minVoltage;
    char *previoustMCFaultString;
    char *previousBMSFaultString;
    uint64_t prevFaultVector;
    int SOC;
    int SOCRaw;
    double packVoltage;
    int maxVoltageCell;

    CAN_BUS canBus;


    DASH_CONTROLLER_INTF *myDashController;
    TFT_DISPLAY myDisplay; 

    TFT_DISPLAY_ITEM motorControllerFaults;

    TFT_DISPLAY_ITEM motorSpeed;
    TFT_DISPLAY_ITEM motorSpeedLabel;

    //TFT_DISPLAY_ITEM busVoltage;

    //TFT_DISPLAY_ITEM outputVoltage;

    //TFT_DISPLAY_ITEM maxTemp;

    //TFT_DISPLAY_ITEM packVoltage;

    //TFT_DISPLAY_ITEM batteryPercentage;

    TFT_DISPLAY_ITEM batteryPerLap;

    TFT_DISPLAY_ITEM lapNumber;

    TFT_DISPLAY_ITEM waterTemp;

    TFT_DISPLAY_ITEM BMSFaults;

    TFT_DISPLAY_ITEM BMSFaultVector;

    TFT_DISPLAY_ITEM ReadyToDriveStatus;

    TFT_RECTANGLE_ITEM MotorSpeedBar;

    TFT_DISPLAY_ITEM BMSMaxCurrent;
    TFT_DISPLAY_ITEM BMSMaxCurrentLabel;

    TFT_DISPLAY_ITEM BMSMinVoltage;
    //TFT_DISPLAY_ITEM BMSMinVoltageLabel;
    

    TFT_DISPLAY_ITEM BMSMaxVoltage;
    //TFT_DISPLAY_ITEM BMSMaxVoltageLabel;

    TFT_DISPLAY_ITEM BMSCurrentCurrent;
    TFT_DISPLAY_ITEM BMSCurrentCurrentLabel;

    TFT_DISPLAY_ITEM BMSSOC;
    
    TFT_DISPLAY_ITEM BMSSOCRaw;

    TFT_DISPLAY_ITEM BMSPackVoltage;
     

    // const etl::array<char[MAX_STRING_SIZE], 8> stateOfSystem = {{"BMS Master in Fault State", "??", "??", "??", "Relay Fault", "??", "??", "??"}};
    // const etl::array<char[MAX_STRING_SIZE], 8> faultFlags = {{"Driving while plugged in", "Interlock tripped", "Communication fault with cell",
                                                            //   "Charge overcurrent", "Discharge overcurrent", "Over-temperature", "Under voltage", "Over voltage"}};
    const etl::array<char[MAX_STRING_SIZE], 8> MCByteZero = {{"Hardware Gate/Desaturation Fault", "HW Over-current Fault", "Accelerator Shorted",
                                                              "Accelerator Open", "Current Sensor Low", "Current Sensor High", "Module Temperature Low", "Module Temperature High"}};
    const etl::array<char[MAX_STRING_SIZE], 8> MCByteOne = {{"Control PCB Temperature Low", "Control PCB Temperature High", "Gate Drive PCB Temperature Low",
                                                             "Gate Drive PCB Temperature High", "5V Sense Voltage Low", "5V Sense Voltage Low", "12V Sense Voltage Low", "12V Sense Voltage Low"}};
    const etl::array<char[MAX_STRING_SIZE], 8> MCByteTwo = {{"2.5V Sense Voltage Low", "2.5V Sense Voltage Low", "1.5V Sense Voltage Low", "1.5V Sense Voltage Low",
                                                             "DC Bus Voltage High", "DC Bus Voltage Low", "Pre-charge Timeout", "Pre-charge Voltage Failure"}};
    const etl::array<char[MAX_STRING_SIZE], 8> MCByteThree = {{"EEPROM Checksum Invalid", "EEPROM Data Out of Range", "EEPROM Update Required", "??", "??", "??",
                                                               "Brake Shorted", "Brake Open"}};
    const etl::array<char[MAX_STRING_SIZE], 8> MCByteFour = {{"Motor Over-speed Fault", "Over-current Fault", "Over-voltage Fault", "Inverter Over-temperature Fault",
                                                              "Accelerator Input Shorted Fault", "Accelerator Input Open Fault", "Direction Command Fault", "Inverter Response Time-out Fault"}};
    const etl::array<char[MAX_STRING_SIZE], 8> MCByteFive = {{"Hardware Gate/Desaturation Fault", "Hardware Over-current Fault", "Under-voltage Fault",
                                                              "CAN Command Message Lost Fault", "Motor Over-temperature Fault", "??", "??", "??"}};
    const etl::array<char[MAX_STRING_SIZE], 8> MCByteSix = {{"Brake Input Shorted Fault", "Brake Input Open Fault", "Module A Over-temperature Fault",
                                                             "Module B Over-temperature Fault", "Module C Over-temperature Fault", "PCB Over-temperature Fault", "Gate Drive Board 1 Over-temperature Fault", "Gate Drive Board 2 Over-temperature Fault"}};
    const etl::array<char[MAX_STRING_SIZE], 8> MCByteSeven = {{"Gate Drive Board 3 Over-temperatuer Fault", "Current Sensor Fault", "??", "Hardware Over-Voltage Fault", "??", "??", "Resolver Not Connected", "Inverter Discharge Active"}};

public:
    TFT_PROCESSOR(DASH_CONTROLLER_INTF *dashController);
    ~TFT_PROCESSOR() = default;

    void updateScreen() override;

    void initializeCallbacks() override;

    void updateMCFaultText(etl::array<uint8_t, 8> const &data);

    void MotorPositionInformation(etl::array<uint8_t, 8> const &data);

    //void VoltageInfo(etl::array<uint8_t, 8> const &data);

    //void AccumTemp(etl::array<uint8_t, 8> const &data);

    //void AccumVoltage(etl::array<uint8_t, 8> const &data);

    //void AccumCharge(etl::array<uint8_t, 8> const &data);

    void IncrementLap(etl::array<uint8_t, 8> const &data);

    void waterTempInfo(etl::array<uint8_t, 8> const &data);

    void readyToDriveMessage(etl::array<uint8_t, 8> const &data);

    //void updateBMSFaults(etl::array<uint8_t, 8> const &data);

    void bmsFaults(etl::array<uint8_t, 8> const &data);

    void bmsCurrent(etl::array<uint8_t, 8> const &data);

    void bmcCharge(etl::array<uint8_t, 8> const &data);

    void bmsStatus(etl::array<uint8_t, 8> const &data);

    void bmsFaultAlert(etl::array<uint8_t, 8> const &data);

    void bmsVoltages(etl::array<uint8_t, 8> const &data);

    void bmsThermistorVoltages(etl::array<uint8_t, 8> const &data);

    void bmsTemperatures(etl::array<uint8_t, 8> const &data);

    void bmsDrainStatusA(etl::array<uint8_t, 8> const &data);

    void bmsDrainSatatusB(etl::array<uint8_t, 8> const &data);

    void checkFaults(uint8_t data, etl::array<char[MAX_STRING_SIZE], 8> message, char faultOutString[MAX_STRING_SIZE]);

    void clearScreen() override;

    void test();

};

#endif /* TFT_PROCESSOR_H_ */