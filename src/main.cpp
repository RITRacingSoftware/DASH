#include <Arduino.h>

#include "display-manager.h"
#include "can-message.h"
#include "can-processor.h"
#include "../lib/lvgl/lvgl.h"

#define BMS_STATUS_ID 0x258
#define MOTOR_POSITION_ID 0x0A5

lv_obj_t* label1;
lv_obj_t* bar1;
lv_style_t barstyle;
CAN_PROCESSOR canProc;

int i = 0;
uint64_t starttime;

void label_updater(void* p) {
	lv_label_set_text_fmt(label1, "Motor speed = %d rpm", i);
	lv_bar_set_value(bar1, i%11, LV_ANIM_OFF);
}

void setup()
{
	Serial.begin(9600);
	Serial.printf("Starting!\n");

	DisplayManager::init();

	label1 = lv_label_create(lv_scr_act());
	lv_label_set_text(label1, "Testing");
	lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);
	//lv_obj_add_style(label1, DisplayManager::getStyle(), LV_PART_MAIN);

	lv_style_init(&barstyle);
	lv_style_set_border_color(&barstyle, lv_palette_main(LV_PALETTE_BLUE));
	lv_style_set_border_width(&barstyle, 2);
	lv_style_set_radius(&barstyle, 1);
	lv_style_set_pad_all(&barstyle, 4);


	bar1 = lv_bar_create(lv_scr_act());
	lv_bar_set_range(bar1, 0, 100);
	lv_obj_set_width(bar1, 16);
	lv_obj_set_height(bar1, 100);
	lv_obj_align(bar1, LV_ALIGN_CENTER, -100, 0);
	lv_obj_add_style(bar1, &barstyle, 0);
	lv_obj_add_style(bar1, &barstyle, LV_PART_INDICATOR);

	lv_timer_create(label_updater, 50, NULL);

	starttime = millis();
}

void loop()
{
	Serial.print("Update\n");

	i = (millis() - starttime) / 1000;
	DisplayManager::update();
	delay(5);

	CAN_MESSAGE message;
	bool readed = canProc.readCAN(message);
	/*if(readed && message.id == BMS_STATUS_ID) {
		int soc = message.data[0];
		i = soc;
	}*/

	if(readed && message.id == MOTOR_POSITION_ID) {
		int16_t motorrpm = message.data[2] | (message.data[3] << 8);
		i = motorrpm;
	}


}
