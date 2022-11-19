#include <Arduino.h>

#include "data-manager.h"
#include "display-manager.h"

void setup()
{
	Serial.begin(9600);
	Serial.printf("Starting!\n");

	DataManager::init();
}

void loop()
{
	DataManager::update();

	delay(100);
}
