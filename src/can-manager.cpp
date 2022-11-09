#include "can-manager.h"

#include <cstdint>
#include <queue>
#include <Arduino.h>

#include "FlexCAN_T4.h"
#include "formula_dbc.h"

#define CAN_BAUD_RATE 1000000
#define READY_TO_DRIVE_ID 0x0AA

namespace CANManager {
	static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can;
	static std::queue<can_message_t> message_queue;

	void CAN_ISR(const CAN_message_t &msg) {
		// This method MUST take less than 1ms to run, else it will be
		//		interrupted by the next CAN message

		can_message_t out;

		out.id = msg.id;
		out.len = msg.len;
		for (int i = 0; i <= 7; i++) // n to hl if need to change endianness
		{
			out.data[i] = msg.buf[i]; // Set the data
		}

		message_queue.push(out);
	}

	void initCAN() {
		// Initial setup
		can.begin();
		can.setBaudRate(CAN_BAUD_RATE);

		// Set up interrupt
		can.setMaxMB(16);
		can.enableFIFO();
		can.enableFIFOInterrupt();
		can.onReceive(CAN_ISR);

		// Filtering
		// TODO: Untested while using FIFO interrupt
		/*can.setFIFOFilter(REJECT_ALL);
		can.setFIFOFilter(0, FORMULA_DBC_MCU_FAULT_CODES_FRAME_ID, FORMULA_DBC_MCU_MOTOR_POSITION_INFO_FRAME_ID, STD);
		can.setFIFOFilter(1, READY_TO_DRIVE_ID, FORMULA_DBC_BMS_FAULT_VECTOR_FRAME_ID, STD);
		can.setFIFOFilter(2, FORMULA_DBC_BMS_CURRENT_FRAME_ID, FORMULA_DBC_BMS_STATUS_FRAME_ID, STD);
		can.setFIFOFilter(3, FORMULA_DBC_BMS_VOLTAGES_FRAME_ID, STD);*/
	}

	bool hasMessage() {
		return !message_queue.empty();
	}

	can_message_t getMessage() {
		can_message_t message = message_queue.front();
		message_queue.pop();
		return message;
	}

	void init() {
		Serial.printf("Initializing CANManager\n");
		initCAN();
		Serial.printf("Initialized CANManager\n");
	}
}
