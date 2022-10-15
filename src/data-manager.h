#pragma once

#include <cstdint>

namespace DataManager {
	typedef struct car_data_s {
		uint16_t motorrpm;
		uint8_t chargepercent;
		int32_t buscurrent;
	} car_data_t;

	void init();
	void update();
}
