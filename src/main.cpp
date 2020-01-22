#include <Arduino.h>
#include "controller/tft-controller.h"
#include "data-processor/data-processor.h"

DATA_PROCESSOR dataProcessor;
TFT_CONTROLLER controller(&dataProcessor);

void setup() {
  Serial.print("startingg");
  pinMode(13, OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  controller.initialize();
  digitalWrite(13, HIGH);
}

void loop() {
<<<<<<< HEAD
  //Serial.println("1");
  //digitalWrite(13, HIGH);
=======
  digitalWrite(13, HIGH);
>>>>>>> 6d9f02759078aafa1638d08f8b47bb8b061a86c0
  controller.updateModel();
  delay(80);
  //digitalWrite(13, LOW);
  controller.updateView();
  //Serial.println("6");
  delay(80);

}