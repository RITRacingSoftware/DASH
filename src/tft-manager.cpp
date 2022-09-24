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
	driver.fillScreen(RA8875_RED);
	driver.textMode(); // Temporary
}

TFTManager::TFTManager() : driver(Adafruit_RA8875(PIN_RA8875_CS, PIN_RA8875_RESET)){

	Serial.printf("Initializing TFT Manager\n");

	Serial.printf("Initializing Display\n");

	initScreen();

	buf_front = (uint16_t*) malloc(TFT_SCREEN_PIXELS * sizeof(uint16_t));
	buf_rear = (uint16_t*) malloc(TFT_SCREEN_PIXELS * sizeof(uint16_t));

	for(int i = 0; i < TFT_SCREEN_PIXELS; i++) {
		buf_front[i] = 0xffff;
		//buf_rear[i] = 0x7777;
	}

	Serial.printf("Initialized Framebuffers\n");

	uint16_t color = 0;

	while(true) {
		for(int i = 0; i < TFT_SCREEN_PIXELS; i++) {
			buf_front[i] = color;
			//buf_rear[i] = 0x7777;
		}
		color += 0x1111;

		Serial.printf("White\n");
		driver.graphicsMode();
		driver.drawPixels(buf_front, TFT_SCREEN_PIXELS, 0, 0);
		//Serial.printf("Grey\n");
		//driver.drawPixels(buf_rear, TFT_SCREEN_PIXELS, 0, 0);
	}

	//bufferToTFT();
}

TFTManager::~TFTManager() {
	//delete driver;
}

uint16_t* TFTManager::getDrawBuffer() {
	//return buf_rear;
	return NULL;
}

void TFTManager::clearDrawBuffer(uint16_t color) {
	for(int i = 0; i < TFT_SCREEN_PIXELS; i++) {
	//	buf_rear[i] = color;
	}
}

void TFTManager::swapBuffers() {
	/*uint16_t* temp = buf_front;
	buf_front = buf_rear;
	buf_rear = temp;*/
}

void TFTManager::bufferToTFT() {
	//driver.graphicsMode();
	//driver.drawPixels(buf_front, TFT_SCREEN_PIXELS, 0, 0);
}

void TFTManager::swapDisplay() {
	swapBuffers();
	bufferToTFT();
}
