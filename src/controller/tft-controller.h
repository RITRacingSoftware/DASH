#include "../include/controller-intf.h"
#include "../include/data-processor-intf.h"
#include "etl/array.h"

class TFT_CONTROLLER : public CONTROLLER_INTF {
private:
  DATA_PROCESSOR_INTF *my_data_processor;

public:
  TFT_CONTROLLER(DATA_PROCESSOR_INTF *dataProcessor);

  ~TFT_CONTROLLER() = default;

  void initialize() override;

  void updateView() override;

  void updateModel() override;

  void processAccumulatorTemperature(etl::array<uint8_t, 8> const &data);
};