#pragma once

#include "data-manager.h"
#include "display-manager.h"
#include "lvgl.h"

namespace ScreenDebug {
	lv_obj_t* init(DisplayManager::styles_t* styles);
	void update(DataManager::car_data_t data);
}
