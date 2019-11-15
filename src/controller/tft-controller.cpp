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

void TFT_CONTROLLER::initialize() {
  // Do nothing
}

void TFT_CONTROLLER::updateView() {
  // Do nothing
}

void TFT_CONTROLLER::updateModel() {
  // Do nothing
}

void TFT_CONTROLLER::processAccumulatorTemperature(
    etl::array<uint8_t, 8> const &data) {
  // Do nothing
}
