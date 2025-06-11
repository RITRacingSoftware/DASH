#pragma once

#include "config.h"

#include <cstdint>
#ifndef DASH_TESTING
#include "Adafruit_RA8875.h"
#endif


namespace TFTManager {
	void init();
	void fillScreen(uint16_t color);
	void drawTexturedRect(int x1, int x2, int y1, int y2, uint16_t* pixels);
}
