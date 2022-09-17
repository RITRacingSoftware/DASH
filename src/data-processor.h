#ifndef DATA_PROCESSOR_H_
#define DATA_PROCESSOR_H_

#include "../include/data-processor-intf.h"
#include "etl/map.h"
#include "etl/delegate.h"
#include "can-processor.h"
#include "gpio-processor.h"
#include "../include/dash-controller-intf.h"
#include "etl/array.h"

#define MAX_MAP_SIZE 25

class DATA_PROCESSOR : public DATA_PROCESSOR_INTF
{
private:
  etl::map<const uint16_t, etl::delegate<void(etl::array<uint8_t, 8> const &)>,
           MAX_MAP_SIZE>
      my_callback_map;

  CAN_PROCESSOR canProcessor;
  DASH_CONTROLLER_INTF *myDashController;
  etl::array<uint16_t, MAX_MAP_SIZE> validIDs;
  //GPIO_PROCESSOR gpioProcessor;

public:
  DATA_PROCESSOR(DASH_CONTROLLER_INTF *dashController);
  ~DATA_PROCESSOR() {}

  bool
  registerCallback(uint16_t const &id,
                   etl::delegate<void(etl::array<uint8_t, 8> const &)> const
                       &callback) override;

  void registerData() override;

  void processData() override;

  void setPinData(uint8_t PIN, uint8_t value) override;
};

#endif /* DATA_PROCESSOR_H_ */
