#pragma once

#include <cstdint>
#include "Adafruit_RA8875.h"

#define TFT_SCREEN_WIDTH 480
#define TFT_SCREEN_HEIGHT 272
#define TFT_SCREEN_PIXELS (TFT_SCREEN_WIDTH * TFT_SCREEN_HEIGHT)

namespace TFTManager {
	void init();
	void fillScreen(uint16_t color);
	void drawTexturedRect(int x1, int x2, int y1, int y2, uint16_t* pixels);
}
