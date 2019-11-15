#include "../../include/data-processor-intf.h"
#include "etl/map.h"

#define MAX_MAP_SIZE 256

class DATA_PROCESSOR : public DATA_PROCESSOR_INTF {
private:
  etl::map<uint16_t, etl::delegate<void(etl::array<uint8_t, 8> const &)>,
           MAX_MAP_SIZE> my_callback_map;

public:
  DATA_PROCESSOR() {}

  bool
  registerCallback(uint16_t const &id,
                   etl::delegate<void(etl::array<uint8_t, 8> const &)> const
                       &callback) override;

  void processData();
};