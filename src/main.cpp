#include "data-manager.h"
#include "display-manager.h"

#ifdef DASH_TESTING
#include "testing_arduino.h"
#else
#include "Arduino.h"
#endif

#if defined(__cplusplus) && defined(DASH_TESTING)
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void setup() {
	Serial.begin(9600);
	Serial.printf("Starting!\n");

	DataManager::init();
}

EXTERNC void loop() {
	DataManager::update();
	delay(10);
}
