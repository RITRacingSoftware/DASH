#include "data-processor.h"
#include "etl/map.h"
#include "can-processor/can-processor.h"
#include "etl/array.h"
#include "stdint.h"
#include "Arduino.h"
#include "data-processor/data-processor.h"

// static bool isBMSmessage(uint16_t id){
//   return ((id = BMS_FAULTS_ID) || (id = BMS_CHARGE_REQUEST_ID) || (id = BMS_CURRENT_ID) || (id = BMS_STATUS_ID) || (id = BMS_FAULT_ALERT_ID) || 
//       (id = BMS_VOLTAGES_ID) || (id = BMS_THERMISTOR_VOLTAGES_ID ) || (id = BMS_TEMPERATURES_ID) || (id = BMS_DRAIN_STATUS_A_ID) || (id = BMS_DRAIN_STATUS_B_ID));
// }

DATA_PROCESSOR::DATA_PROCESSOR(DASH_CONTROLLER_INTF *dashController)
{
  myDashController = dashController;
}

bool DATA_PROCESSOR::registerCallback(
    uint16_t const &id,
    etl::delegate<void(etl::array<uint8_t, 8> const &)> const &callback)
{
  if (my_callback_map.full())
  {
    return false;
  }
  my_callback_map.insert({id, callback});
  return true;
}

void DATA_PROCESSOR::processData()
{
  bool readingCAN = true; // Create a variable for looping reading can,
                          // initialize to true to start loop
  while (readingCAN)      // Loop reading CAN messages until there aren't any
                          // messages left in the queue
  {
    CAN_MESSAGE message;                        // Create an empty message to be populated with info
    readingCAN = canProcessor.readCAN(message); // Read the next can
    // message
    // into array at current index,
    // and set whether there are
    // messages left
    if (readingCAN)
    {
      // if(isBMSmessage(message.id)){

      // }
      if (this->my_callback_map.find(message.id) != this->my_callback_map.end())
      {
        etl::delegate<void(etl::array<uint8_t, 8> const &)> func =
            this->my_callback_map.at(message.id);
        if (func.is_valid())
        {
          Serial.printf("Running callback for id %02X\n\r", message.id);
          func(message.data);
        } // Call the function with the data from the can
      }
    } // Get the function associated with the id}
    // message
  }
}

void DATA_PROCESSOR::registerData() {}

void DATA_PROCESSOR::setPinData(uint8_t PIN, uint8_t value)
{
  //this->gpioProcessor.writeGPIO(PIN, value);
}