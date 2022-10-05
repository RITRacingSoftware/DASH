#include <Arduino.h>

#include "data-manager.h"

void setup()
{
	Serial.begin(9600);
	Serial.printf("Starting!\n");

	DataManager::init();
}

void loop()
{
	Serial.print("Update\n");

	DataManager::update();
	delay(5);
}
