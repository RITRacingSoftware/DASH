#include <Arduino.h>
#include "dash-controller.h"
#include "etl/delegate.h"
#include "etl/array.h"


DASH_CONTROLLER::DASH_CONTROLLER()
  :my_tft_processor(this), my_data_processor(this)
{
  this->my_data_processor.registerCallback(
      0xAB,
      etl::delegate<void(etl::array<uint8_t, 8> const &)>::create<
          DASH_CONTROLLER, &DASH_CONTROLLER::processAccumulatorTemperature>(
          *this));

  
  
}

void DASH_CONTROLLER::initialize() {
  this->my_tft_processor.initializeCallbacks();
}

void DASH_CONTROLLER::updateView() {
  this->my_tft_processor.updateScreen();
  // Do nothing
}

void DASH_CONTROLLER::updateModel() { this->my_data_processor.processData(); }

bool DASH_CONTROLLER::registerCallback(uint16_t const &id,
                   etl::delegate<void(etl::array<uint8_t, 8> const &)> const
                       &callback)
                       {
                         return my_data_processor.registerCallback(id, callback);
                       }

void DASH_CONTROLLER::processAccumulatorTemperature(
    etl::array<uint8_t, 8> const &data) {

  pinMode(20, OUTPUT);
  digitalWrite(20, HIGH);
  Serial.println("Got message!!");
  Serial.printf("Data: ");
  for (int i = 0; i < 8; i++) {
    Serial.printf("%u", data[i]);
  }
  Serial.println("");
}
