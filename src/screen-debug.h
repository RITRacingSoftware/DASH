#include "lvgl.h"
#pragma once

#include "data-manager.h"
#include "display-manager.h"

namespace ScreenDebug {
	lv_obj_t* init(DisplayManager::styles_t* styles);
	void update(DataManager::car_data_t data);
}
