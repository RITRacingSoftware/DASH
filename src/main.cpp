#include <Arduino.h>

#include "display-manager.h"
#include "can-message.h"
#include "can-processor.h"
#include "../lib/lvgl/lvgl.h"

#define BMS_STATUS_ID 0x258

lv_obj_t* label1;
CAN_PROCESSOR canProc;

int i = 0;
uint64_t starttime;

void label_updater(void* p) {
	lv_label_set_text_fmt(label1, "i = %d", i);
}

void setup()
{
	Serial.begin(9600);
	Serial.printf("Starting!\n");

	DisplayManager::init();

	label1 = lv_label_create(lv_scr_act());
	lv_label_set_text(label1, "Testing");
	lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);
	lv_obj_add_style(label1, DisplayManager::getStyle(), LV_PART_MAIN);

	lv_timer_create(label_updater, 50, NULL);

	starttime = millis();
}

void loop()
{
	Serial.print("Update\n");

	i = (millis() - starttime) / 1000;

	lv_timer_handler();
	delay(5);

	CAN_MESSAGE message;
	bool readed = canProc.readCAN(message);
	if(readed && message.id == BMS_STATUS_ID) {
		int soc = message.data[0];
		i = soc;
	}
}
