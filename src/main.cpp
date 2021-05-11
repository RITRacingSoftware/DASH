#include <Arduino.h>
#include "controller/dash-controller.h"
#include "data-processor/data-processor.h"

#define START_BUTTON_PIN 21
#define START_SOUND_SIGNAL 20
#define DELAY 100

DASH_CONTROLLER controller;
int lastStartButtonState;

void setup()
{

  //Serial.print("startingg");
  pinMode(START_BUTTON_PIN, INPUT);
  pinMode(START_SOUND_SIGNAL, OUTPUT);
  //inMode(19, OUTPUT);
  //pinMode(16, OUTPUT); //for testing
  // put your setup code here, to run once:
  Serial.begin(9600);
  //delay(7000);
  //Serial.printf("startting\n");
  controller.initialize();
  //digitalWrite(19, HIGH);
  //digitalWrite(16, HIGH); //for testing RTDS

  //When this is low, pin 4 on connector is 5V, when the harness schematic has RTD trigger as pin 3
  digitalWrite(START_SOUND_SIGNAL, LOW);
}

void loop()
{
  //Serial.print("message recieved = ");
  //Serial.println(controller.driveReady);
  //Serial.print("Button = ");
  //Serial.println(digitalRead(START_BUTTON_PIN));

  //if (controller.driveReady && (digitalRead(START_BUTTON_PIN) == HIGH))

  //CHANGE BACK
  if (digitalRead(START_BUTTON_PIN) == HIGH)
  {
    if(lastStartButtonState == HIGH){
      digitalWrite(START_SOUND_SIGNAL, HIGH);
      delay(100);
      digitalWrite(START_SOUND_SIGNAL, LOW);
    }
    else{
      lastStartButtonState = HIGH;
    }
  }
  else{
    lastStartButtonState = LOW;
  }
  //digitalWrite(19, HIGH);
  controller.updateModel();
  //delay(DELAY);
  //digitalWrite(19, LOW);
  controller.updateView();
  // Serial.println("6");
  //delay(DELAY);
}