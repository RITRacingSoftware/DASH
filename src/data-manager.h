#pragma once

#include <cstdint>

#define NUM_BMS_CELLS 90

namespace DataManager {
	typedef struct car_data_s {
		int16_t mcu_motorrpm;
		float mcu_wheelspeed;

		uint8_t vc_status;
		uint8_t vc_faultvector;
		uint8_t mcu_status;
		uint16_t bms_faultvector;

		uint8_t bms_soc;
		float bms_buscurrent;
		float bms_maxcurrent;

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
	} car_data_t;

	void init();
	void update();
}
