#pragma once

#include "../lib/lvgl/lvgl.h"
#include "data-manager.h"

namespace DisplayManager {
	void init();
	void update(DataManager::car_data_t data);
}
