#include "can-manager.h"

#include <cstdint>
#include <queue>

#ifdef DASH_TESTING
#include "testing_arduino.h"
#include "read_cf.h"
#else
#include <Arduino.h>
#include "FlexCAN_T4.h"
#endif

#include "c_files/main_dbc.h"

#define CAN_BAUD_RATE 1000000
#define READY_TO_DRIVE_ID 0x0AA //ask solomon on thursday if this has changed ....

namespace CANManager {
#ifdef DASH_TESTING
    static cf_file file;
    void initCAN() {
        cf_open(&file, "../20250611a.cf");
    }
    bool hasMessage() {
        return cf_available(&file);
    }
    can_message_t getMessage() {
        uint8_t buffer[16];
        can_message_t message;
        cf_read_message(&file, buffer, 16);
        message.id = *((uint32_t*)(buffer+4));
        message.len = buffer[1]&0x7f;
        for (int i=0; i < 8; i++) message.data[i] = buffer[i+8];
        return message;
    }
#else
	static std::queue<can_message_t> message_queue;
	static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can;

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
		// TODO: Determine if this is at all still useful
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
#endif
	void init() {
		Serial.printf("Initializing CANManager\n");
		initCAN();
		Serial.printf("Initialized CANManager\n");
	}
}
