#include <Arduino.h>
#include "dash-controller.h"
#include "etl/delegate.h"
#include "etl/array.h"

#define FAULT_CODES_ID 0x0AB
#define MOTOR_POSITION_ID 0x0A5
#define VOLTAGE_INFO_ID 0x0A7

DASH_CONTROLLER::DASH_CONTROLLER()
  :my_tft_processor(this), my_data_processor(this)
{
  // this->my_data_processor.registerCallback(
  //     0xAB,
  //     etl::delegate<void(etl::array<uint8_t, 8> const &)>::create<
  //         DASH_CONTROLLER, &DASH_CONTROLLER::processAccumulatorTemperature>(
  //         *this));

  
  
}

void DASH_CONTROLLER::initialize() {
  this->my_tft_processor.initializeCallbacks();
}

void DASH_CONTROLLER::updateView() {
  this->my_tft_processor.updateScreen();
  // Do nothing
}

void DASH_CONTROLLER::updateModel() { this->my_data_processor.processData(); }

bool DASH_CONTROLLER::registerCallback()
                       {
                         my_data_processor.registerCallback(FAULT_CODES_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::updateAcumText>(my_tft_processor));
                         my_data_processor.registerCallback(MOTOR_POSITION_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::MotorPositionInformation>(my_tft_processor));
                         my_data_processor.registerCallback(VOLTAGE_INFO_ID, etl::delegate<void(etl::array<uint8_t, 8> const &data)>::create<TFT_PROCESSOR, &TFT_PROCESSOR::VoltageInfo>(my_tft_processor));
                       }

// void DASH_CONTROLLER::processAccumulatorTemperature(
//     etl::array<uint8_t, 8> const &data) {

//   pinMode(20, OUTPUT);
//   digitalWrite(20, HIGH);
//   Serial.println("Got message!!");
//   Serial.printf("Data: ");
//   for (int i = 0; i < 8; i++) {
//     Serial.printf("%u", data[i]);
//   }
//   Serial.println("");
// }
