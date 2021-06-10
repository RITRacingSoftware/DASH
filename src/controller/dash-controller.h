#ifndef DASH_CONTROLLER_H_
#define DASH_CONTROLLER_H_

#include "../include/controller-intf.h"
#include "../../include/dash-controller-intf.h"
#include "../tft-processor/tft-processor.h"
#include "../data-processor/data-processor.h"
#include "../tft-display/tft-display.h"

//Define CAN message IDs
#define MC_FAULT_CODES_ID 0x0AB
#define MOTOR_POSITION_ID 0x0A5
//#define VOLTAGE_INFO_ID 0x0A7
// #define ACCUM_TEMP_ID 0x627
// #define ACCUM_VOLTAGE_ID 0x623
// #define ACCUM_CHARGE_ID 0x626
#define LAP_COMPLETE_ID 0x000
#define WATER_TEMP_ID 0x000
#define READY_TO_DRIVE_ID 0x0AA
#define BMS_FAULTS_ID 0x2BC
#define BMS_CHARGE_REQUEST_ID 0x0x1806e5f4u
#define BMS_CURRENT_ID 0x384
#define BMS_STATUS_ID 0x258
#define BMS_FAULT_ALERT_ID 0x2bd  //Don't need to use
#define BMS_VOLTAGES_ID 0x2be
#define BMS_THERMISTOR_VOLTAGES_ID 0x2bf
#define BMS_TEMPERATURES_ID 0x2c0
#define BMS_DRAIN_STATUS_A_ID 0x320
#define BMS_DRAIN_STATUS_B_ID 0x321
//IMPORTANT: IDs must be added to filters in order to be read

class DASH_CONTROLLER : public DASH_CONTROLLER_INTF
{
private:
  TFT_PROCESSOR my_tft_processor;
  DATA_PROCESSOR my_data_processor;

public:
  bool driveReady;

  DASH_CONTROLLER();

  ~DASH_CONTROLLER() = default;

  void initialize() override;
  void updateView() override;
  void updateModel() override;
  bool registerCallback() override;
  void readyToDrive() override;

  //void processAccumulatorTemperature(etl::array<uint8_t, 8> const &data); //Should be moved to tft processor
};

#endif /* DASH_CONTROLLER_H_ */