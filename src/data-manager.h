#pragma once

#include <cstdint>

#define NUM_BMS_CELLS 90

namespace DataManager {
	typedef struct car_data_s {
		uint16_t mcu_motorrpm;
		uint8_t bms_soc;
		int32_t bms_buscurrent;
		uint16_t bms_voltages_cells[NUM_BMS_CELLS];
		uint16_t bms_cellvoltages_min;
		uint16_t bms_cellvoltages_max;
	} car_data_t;

	void init();
	void update();
}
