#include "data-processor.h"
#include "etl/map.h"
#include "can-processor/can-processor.h"
#include "etl/array.h"
#include "stdint.h"

#define MAX_SET_SIZE 80

//Structure for Can messages
struct CAN_MESSAGE{
    uint16_t id;
    etl::array<uint8_t, 8> data;
};


/*
@brief Registers a callback function for a particular CAN id. Ie when it gets a message of that can id it will
execute the function passed through
@param id The id to respond it
@param callback The function to call when it gets that ID
@return True if it was able to add the callback to the registry, false otherwise
The class using the dataprocessor will call register() to register all the callbacks on startup
*/
bool DATA_PROCESSOR::registerCallback(uint16_t const &id, etl::delegate<void(etl::array<uint8_t, 8> const &)> const
&callback)
{
    if(my_callback_map.full()){
        return false;
    }
    std::pair<uint16_t, etl::delegate<void(etl::array<uint8_t, 8> const &)>> pair (id, callback);
    my_callback_map.insert(pair);
    return true;
}

/*
  @brief reads the CAN_PROCESSOR and GPIO_POROCESSOR
  The class that uses the dataprocessor will call this function in an infinite loop to tell it to collect data
  */
  void DATA_PROCESSOR::processData()
  {
      bool readingCAN = true; //Create a variable for looping reading can, initialize to true to start loop
      while(readingCAN){ //Loop reading CAN messages until there aren't any messages left in the queue
          CAN_MESSAGE message; //Create an empty message to be populated with info
          readingCAN = canProcessor.readCAN(message); //Read the next can message into array at current index, and set whether there are messages left
          etl::delegate<void(etl::array<uint8_t, 8> const &)> func = my_callback_map.at(message.id); //Get the function associated with the id
          func(message.data); //Call the function with the data from the can message
      } 
  }