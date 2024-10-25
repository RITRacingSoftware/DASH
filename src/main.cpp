#include <Arduino.h>


#include "data-manager.h"
#include "display-manager.h"



#include "lvgl.h"



void setup()
{	

	Serial.begin(9600);
	Serial.printf("Starting!\n");

	DataManager::init();
	lv_init();

 
}

void loop()
{

	DataManager::update();

	delay(100);
}
