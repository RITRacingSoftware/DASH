#pragma once

#include <cstdint>

namespace CANManager {
	typedef struct can_message_s {
		uint16_t id;
		uint8_t data[8];
	} can_message_t;

	void init();
	void update();
	bool readMessage(can_message_t* out);
}
