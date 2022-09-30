#ifndef CAN_MESSAGE_H_
#define CAN_MESSAGE_H_

#include "etl/queue.h"

// Structure for Can messages
class CAN_MESSAGE {
public:
  uint16_t id;
  uint8_t data[];

  CAN_MESSAGE() = default;
};

#endif /* CAN_MESSAGE_H_ */
