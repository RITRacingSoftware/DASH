#pragma once

#include "data-manager.h"
#include "display-manager.h"
#include "lvgl.h"
#include "tft-manager.h"

namespace ScreenEndurance {
	lv_obj_t* init(DisplayManager::endurance_styles_t* en);
	void update(DataManager::car_data_t data);
}

