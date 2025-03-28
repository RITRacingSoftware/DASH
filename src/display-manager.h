#pragma once

#include "lvgl.h"
#include "data-manager.h"

namespace DisplayManager {
	typedef struct styles_s {
		lv_style_t style;
		lv_style_t barstyle;
		lv_style_t barindstyle;
		lv_style_t faultstyle;
	} styles_t;

	const char* VC_STATUS_MESSAGES[] = {
		"NOT READY",
		"STARTUP",
		"READY",
		"FAULTED",
	};

	const char* VC_FAULT_MESSAGES[] = {
		"VC BRAKE SENSOR IRRATIONAL",
		// "VC ACCELERATOR IRRATIONAL",
		"VC APPS SENSOR DISAGREEMENT",
		"VC APPS DOUBLE PEDAL",
		// "VC HARDFAULT",
	};

	const char* MCU_STATUS_MESSAGES[] = {
		"DISCONN.",
		"DISABLED",
		"UNLOCKING",
		"ENABLED",
		"READY",
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
	
	void init();
	void update(DataManager::car_data_t data);
	void switchScreens(int new_screen);
}
