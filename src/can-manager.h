#pragma once

#include <cstdint>

namespace CANManager {
	typedef struct can_message_s {
		uint16_t id;
		uint8_t len;
		uint8_t data[8];
	} can_message_t;

	void init();
	bool hasMessage();
	can_message_t getMessage();
}
