#ifndef CAN_MESSAGE_H_
#define CAN_MESSAGE_H_

#include "etl/queue.h"

// Structure for Can messages
class CAN_MESSAGE {
public:
  uint16_t id;
  etl::array<uint8_t, 8> data;

  CAN_MESSAGE(uint16_t id_in, etl::array<uint8_t, 8> data_in)
      : id(id_in), data(data_in) {}

  CAN_MESSAGE() = default;
};

#endif /* CAN_MESSAGE_H_ */