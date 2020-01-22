#include <Arduino.h>
#include "dash-controller.h"
#include "etl/delegate.h"
#include "etl/array.h"

DASH_CONTROLLER::DASH_CONTROLLER(DATA_PROCESSOR_INTF *dataProcessor)
    : my_data_processor(dataProcessor) {

  this->my_data_processor->registerCallback(
      0xAB,
      etl::delegate<void(etl::array<uint8_t, 8> const &)>::create<
          DASH_CONTROLLER, &DASH_CONTROLLER::processAccumulatorTemperature>(
          *this));
}

void DASH_CONTROLLER::initialize() {}

void DASH_CONTROLLER::updateView() {
  // Do nothing
}

void DASH_CONTROLLER::updateModel() { this->my_data_processor->processData(); }

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
