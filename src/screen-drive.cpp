#include "screen-drive.h"

#include <Arduino.h>

#include "data-manager.h"
#include "display-manager.h"

#include "lvgl.h"

namespace ScreenDrive {
	lv_obj_t* screen;
	DataManager::car_data_t lastdata;

	// Display elements to keep
	struct elements_s {
		// TODO: Add display elements :)
	} elements;

	lv_obj_t* init(DisplayManager::styles_t* styles) {
		Serial.printf("Initializing Drive Screen\n");

		screen = lv_obj_create(NULL);
		lv_obj_add_style(screen, &styles->style, LV_PART_MAIN);

		// RPM Bar Label
		lv_obj_t* test_label = lv_label_create(screen);
		lv_label_set_text(test_label, "Drive Screen Placeholder");
		lv_obj_align(test_label, LV_ALIGN_CENTER, 0, 0);

		Serial.printf("Initialized Drive Screen\n");

		return screen;
	}

	void update(DataManager::car_data_t data) {
		// TODO: Process data

		lastdata = data;
	}
}
