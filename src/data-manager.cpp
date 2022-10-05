#include "data-manager.h"

#include <cstdint>

#include "can-manager.h"
#include "display-manager.h"

#define CAN_ID_BMS_STATUS 0x258
#define CAN_ID_MOTOR_POSITION 0x0A5

namespace DataManager {
	car_data_t data;

	void init() {
		DisplayManager::init();
		CANManager::init();
	}

	void update() {
		CANManager::can_message_t message;
		while(CANManager::readMessage(&message)) {
			if(message.id == CAN_ID_MOTOR_POSITION) {
				data.motorrpm = message.data[2] | (message.data[3] << 8);
			}
		}

		DisplayManager::update(data);
	}
}
