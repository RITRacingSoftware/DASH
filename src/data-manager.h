#pragma once

#include <cstdint>

#define NUM_BMS_CELLS 90

namespace DataManager {
	typedef struct car_data_s {
		uint16_t mcu_motorrpm;
		uint8_t bms_soc;
		int32_t bms_buscurrent;
		int32_t bms_maxcurrent;
		struct {
			uint8_t bms_voltages_mux;
			uint16_t bms_voltages_cell[NUM_BMS_CELLS];
		} bms_cellvoltages_struct;
		uint16_t bms_cellvoltages_min;
		uint16_t bms_cellvoltages_max;
	} car_data_t;

	void init();
	void update();
}
