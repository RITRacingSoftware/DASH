#include <Arduino.h>

#include "tft-manager.h"

TFTManager man;

void setup()
{
	Serial.begin(9600);
	Serial.print("Starting!\n");
}

void loop()
{
	Serial.print("Update\n");
	man.swapDisplay();
}
