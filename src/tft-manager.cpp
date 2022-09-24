#include "tft-manager.h"

#include <cstdlib>
#include <Arduino.h>

#define PIN_RA8875_CS 10
#define PIN_RA8875_RESET 9

void TFTManager::initScreen() {
	driver.begin(RA8875_480x272);

	driver.displayOn(true);
	driver.GPIOX(true); // Enable TFT - display enable tied to GPIOX
	driver.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
	driver.PWM1out(255);

	driver.graphicsMode();
	driver.fillScreen(RA8875_BLACK);
}

TFTManager::TFTManager() : driver(Adafruit_RA8875(PIN_RA8875_CS, PIN_RA8875_RESET)){

	Serial.printf("Initializing TFT Manager\n");

	Serial.printf("Initializing Display\n");
	initScreen();

	// Initialize Frame Buffer
	framebuf = (uint16_t*) malloc(TFT_SCREEN_PIXELS * sizeof(uint16_t));
	fillBuffer(RA8875_BLACK);
	Serial.printf("Initialized Framebuffer\n");

}

TFTManager::~TFTManager() {

}

uint16_t* TFTManager::getDrawBuffer() {
	//return buf_rear;
	return NULL;
}

void TFTManager::fillBuffer(uint16_t color) {
	for(int i = 0; i < TFT_SCREEN_PIXELS; i++) {
		framebuf[i] = color;
	}
}

void TFTManager::drawBuffer() {
	driver.graphicsMode();
	driver.drawPixels(framebuf, TFT_SCREEN_PIXELS, 0, 0);
}
