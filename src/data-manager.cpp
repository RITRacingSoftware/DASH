#include "data-manager.h"

#include <cstdint>
#include <cmath>
#include <Arduino.h>


#include "can-manager.h"
#include "display-manager.h"
#include "formula_main_dbc.h"

namespace DataManager {
	car_data_t data;

	// Calculated using GNU units to prevent precision loss
	// > units "1/minute * 11/45 * 16in * pi" "mph"
	const double RPM_TO_MPH = .011635528;

	void init() {
		Serial.printf("Initializing DataManager\n");
		DisplayManager::init();
		CANManager::init();

		data.bms_maxcurrent = -100000;
		//Assume we have a fault until we certianly don't
		data.bms_faultvector = 0x8000;
		data.lv_voltage = 0.0;
		Serial.printf("Initialized DataManager\n");
	}

	void update() {
		CANManager::can_message_t message;
		while(CANManager::hasMessage()) {
			message = CANManager::getMessage();
			Serial.printf("Got CAN message, ID=0x%04x\n", message.id);
			switch(message.id) {
				case FORMULA_MAIN_DBC_MCU_MOTOR_POSITION_INFO_FRAME_ID: {
					formula_main_dbc_mcu_motor_position_info_t motor_position;
					formula_main_dbc_mcu_motor_position_info_unpack(&motor_position, message.data, message.len);
					data.mcu_motorrpm = motor_position.d2_motor_speed;
					float wheelmph = data.mcu_motorrpm * RPM_TO_MPH;
					data.mcu_wheelspeed = wheelmph;
					break; }
				case FORMULA_MAIN_DBC_BMS_STATUS_FRAME_ID: {
					formula_main_dbc_bms_status_t bms_status;
					formula_main_dbc_bms_status_unpack(&bms_status, message.data, message.len);
					data.bms_soc = bms_status.bms_status_soc;
					data.bms_packvoltage = bms_status.bms_status_pack_voltage * 0.1;
					break; }
				case FORMULA_MAIN_DBC_BMS_CELL_OVERVIEW_FRAME_ID: {
					formula_main_dbc_bms_cell_overview_t cells;
					formula_main_dbc_bms_cell_overview_unpack(&cells, message.data, message.len);
					data.bms_cellvoltages_min = formula_main_dbc_bms_cell_overview_bms_overview_volt_min_decode(cells.bms_overview_volt_min);
					data.bms_cellvoltages_max = formula_main_dbc_bms_cell_overview_bms_overview_volt_max_decode(cells.bms_overview_volt_max);
					data.bms_maxtemp = formula_main_dbc_bms_cell_overview_bms_overview_temp_max_decode(cells.bms_overview_temp_max);
					break;
				}
				case FORMULA_MAIN_DBC_BMS_CURRENT_FRAME_ID: {
					formula_main_dbc_bms_current_t current;
					formula_main_dbc_bms_current_unpack(&current, message.data, message.len);
					data.bms_buscurrent = current.bms_inst_current_filt * 0.001;
					if(data.bms_buscurrent > data.bms_maxcurrent) {
						data.bms_maxcurrent = data.bms_buscurrent;
					}
					break; }
				case FORMULA_MAIN_DBC_VC_STATUS_FRAME_ID: {
					formula_main_dbc_vc_status_t vcstatus;
					formula_main_dbc_vc_status_unpack(&vcstatus, message.data, message.len);
					data.vc_status = vcstatus.vc_status_vehicle_state;
					data.mcu_status = vcstatus.vc_status_mc_state;
					break; }
				case FORMULA_MAIN_DBC_VC_FAULT_VECTOR_FRAME_ID: {
					uint8_t mask = 0xf;
					data.vc_faultvector = (data.vc_faultvector & ~mask) |
												(message.data[0] & mask);
					// Masked to keep other bits
					break; }
				case FORMULA_MAIN_DBC_VC_HARD_FAULT_INDICATOR_FRAME_ID: {
					formula_main_dbc_vc_hard_fault_indicator_t fault;
					formula_main_dbc_vc_hard_fault_indicator_unpack(&fault, message.data, message.len);
					uint8_t faulted = fault.vc_hard_fault_indicator_task != 0;
					uint8_t mask = 0x10;
					data.vc_faultvector = (data.vc_faultvector & ~mask) |
												((faulted << 4) & mask);
					Serial.printf("VC hardfault = %d\n", fault.vc_hard_fault_indicator_task);
					Serial.printf("VC fault vec = 0x%02x\n", data.vc_faultvector);
					// Masked to keep other bits
					break; }
				case FORMULA_MAIN_DBC_BMS_FAULT_VECTOR_FRAME_ID: {
					// This is not as DBC-agnostic as I would like,
					//		but it's the best way I see to do it.
					data.bms_faultvector = message.data[0] + (message.data[1] << 8);
					break; }
				case FORMULA_MAIN_DBC_BMS_LIMP_MODE_FRAME_ID: {
					formula_main_dbc_bms_limp_mode_t limpmode;
					formula_main_dbc_bms_limp_mode_unpack(&limpmode, message.data, message.len);
					uint8_t volt_limp = limpmode.bms_limp_mode_voltage_limp_level;
					uint8_t temp_limp = limpmode.bms_limp_mode_temp_limp_level;
					uint8_t overall_limp;

					if(volt_limp > temp_limp) {
						data.bms_limplevel = volt_limp;
					}
					else {
						data.bms_limplevel = temp_limp;
					}
					break;
				}
				case FORMULA_MAIN_DBC_MCU_INTERNAL_STATES_FRAME_ID: {
					formula_main_dbc_mcu_internal_states_t states;
					formula_main_dbc_mcu_internal_states_unpack(&states, message.data, message.len);
					data.mcu_vsm_state = states.d1_vsm_state;
					break;
				}
				case FORMULA_MAIN_DBC_C70_TIRE_TEMPS_FRAME_ID: {
					formula_main_dbc_c70_tire_temps_t temps;
					formula_main_dbc_c70_tire_temps_unpack(&temps, message.data, message.len);
					data.tiretemp_fl = formula_main_dbc_c70_tire_temps_tire_temp_fl_max_decode(temps.tire_temp_fl_max);
					data.tiretemp_fr = formula_main_dbc_c70_tire_temps_tire_temp_fr_max_decode(temps.tire_temp_fr_max);
					data.tiretemp_rl = formula_main_dbc_c70_tire_temps_tire_temp_rl_max_decode(temps.tire_temp_rl_max);
					data.tiretemp_rr = formula_main_dbc_c70_tire_temps_tire_temp_rr_max_decode(temps.tire_temp_rr_max);
					break;
				}
				case FORMULA_MAIN_DBC_C70_ROTOR_TEMPS_FRAME_ID: {
					formula_main_dbc_c70_rotor_temps_t temps;
					formula_main_dbc_c70_rotor_temps_unpack(&temps, message.data, message.len);
					float rotortemp_fl = formula_main_dbc_c70_rotor_temps_rotor_temp_fl_max_decode(temps.rotor_temp_fl_max);
					float rotortemp_fr = formula_main_dbc_c70_rotor_temps_rotor_temp_fr_max_decode(temps.rotor_temp_fr_max);
					float rotortemp_rl = formula_main_dbc_c70_rotor_temps_rotor_temp_rl_max_decode(temps.rotor_temp_rl_max);
					float rotortemp_rr = formula_main_dbc_c70_rotor_temps_rotor_temp_rr_max_decode(temps.rotor_temp_rr_max);
					float rotortemp_max = fmax(fmax(rotortemp_fl, rotortemp_fr), fmax(rotortemp_rl, rotortemp_rr));
					data.rotortemp = rotortemp_max;
					break;
				}
				case FORMULA_MAIN_DBC_PBX_STATUS_FRAME_ID: {
					formula_main_dbc_pbx_status_t status;
					formula_main_dbc_pbx_status_unpack(&status, message.data, message.len);
					data.lv_voltage = formula_main_dbc_pbx_status_pbx_lv_voltage_decode(status.pbx_lv_voltage);
					break;
				}
			}
		}

		DisplayManager::update(data);
	}
}
