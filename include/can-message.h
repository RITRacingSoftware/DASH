#pragma once

// Structure for Can messages
class CAN_MESSAGE {
public:
  uint16_t id;
  uint8_t data[8];

  CAN_MESSAGE() = default;
};
