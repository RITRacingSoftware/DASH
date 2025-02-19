#pragma once

#include <cstdint>

#define NUM_BMS_CELLS 90

namespace DataManager {
	typedef struct car_data_s {
		// float vel_n;
		// float vel_e;
		float vel;
		// float mcu_wheelspeed; //TODO: Old placeholder, switch to mcu_carspeed
		float mcu_motorrpm; //TODO: Old placeholder, split into one for each motor
		float mcu_carspeed;

		uint8_t vc_status;
		uint8_t vc_faultvector;
		// Add new stauses for each motor
		uint8_t mcu_status; //TODO: Split in 4 or remove
		uint8_t mcu_vsm_state; //TODO: Split in 4 or remove
		
		uint16_t bms_faultvector;
		uint8_t bms_soc;
		float bms_buscurrent;
		float bms_maxcurrent;
		float bms_maxtemp;

		float lv_voltage;
		float goPedal_A;
		float goPedal_B;
		float goPedal_AVG;
		float nogo_pressure;

		uint8_t bms_limplevel;

		// This mirrors the formula_dbc_bms_voltages_t struct
		// in the auto-generated DBC header file.
		// Cells are randomly ordered, but the values are good.
		struct {
			uint8_t bms_voltages_mux;
			uint16_t bms_voltages_cell[NUM_BMS_CELLS];
		} bms_cellvoltages_struct;
		float bms_cellvoltages_min;
		float bms_cellvoltages_max;
		float bms_packvoltage;

		float tiretemp_fl;
		float tiretemp_fr;
		float tiretemp_rl;
		float tiretemp_rr;

		float rotortemp;
	} car_data_t;

	void init();
	void update();
}
