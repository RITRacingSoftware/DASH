#pragma once

#include "lvgl.h"
#include "data-manager.h"

namespace DisplayManager {
	typedef struct styles_s {
		lv_style_t style;
		lv_style_t bar_style;
		lv_style_t barind_style;
		lv_style_t fault_style;
		lv_style_t container_style;
	} styles_t;

	void init();
	void update(DataManager::car_data_t data);
	void switchScreens(int new_screen);
}
