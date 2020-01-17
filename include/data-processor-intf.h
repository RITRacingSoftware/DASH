#ifndef DATA_PROCESSOR_INTF_H_
#define DATA_PROCESSOR_INTF_H_

#include "etl/delegate.h"
#include "etl/array.h"

class DATA_PROCESSOR_INTF {
public:
  DATA_PROCESSOR_INTF() {}

  virtual ~DATA_PROCESSOR_INTF() {}

  /*
  @brief Registers a callback function for a particular CAN id. Ie when it gets
  a message of that can id it will
  execute the function passed through
  @param id The id to respond it
  @param callback The function to call when it gets that ID
  @return True if it was able to add the callback to the registry, false
  otherwise
  The class using the dataprocessor will call register() to register all the
  callbacks on startup
  */
  virtual bool registerCallback(
      uint16_t const &,
      etl::delegate<void(etl::array<uint8_t, 8> const &)> const &callback);

  /*
  @brief reads the CAN_PROCESSOR and GPIO_POROCESSOR
  The class that uses the dataprocessor will call this function in an infinite
  loop to tell it to collect data
  */
  virtual void registerData();

  virtual void setPinData(uint8_t PIN, uint8_t value);

  virtual void processData();
};

#endif /* DATA_PROCESSOR_INTF_H_ */