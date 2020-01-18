#include <Arduino.h>
#include "controller/tft-controller.h"
#include "data-processor/data-processor.h"

DATA_PROCESSOR dataProcessor;
TFT_CONTROLLER controller(&dataProcessor);

void setup() {
  pinMode(13, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  controller.initialize();
}

void loop() {
  Serial.println("1");
  digitalWrite(13, HIGH);
  controller.updateModel();
  delay(80);
  digitalWrite(13, LOW);
  controller.updateView();
  delay(80);
}