#pragma once

#include <cstdint>
#include "Adafruit_RA8875.h"

#define TFT_SCREEN_WIDTH 480
#define TFT_SCREEN_HEIGHT 272
#define TFT_SCREEN_PIXELS (TFT_SCREEN_WIDTH * TFT_SCREEN_HEIGHT)

class TFTManager {
private:
	Adafruit_RA8875 driver;
	uint16_t* buf_front;
	uint16_t* buf_rear;

	void initScreen();
	void swapBuffers();
	void bufferToTFT();

public:
	TFTManager();
	~TFTManager();
	uint16_t* getDrawBuffer();
	void clearDrawBuffer(uint16_t color);
	void swapDisplay();
};
