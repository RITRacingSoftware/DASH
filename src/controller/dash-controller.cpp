#include <Arduino.h>
#include "dash-controller.h"
#include "etl/delegate.h"
#include "etl/array.h"



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
  this->my_tft_processor.clearScreen();
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
  my_data_processor.registerCallback(LAP_COMPLETE_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::IncrementLap>(my_tft_processor));
  my_data_processor.registerCallback(WATER_TEMP_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::waterTempInfo>(my_tft_processor));
  my_data_processor.registerCallback(READY_TO_DRIVE_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::readyToDriveMessage>(my_tft_processor));
  my_data_processor.registerCallback(BMS_FAULTS_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::bmsFaults>(my_tft_processor));
  my_data_processor.registerCallback(BMS_CURRENT_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::bmsCurrent>(my_tft_processor));
  my_data_processor.registerCallback(BMS_STATUS_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::bmsStatus>(my_tft_processor));
  my_data_processor.registerCallback(BMS_VOLTAGES_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::bmsVoltages>(my_tft_processor));

  return true;
}

void DASH_CONTROLLER::readyToDrive()
{
  this->driveReady = true;
}
