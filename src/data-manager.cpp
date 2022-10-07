#include "data-manager.h"

#include <cstdint>
#include <Arduino.h>

#include "can-manager.h"
#include "display-manager.h"
#include "../lib/Formula-DBC/formula_dbc.h"

// TODO: Migrate to DBC
#define CAN_ID_MOTOR_POSITION 0x0A5

namespace DataManager {
	car_data_t data;

	void init() {
		Serial.printf("Initializing DataManager\n");
		DisplayManager::init();
		CANManager::init();
		Serial.printf("Initialized DataManager\n");
	}

	void update() {
		CANManager::can_message_t message;
		while(CANManager::readMessage(&message)) {
			Serial.printf("Got CAN message, ID=%04x\n", message.id);

			if(message.id == FORMULA_DBC_BMS_STATUS_FRAME_ID) {
				formula_dbc_bms_status_t bms_status;
				formula_dbc_bms_status_unpack(&bms_status, message.data, message.len);
				uint8_t soc = bms_status.bms_status_soc;
			}

			if(message.id == CAN_ID_MOTOR_POSITION) {


				data.motorrpm = message.data[2] | (message.data[3] << 8);
				Serial.printf("New motor speed = %d RPM\n", data.motorrpm);
			}
		}

		DisplayManager::update(data);
	}
}
