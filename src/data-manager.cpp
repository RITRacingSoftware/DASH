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

		data.bms_maxcurrent = -100000;

		Serial.printf("Initialized DataManager\n");
	}

	void update() {
		CANManager::can_message_t message;
		while(CANManager::hasMessage()) {
			message = CANManager::getMessage();
			Serial.printf("Got CAN message, ID=0x%04x\n", message.id);

			if(message.id == FORMULA_DBC_MCU_MOTOR_POSITION_INFO_FRAME_ID) {
				formula_dbc_mcu_motor_position_info_t motor_position;
				formula_dbc_mcu_motor_position_info_unpack(&motor_position, message.data, message.len);
				data.mcu_motorrpm = motor_position.d2_motor_speed;
				Serial.printf("New motor speed = %d RPM\n", data.mcu_motorrpm);
			}

			if(message.id == FORMULA_DBC_BMS_STATUS_FRAME_ID) {
				formula_dbc_bms_status_t bms_status;
				formula_dbc_bms_status_unpack(&bms_status, message.data, message.len);
				data.bms_soc = bms_status.bms_status_soc;
				Serial.printf("New battery SOC = %d %%\n", data.bms_soc);
			}

			if(message.id == FORMULA_DBC_BMS_CURRENT_FRAME_ID) {
				formula_dbc_bms_current_t current;
				formula_dbc_bms_current_unpack(&current, message.data, message.len);
				data.bms_buscurrent = current.bms_inst_current_filt;
				Serial.printf("New bus current = %3.3f A\n", data.bms_buscurrent * 0.001);
				if(data.bms_buscurrent > data.bms_maxcurrent) {
					data.bms_maxcurrent = data.bms_buscurrent;
				}
			}

			if(message.id == FORMULA_DBC_BMS_VOLTAGES_FRAME_ID) {
				formula_dbc_bms_voltages_t* voltages = (formula_dbc_bms_voltages_t*)
					&(data.bms_cellvoltages_struct);
				formula_dbc_bms_voltages_unpack(voltages, message.data, message.len);

				uint16_t min = 65535;
				uint16_t max = 0;

				for(int i = 0; i < NUM_BMS_CELLS; i++) {
					uint16_t cell = data.bms_cellvoltages_struct.bms_voltages_cell[i];
					if(cell < min) {
						min = cell;
					}
					if(cell > max) {
						max = cell;
					}
				}

				data.bms_cellvoltages_min = min;
				data.bms_cellvoltages_max = max;
			}

			if(message.id == FORMULA_DBC_BMS_FAULT_VECTOR_FRAME_ID) {
				formula_dbc_bms_fault_vector_t faults;
				formula_dbc_bms_fault_vector_unpack(&faults, message.data, message.len);
				data.bms_faultvector[0] = faults.bms_fault_vector_slave_comm_cells;
				data.bms_faultvector[1] = faults.bms_fault_vector_slave_comm_temps;
				data.bms_faultvector[2] = faults.bms_fault_vector_slave_comm_drain_request;
				data.bms_faultvector[3] = faults.bms_fault_vector_current_sensor_comm;
				data.bms_faultvector[4] = faults.bms_fault_vector_over_current;
				data.bms_faultvector[5] = faults.bms_fault_vector_cell_voltage_irrational;
				data.bms_faultvector[6] = faults.bms_fault_vector_cell_voltage_diff;
				data.bms_faultvector[7] = faults.bms_fault_vector_out_of_juice;
				data.bms_faultvector[8] = faults.bms_fault_vector_temperature_irrational;
				data.bms_faultvector[9] = faults.bms_fault_vector_over_temperature;
				data.bms_faultvector[10] = faults.bms_fault_vector_drain_failure;
				data.bms_faultnum = 0;
				Serial.printf("BMS faultvector[0] = 0x%02x\n", message.data[0]);
				for(int i = 0; i < 11; i++) {
					if(data.bms_faultvector[i]) {
						Serial.printf("BMS fault #%d\n", i);
						data.bms_faultnum++;
					}
				}
				Serial.printf("BMS fault num = %d\n", data.bms_faultnum);
			}
		}

		DisplayManager::update(data);
	}
}
