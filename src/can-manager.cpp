#include "can-manager.h"

#include <cstdint>

#include "../lib/FlexCAN_T4/FlexCAN_T4.h"

#define CAN_BAUD_RATE 1000000
#define MC_FAULT_CODES_ID 0x0AB
#define MOTOR_POSITION_ID 0x0A5
#define LAP_COMPLETE_ID 0x000
#define WATER_TEMP_ID 0x000
#define READY_TO_DRIVE_ID 0x0AA
#define BMS_FAULTS_ID 0x2BC
#define BMS_CHARGE_REQUEST_ID 0x0x1806e5f4u
#define BMS_CURRENT_ID 0x384
#define BMS_STATUS_ID 0x258
#define BMS_FAULT_ALERT_ID 0x2bd
#define BMS_VOLTAGES_ID 0x2be
#define BMS_THERMISTOR_VOLTAGES_ID 0x2bf
#define BMS_TEMPERATURES_ID 0x2c0
#define BMS_DRAIN_STATUS_A_ID 0x320
#define BMS_DRAIN_STATUS_B_ID 0x321

namespace CANManager {
	static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can;

	void initCAN() {
		can.begin();
	    can.setBaudRate(CAN_BAUD_RATE);
	    can.enableFIFO();
	    can.enableMBInterrupt(FIFO);
	    can.setFIFOFilter(REJECT_ALL);
	    can.setFIFOFilter(0, MC_FAULT_CODES_ID, MOTOR_POSITION_ID, STD);
	    can.setFIFOFilter(1, READY_TO_DRIVE_ID, BMS_FAULTS_ID, STD);
	    can.setFIFOFilter(2, BMS_CURRENT_ID, BMS_STATUS_ID, STD);
	    can.setFIFOFilter(3, BMS_VOLTAGES_ID, STD);
	}

	bool readMessage(can_message_t* out) {
		CAN_message_t message;
		if (can.read(message))
		{
			out->id = message.id;
			for (int i = 0; i <= 7; i++) // n to hl if need to change endianness
			{
					out->data[i] = message.buf[i]; // Set the data
			}
			return true;
		}
		return false;
	}

	void init() {
		initCAN();
	}

	void update() {

	}
}
