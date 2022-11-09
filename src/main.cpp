#include <Arduino.h>

#include "data-manager.h"
#include "display-manager.h"

void setup()
{
	Serial.begin(9600);
	Serial.printf("Starting!\n");

	DataManager::init();
}

unsigned long lastswitchtime = 0;
bool lastscreen = 0;

void loop()
{
	DataManager::update();

	unsigned long time = millis();
	if(time - lastswitchtime > 5000) {
		lastswitchtime = time;
		lastscreen = !lastscreen;
		DisplayManager::switchScreens(lastscreen);
	}

	delay(100);
}
