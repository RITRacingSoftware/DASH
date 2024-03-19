//#pragma once

#include <cstdint>
#include "Adafruit_RA8875.h"


//#define LCD_res_480x272  //old LCD value
#define LCD_res_800x480  //new FOCUSlcd value

#define RA8875_PWM_BRIGHTNESS 255 //LED birghtness 0~255

#ifdef LCD_res_480x272
	#define TFT_SCREEN_WIDTH 480
	#define TFT_SCREEN_HEIGHT 272

	#define RA8875_init_res RA8875_480x272 //remember i manually commented that out in tft-manage
#endif

#ifdef LCD_res_800x480
#define TFT_SCREEN_WIDTH 800
#define TFT_SCREEN_HEIGHT 480
#define RA8875_init_res RA8875_800x480
#endif

#define TFT_SCREEN_PIXELS (TFT_SCREEN_WIDTH * TFT_SCREEN_HEIGHT)

namespace TFTManager {
	void init();
	void fillScreen(uint16_t color);
	void drawTexturedRect(int x1, int x2, int y1, int y2, uint16_t* pixels);
}
