#include "etl/array.h"
#include <Arduino.h>

etl::array<uint8_t, 4> myArr;

void setup() {
  // put your setup code here, to run once:
  myArr = {0, 1, 2, 3};
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 3; i++) {
    Serial.printf("My number: %u", myArr[i]);
  }
  delay(1000);
}