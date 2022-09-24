#include <Arduino.h>

#include "tft-manager.h"
#include "../lib/lvgl/lvgl.h"

TFTManager man;

void setup()
{
	Serial.begin(9600);
	Serial.print("Starting!\n");
}

uint16_t color = 0x0000;

void loop()
{
	Serial.print("Update\n");
	man.fillBuffer(color);
	color += 0x1111;
	man.drawBuffer();
}
