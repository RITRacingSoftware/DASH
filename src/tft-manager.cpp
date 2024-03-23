#include "tft-manager.h"

#include <cstdlib>
#include <Arduino.h>

#define PIN_RA8875_CS 10
#define PIN_RA8875_RESET 9

namespace TFTManager {
	Adafruit_RA8875* driver;

	void initScreen() {
		driver->begin(RA8875_800x480);
		driver->displayOn(true);
		driver->GPIOX(true); // Enable TFT - display enable tied to GPIOX
		driver->PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
		driver->PWM1out(255);

		driver->graphicsMode();
	}

	void init(){
		Serial.printf("Initializing TFTManager\n");

		driver = new Adafruit_RA8875(PIN_RA8875_CS, PIN_RA8875_RESET);
		initScreen();

		fillScreen(RA8875_BLACK);
		Serial.printf("Initialized TFTManager\n");
	}

	void fillScreen(uint16_t color) {
		driver->fillScreen(color);
	}

	void drawTexturedRect(int x1, int x2, int y1, int y2, uint16_t* pixels) {
		for(int y = y1; y <= y2; y++) {
			uint16_t width = x2-x1+1;
			driver->drawPixels(pixels, width, x1, y);
			pixels += width;
		}
	}
}
