#ifndef DASH_CONTROLLER_H_
#define DASH_CONTROLLER_H_

#include "../include/controller-intf.h"
#include "../include/data-processor-intf.h"

class DASH_CONTROLLER : public CONTROLLER_INTF {
private:
  DATA_PROCESSOR_INTF *my_data_processor;

public:
  DASH_CONTROLLER(DATA_PROCESSOR_INTF *dataProcessor);

  ~DASH_CONTROLLER() = default;

  void initialize() override;

  void updateView() override;

  void updateModel() override;

  void processAccumulatorTemperature(etl::array<uint8_t, 8> const &data);
};

#endif /* DASH_CONTROLLER_H_ */