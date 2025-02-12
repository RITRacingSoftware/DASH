#pragma once

#include "data-manager.h"
#include "display-manager.h"
#include "lvgl.h"
#include "tft-manager.h"

namespace ScreenDrive {
	lv_obj_t* init(DisplayManager::styles_t* styles);
	void update(DataManager::car_data_t data);
}

/*


// Battery SOC
		elements.bms_soc_bar = lv_bar_create(screen);
		lv_obj_set_size(elements.bms_soc_bar, 100, 230);
		lv_obj_align(elements.bms_soc_bar, LV_ALIGN_TOP_LEFT, 10, 10);
		lv_obj_add_style(elements.bms_soc_bar, &styles->barstyle, 0);
		lv_obj_add_style(elements.bms_soc_bar, &styles->barindstyle, LV_PART_INDICATOR);
		lv_bar_set_range(elements.bms_soc_bar, 0, 100);
		elements.bms_soc_label = lv_label_create(screen);
		lv_obj_align_to(elements.bms_soc_label, elements.bms_soc_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
		lv_label_set_text(elements.bms_soc_label, "??%");

		// Motor RPM
		elements.mcu_rpm_bar = lv_bar_create(screen);
		lv_bar_set_range(elements.mcu_rpm_bar, 0, 5000);
		lv_obj_set_size(elements.mcu_rpm_bar, 100, 230);
		lv_obj_align(elements.mcu_rpm_bar, LV_ALIGN_TOP_RIGHT, -10, 10);
		lv_obj_add_style(elements.mcu_rpm_bar, &styles->barstyle, 0);
		lv_obj_add_style(elements.mcu_rpm_bar, &styles->barindstyle, LV_PART_INDICATOR);
		elements.mcu_rpm_label = lv_label_create(screen);
		lv_label_set_text(elements.mcu_rpm_label, "???? RPM");
		lv_obj_align_to(elements.mcu_rpm_label, elements.mcu_rpm_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

		// Wheel Speed
		lv_style_init(&mph_style);
		lv_style_set_bg_color(&mph_style, lv_color_black());
		lv_style_set_text_color(&mph_style, lv_color_white());
		lv_style_set_text_font(&mph_style, &lv_font_montserrat_28_compressed);

		elements.mph_label = lv_label_create(screen);
		lv_label_set_text(elements.mph_label, "??.? MPH");
		lv_obj_align(elements.mph_label, LV_ALIGN_TOP_MID, 0, 10);
		//lv_obj_add_style(elements.mph_label, &mph_style, LV_PART_MAIN);

		Serial.printf("Initialized Drive Screen\n");
*/