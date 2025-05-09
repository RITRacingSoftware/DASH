#pragma once

#include "lvgl.h"
#include "data-manager.h"

namespace DisplayManager {
	typedef struct debug_styles {
		lv_style_t mainTextSmall;
		lv_style_t mainTextLarge;
		lv_style_t faultTimecode;
		lv_style_t faultText;
	} debug_styles_t;

	typedef struct drive_styles {
		lv_style_t tempText;
		lv_style_t middleText;
		lv_style_t bmsText;

	} drive_styles_t;



	void init();
	void update(DataManager::car_data_t data);
	void switchScreens(int new_screen);
}
