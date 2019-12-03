#include "../../include/data-processor-intf.h"
#include "etl/map.h"
#include "etl/delegate.h"


#define MAX_MAP_SIZE 256

class DATA_PROCESSOR : public DATA_PROCESSOR_INTF {
private:
  etl::map<uint16_t, etl::delegate<void(etl::array<uint8_t, 8> const &)>,
           MAX_MAP_SIZE> my_callback_map;
  
  CAN_PROCESSOR canProcessor;
  

public:
  DATA_PROCESSOR() {canProcessor = CAN_PROCESSOR();}
  ~DATA_PROCESSOR(){}

  /*
  @brief Registers a callback function for a particular CAN id. Ie when it gets a message of that can id it will
  execute the function passed through
  @param id The id to respond it
  @param callback The function to call when it gets that ID
  @return True if it was able to add the callback to the registry, false otherwise
  The class using the dataprocessor will call register() to register all the callbacks on startup
  */
  bool registerCallback(uint16_t const &id,
                   etl::delegate<void(etl::array<uint8_t, 8> const &)> const
                       &callback);

  /*
  @brief reads the CAN_PROCESSOR and GPIO_POROCESSOR
  The class that uses the dataprocessor will call this function in an infinite loop to tell it to collect data
  */
  void processData();
};