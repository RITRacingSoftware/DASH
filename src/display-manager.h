#pragma once

#include "lvgl.h"
#include "data-manager.h"

namespace DisplayManager {
	void init();
	void update(DataManager::car_data_t data);
}
