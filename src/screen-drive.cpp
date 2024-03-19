#include "screen-drive.h"
#include "tft-manager.h"

#include <Arduino.h>

#include "data-manager.h"
#include "display-manager.h"

#include "lvgl.h"

namespace ScreenDrive {
	lv_obj_t* screen;
	DataManager::car_data_t lastdata;

	// Display elements to keep
	struct elements_s {
		lv_obj_t* bms_soc_bar;
		lv_obj_t* bms_soc_label;

		lv_obj_t* mcu_rpm_bar;
		lv_obj_t* mcu_rpm_label;
		lv_obj_t* mph_label;

		lv_obj_t* brake_temp_bar;
		lv_obj_t* motor_temp_bar;

		lv_obj_t* status_label;
	} elements;

	lv_style_t mph_style;

	lv_obj_t* init(DisplayManager::styles_t* styles) {
		Serial.printf("Initializing Drive Screen\n");

		screen = lv_obj_create(NULL);
		lv_obj_add_style(screen, &styles->style, LV_PART_MAIN);

		// Battery SOC
		elements.bms_soc_bar = lv_bar_create(screen);
		lv_obj_set_size(elements.bms_soc_bar, 700, 430);
		lv_obj_align(elements.bms_soc_bar, LV_ALIGN_TOP_RIGHT, 10, 10);
		lv_obj_add_style(elements.bms_soc_bar, &styles->barstyle, 0);
		lv_obj_add_style(elements.bms_soc_bar, &styles->barindstyle, LV_PART_INDICATOR);
		lv_bar_set_range(elements.bms_soc_bar, 0, 100);
		elements.bms_soc_label = lv_label_create(screen);
		lv_obj_align_to(elements.bms_soc_label, elements.bms_soc_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
		lv_label_set_text(elements.bms_soc_label, "WHATPERCENT??%");

		Serial.printf("Initialized Drive Screen\n");

		return screen;
	}

	void update(DataManager::car_data_t data) {
		// TODO: Process data

		lastdata = data;
	}
}
