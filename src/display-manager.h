#pragma once

#include "../lib/lvgl/lvgl.h"
#include "tft-manager.h"

namespace DisplayManager {
	void init();
	void destroy();
	lv_style_t* getStyle();
}
