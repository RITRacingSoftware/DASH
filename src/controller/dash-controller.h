#ifndef DASH_CONTROLLER_H_
#define DASH_CONTROLLER_H_

#include "../include/controller-intf.h"
#include "../../include/dash-controller-intf.h"
#include "../../include/data-processor-intf.h"
#include "../../include/tft-processor-intf.h"
#include "../tft-display/tft-display.h"

class DASH_CONTROLLER : public DASH_CONTROLLER_INTF {
private:
  DATA_PROCESSOR_INTF *my_data_processor;
  TFT_PROCESSOR_INTF *my_tft_processor;


public:
  DASH_CONTROLLER();

  ~DASH_CONTROLLER() = default;

  void initialize() override;
  void updateView() override;
  void updateModel() override;
  void registerCallback(uint16_t const &id,
                    etl::delegate<void(etl::array<uint8_t, 8> const &)> const
                        &callback) override;
                        
  void processAccumulatorTemperature(etl::array<uint8_t, 8> const &data); //Should be moved to tft processor

  
};

#endif /* DASH_CONTROLLER_H_ */