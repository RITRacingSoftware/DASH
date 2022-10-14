#include "data-manager.h"

#include <cstdint>
#include <Arduino.h>

#include "can-manager.h"
#include "display-manager.h"
#include "../lib/Formula-DBC/formula_dbc.h"

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
				data.chargepercent = bms_status.bms_status_soc;
				Serial.printf("New battery SOC = %d%%\n", data.chargepercent);
			}

			if(message.id == FORMULA_DBC_MCU_MOTOR_POSITION_INFO_FRAME_ID) {
				formula_dbc_mcu_motor_position_info_t motor_position;
				formula_dbc_mcu_motor_position_info_unpack(&motor_position, message.data, message.len);
				data.motorrpm = motor_position.d2_motor_speed;
				Serial.printf("New motor speed = %d RPM\n", data.motorrpm);
			}
		}

		DisplayManager::update(data);
	}
}
