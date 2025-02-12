#include "screen-eve.h"
#include "tft-manager.h"

#include <Arduino.h>

#include "data-manager.h"
#include "display-manager.h"


#include "lvgl.h"

LV_IMG_DECLARE(drive_screen_eva_version_v3_immovable);
namespace ScreenEve {
	
	// The dbc seems to be outdated/not match what the VC is sending, I know this
	// isn't the correct way to fix it but I'm doing it for now to make it match the VC
	const char* VC_FAULT_MESSAGES[] = {
		"VC BRAKE SENSOR IRRATIONAL",
		// "VC ACCELERATOR IRRATIONAL",
		"VC APPS SENSOR DISAGREEMENT",
		"VC APPS DOUBLE PEDAL",
		// "VC HARDFAULT",
	};

	const char* BMS_FAULT_MESSAGES[] = {
		"BMS SLAVE COMM CELLS",
		"BMS SLAVE COMM TEMPS",
		"BMS SLAVE COMM DRAIN REQUEST",
		"BMS CURRENT SENSOR COMM",
		"BMS OVER CURRENT",
		"BMS CELL VOLTAGE IRRATIONAL",
		"BMS CELL VOLTAGE DIFF",
		"BMS OUT OF JUICE",
		"BMS TEMPERATURE IRRATIONAL",
		"BMS OVER TEMPERATURE",
		"BMS DRAIN FAILURE",
	};

	lv_obj_t* screen;
	DataManager::car_data_t lastdata;

	// Display elements to keep
	struct elements_s {
		lv_obj_t* background;
	} elements;

	lv_style_t temp_cold_style;
	lv_style_t temp_optimal_style;
	lv_style_t temp_hot_style;

	lv_obj_t* init(DisplayManager::styles_t* styles) {
		Serial.printf("Initializing Eve Screen\n");

		//Styles
		lv_style_init(&temp_cold_style);
		lv_style_set_bg_color(&temp_cold_style, lv_palette_main(LV_PALETTE_BLUE));
		lv_style_init(&temp_optimal_style);
		lv_style_set_bg_color(&temp_optimal_style, lv_palette_main(LV_PALETTE_GREEN));
		lv_style_init(&temp_hot_style);
		lv_style_set_bg_color(&temp_hot_style, lv_palette_main(LV_PALETTE_RED));

		screen = lv_obj_create(NULL);
		lv_obj_add_style(screen, &styles->style, LV_PART_MAIN);

		// Background image
		elements.background = lv_img_create(screen);
		lv_img_set_src(elements.background, &drive_screen_eva_version_v3_immovable);
		lv_obj_align(elements.background, LV_ALIGN_CENTER, 0, 0);

		Serial.printf("Initialized Eve Screen\n");
		return screen;
	}

	void update(DataManager::car_data_t data) {
		// Status elements
	}
}