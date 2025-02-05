#include <Arduino.h>

// #include "data-manager.h"
// #include "display-manager.h"

// void setup()
// {
// 	Serial.begin(9600);
// 	Serial.printf("Starting!\n");

// 	DataManager::init();
// }

// void loop()
// {
// 	DataManager::update();
// 	lv_timer_handler();
// 	delay(100);
// }

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(13, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  digitalWrite(13, HIGH);
  // wait for a second
  delay(1000);
  // turn the LED off by making the voltage LOW
  digitalWrite(13, LOW);
   // wait for a second
  delay(1000);
}
