#ifndef DATA_PROCESSOR_H_
#define DATA_PROCESSOR_H_

#include "../../include/data-processor-intf.h"
#include "etl/map.h"
#include "etl/delegate.h"
#include "can-processor/can-processor.h"

#define MAX_MAP_SIZE 25

class DATA_PROCESSOR : public DATA_PROCESSOR_INTF {
private:
  etl::map<uint16_t, etl::delegate<void(etl::array<uint8_t, 8> const &)>,
           MAX_MAP_SIZE> my_callback_map;

  CAN_PROCESSOR canProcessor;

public:
  DATA_PROCESSOR() {}
  ~DATA_PROCESSOR() {}

  bool
  registerCallback(uint16_t const &id,
                   etl::delegate<void(etl::array<uint8_t, 8> const &)> const
                       &callback) override;

  void registerData() override;

  void processData();
};

#endif /* DATA_PROCESSOR_H_ */