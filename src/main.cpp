#include <Arduino.h>
#include "controller/tft-controller.h"

TFT_CONTROLLER controller;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  controller.initialize();
}

void loop()
{
  controller.updateModel();
  delay(80);
  controller.updateView();
  delay(80);
}