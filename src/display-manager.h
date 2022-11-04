#pragma once

#include "lvgl.h"
#include "data-manager.h"

namespace DisplayManager {
	typedef struct styles_s {
		lv_style_t style;
		lv_style_t barstyle;
		lv_style_t barindstyle;
		lv_style_t faultstyle;
	} styles_t;

	void init();
	void update(DataManager::car_data_t data);
}
