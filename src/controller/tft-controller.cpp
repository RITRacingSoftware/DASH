#include <Arduino.h>
#include "tft-controller.h"
#include "etl/delegate.h"
#include "etl/array.h"

TFT_CONTROLLER::TFT_CONTROLLER(DATA_PROCESSOR_INTF *dataProcessor)
    : my_data_processor(dataProcessor) {

  this->my_data_processor->registerCallback(
      0xAB, etl::delegate<void(etl::array<uint8_t, 8> const &)>::create<
                TFT_CONTROLLER, &TFT_CONTROLLER::processAccumulatorTemperature>(
                *this));
}

void TFT_CONTROLLER::initialize() {}

void TFT_CONTROLLER::updateView() {
  // Do nothing
}

<<<<<<< HEAD
void TFT_CONTROLLER::updateModel() {
  //Serial.println("2");
  this->my_data_processor->processData();
}
=======
void TFT_CONTROLLER::updateModel() { this->my_data_processor->processData(); }
>>>>>>> 6d9f02759078aafa1638d08f8b47bb8b061a86c0

void TFT_CONTROLLER::processAccumulatorTemperature(
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
