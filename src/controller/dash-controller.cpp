#include <Arduino.h>
#include "dash-controller.h"
#include "etl/delegate.h"
#include "etl/array.h"

//Define CAN message IDs
#define MC_FAULT_CODES_ID 0x0AB
#define MOTOR_POSITION_ID 0x0A5
#define VOLTAGE_INFO_ID 0x0A7
#define ACCUM_TEMP_ID 0x627
#define ACCUM_VOLTAGE_ID 0x623
#define ACCUM_CHARGE_ID 0x626
#define LAP_COMPLETE_ID 0x000
#define WATER_TEMP_ID 0x000
#define READY_TO_DRIVE_ID 0x0AA
#define BMS_FAULTS_ID 0x622

// Mask
#define STATE_MASK (0xFFFF << (6 * 8))

DASH_CONTROLLER::DASH_CONTROLLER()
    : my_tft_processor(this), my_data_processor(this)
{
  this->driveReady = false;
}

void DASH_CONTROLLER::initialize()
{
  this->my_tft_processor.initializeCallbacks();
}

void DASH_CONTROLLER::updateView()
{
  this->my_tft_processor.updateScreen();
  // Do nothing
}

void DASH_CONTROLLER::updateModel() { this->my_data_processor.processData(); }

bool DASH_CONTROLLER::registerCallback()
{
  my_data_processor.registerCallback(MC_FAULT_CODES_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::updateMCFaultText>(my_tft_processor));
  my_data_processor.registerCallback(MOTOR_POSITION_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::MotorPositionInformation>(my_tft_processor));
  my_data_processor.registerCallback(VOLTAGE_INFO_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::VoltageInfo>(my_tft_processor));
  my_data_processor.registerCallback(ACCUM_TEMP_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::AccumTemp>(my_tft_processor));
  my_data_processor.registerCallback(ACCUM_VOLTAGE_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::AccumVoltage>(my_tft_processor));
  my_data_processor.registerCallback(ACCUM_CHARGE_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::AccumCharge>(my_tft_processor));
  my_data_processor.registerCallback(LAP_COMPLETE_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::IncrementLap>(my_tft_processor));
  my_data_processor.registerCallback(WATER_TEMP_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::waterTempInfo>(my_tft_processor));
  my_data_processor.registerCallback(READY_TO_DRIVE_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::readyToDriveMessage>(my_tft_processor));
  my_data_processor.registerCallback(BMS_FAULTS_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::updateBMSFaults>(my_tft_processor));
}

void DASH_CONTROLLER::readyToDrive()
{
  this->driveReady = true;
}
