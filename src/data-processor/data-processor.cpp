#include "data-processor.h"
#include "etl/map.h"
#include "can-processor/can-processor.h"
#include "etl/array.h"
#include "stdint.h"


bool DATA_PROCESSOR::registerCallback(uint16_t const &id, etl::delegate<void(etl::array<uint8_t, 8> const &)> const
&callback)
{
    if(my_callback_map.full())
    {
        return false;
    }
    my_callback_map.insert({id, callback});
    return true;
}

  void DATA_PROCESSOR::processData()
  {
      bool readingCAN = true; //Create a variable for looping reading can, initialize to true to start loop
      while(readingCAN)//Loop reading CAN messages until there aren't any messages left in the queue
      { 
          CAN_MESSAGE message; //Create an empty message to be populated with info
          readingCAN = canProcessor.readCAN(message); //Read the next can message into array at current index, and set whether there are messages left
          etl::delegate<void(etl::array<uint8_t, 8> const &)> func = my_callback_map.at(message.id); //Get the function associated with the id
          func(message.data); //Call the function with the data from the can message
      } 
  }