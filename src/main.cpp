#include <Arduino.h>
#include "controller/dash-controller.h"
#include "data-processor/data-processor.h"

#define START_BUTTON_PIN 4
#define START_SOUND_SIGNAL 18
#define DELAY 100

DASH_CONTROLLER controller;

void setup()
{

  //Serial.print("startingg");
  pinMode(START_BUTTON_PIN, INPUT);
  pinMode(START_SOUND_SIGNAL, OUTPUT);
  //inMode(19, OUTPUT);
  //pinMode(16, OUTPUT); //for testing
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10000);
  controller.initialize();
  //digitalWrite(19, HIGH);
  //digitalWrite(16, HIGH); //for testing RTDS
  digitalWrite(START_SOUND_SIGNAL, HIGH);
}

void loop()
{
  //Serial.print("message recieved = ");
  //Serial.println(controller.driveReady);
  //Serial.print("Button = ");
  //Serial.println(digitalRead(START_BUTTON_PIN));
  if (controller.driveReady && (digitalRead(START_BUTTON_PIN) == HIGH))
  {
    digitalWrite(START_SOUND_SIGNAL, LOW);
  }
  //digitalWrite(19, HIGH);
  controller.updateModel();
  delay(DELAY);
  //Serial.println("loop");
  //digitalWrite(19, LOW);
  controller.updateView();
  // Serial.println("6");
  delay(DELAY);
}