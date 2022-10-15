#include "can-manager.h"

#include <cstdint>
#include <Arduino.h>

#include "../lib/FlexCAN_T4/FlexCAN_T4.h"
#include "../lib/Formula-DBC/formula_dbc.h"

#define CAN_BAUD_RATE 1000000
#define READY_TO_DRIVE_ID 0x0AA

namespace CANManager {
	static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can;

	void initCAN() {
		can.begin();
		can.setBaudRate(CAN_BAUD_RATE);
		can.enableFIFO();
		can.enableMBInterrupt(FIFO);
		can.setFIFOFilter(REJECT_ALL);
		can.setFIFOFilter(0, FORMULA_DBC_MCU_FAULT_CODES_FRAME_ID, FORMULA_DBC_MCU_MOTOR_POSITION_INFO_FRAME_ID, STD);
		//can.setFIFOFilter(1, READY_TO_DRIVE_ID, FORMULA_DBC_BMS_FAULT_VECTOR_FRAME_ID, STD);
		//can.setFIFOFilter(2, FORMULA_DBC_BMS_CURRENT_FRAME_ID, FORMULA_DBC_BMS_STATUS_FRAME_ID, STD);
		//can.setFIFOFilter(3, FORMULA_DBC_BMS_VOLTAGES_FRAME_ID, STD);
	}

	bool readMessage(can_message_t* out) {
		CAN_message_t message;
		if (can.read(message))
		{
			out->id = message.id;
			out->len = message.len;
			for (int i = 0; i <= 7; i++) // n to hl if need to change endianness
			{
					out->data[i] = message.buf[i]; // Set the data
			}
			return true;
		}
		return false;
	}

	void init() {
		Serial.printf("Initializing CANManager\n");
		initCAN();
		Serial.printf("Initialized CANManager\n");
	}
}
