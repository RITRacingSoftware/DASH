#include <Arduino.h>
#include "controller/dash-controller.h"
#include "data-processor/data-processor.h"

DASH_CONTROLLER controller;

void setup() {
  Serial.print("startingg");
  pinMode(19, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  controller.initialize();
  digitalWrite(19, HIGH);
}

void loop() {
  digitalWrite(19, HIGH);
  controller.updateModel();
  delay(80);
  Serial.println("loop");
  digitalWrite(19, LOW);
  controller.updateView();
  // Serial.println("6");
  delay(80);
}