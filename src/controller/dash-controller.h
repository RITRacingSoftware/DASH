#ifndef DASH_CONTROLLER_H_
#define DASH_CONTROLLER_H_

#include "../include/controller-intf.h"
#include "../../include/dash-controller-intf.h"
#include "../tft-processor/tft-processor.h"
#include "../data-processor/data-processor.h"
#include "../tft-display/tft-display.h"

class DASH_CONTROLLER : public DASH_CONTROLLER_INTF {
private:
   
   TFT_PROCESSOR my_tft_processor;
   DATA_PROCESSOR my_data_processor;
 

public:
  DASH_CONTROLLER();

  ~DASH_CONTROLLER() = default;

  void initialize() override;
  void updateView() override;
  void updateModel() override;
  bool registerCallback(uint16_t const &id,
                    etl::delegate<void(etl::array<uint8_t, 8> const &)> const
                        &callback) override;
                        
  void processAccumulatorTemperature(etl::array<uint8_t, 8> const &data); //Should be moved to tft processor

  
};

#endif /* DASH_CONTROLLER_H_ */