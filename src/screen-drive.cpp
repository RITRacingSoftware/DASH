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
		lv_obj_t* brake_temp_bar;
		lv_obj_t* brake_temp_label;

		lv_obj_t* fl_temp_bar;
		lv_obj_t* fl_temp_label;
		lv_obj_t* fr_temp_bar;
		lv_obj_t* fr_temp_label;
		lv_obj_t* rl_temp_bar;
		lv_obj_t* rl_temp_label;
		lv_obj_t* rr_temp_bar;
		lv_obj_t* rr_temp_label;

		lv_obj_t* rtd_label;

		lv_obj_t* hv_voltage_label;
		lv_obj_t* hv_cells_label;
		lv_obj_t* hv_temp_label;

		lv_obj_t* faults_area;

		lv_obj_t* mph_label;
		lv_obj_t* lv_voltage_label;
	} elements;

	lv_style_t mph_style;

	lv_obj_t* init(DisplayManager::styles_t* styles) {
		Serial.printf("Initializing Drive Screen\n");

		screen = lv_obj_create(NULL);
		lv_obj_add_style(screen, &styles->style, LV_PART_MAIN);

		// Status elements
		elements.rtd_label = lv_label_create(screen);
		lv_obj_align(elements.rtd_label, LV_ALIGN_BOTTOM_LEFT, 20, -30);
		lv_label_set_recolor(elements.rtd_label, true);
		lv_label_set_text(elements.rtd_label, "#ff0000 NOT READY#");

		elements.mph_label = lv_label_create(screen);
		lv_obj_align(elements.mph_label, LV_ALIGN_BOTTOM_MID, 0, -30);
		lv_label_set_recolor(elements.mph_label, true);
		lv_label_set_text(elements.mph_label, "MPH: ??");

		elements.lv_voltage_label = lv_label_create(screen);
		lv_obj_align(elements.lv_voltage_label, LV_ALIGN_BOTTOM_RIGHT, -40, -30);
		lv_label_set_recolor(elements.lv_voltage_label, true);
		lv_label_set_text(elements.lv_voltage_label, "LV: ??.?");

		// Temps
		elements.brake_temp_bar = lv_bar_create(screen);
		lv_bar_set_range(elements.brake_temp_bar, 0, 580);
		lv_obj_set_size(elements.brake_temp_bar, 360, 110);
		lv_obj_align(elements.brake_temp_bar, LV_ALIGN_TOP_LEFT, 20, 20);
		lv_obj_add_style(elements.brake_temp_bar, &styles->barstyle, 0);
		lv_obj_add_style(elements.brake_temp_bar, &styles->barindstyle, LV_PART_INDICATOR);
		elements.brake_temp_label = lv_label_create(screen);
		lv_label_set_text(elements.brake_temp_label, "?? C");
		lv_obj_align_to(elements.brake_temp_label, elements.brake_temp_bar, LV_ALIGN_CENTER, 0, 0);

		elements.fl_temp_bar = lv_bar_create(screen);
		lv_bar_set_range(elements.fl_temp_bar, 0, 100);
		lv_obj_set_size(elements.fl_temp_bar, 175, 110);
		lv_obj_align(elements.fl_temp_bar, LV_ALIGN_TOP_LEFT, 20, 140);
		lv_obj_add_style(elements.fl_temp_bar, &styles->barstyle, 0);
		lv_obj_add_style(elements.fl_temp_bar, &styles->barindstyle, LV_PART_INDICATOR);
		elements.fl_temp_label = lv_label_create(screen);
		lv_label_set_text(elements.fl_temp_label, "?? C");
		lv_obj_align_to(elements.fl_temp_label, elements.fl_temp_bar, LV_ALIGN_CENTER, 0, 0);

		elements.fr_temp_bar = lv_bar_create(screen);
		lv_bar_set_range(elements.fr_temp_bar, 0, 100);
		lv_obj_set_size(elements.fr_temp_bar, 175, 110);
		lv_obj_align(elements.fr_temp_bar, LV_ALIGN_TOP_LEFT, 205, 140);
		lv_obj_add_style(elements.fr_temp_bar, &styles->barstyle, 0);
		lv_obj_add_style(elements.fr_temp_bar, &styles->barindstyle, LV_PART_INDICATOR);
		elements.fr_temp_label = lv_label_create(screen);
		lv_label_set_text(elements.fr_temp_label, "?? C");
		lv_obj_align_to(elements.fr_temp_label, elements.fr_temp_bar, LV_ALIGN_CENTER, 0, 0);

		elements.rl_temp_bar = lv_bar_create(screen);
		lv_bar_set_range(elements.rl_temp_bar, 0, 100);
		lv_obj_set_size(elements.rl_temp_bar, 175, 110);
		lv_obj_align(elements.rl_temp_bar, LV_ALIGN_TOP_LEFT, 20, 260);
		lv_obj_add_style(elements.rl_temp_bar, &styles->barstyle, 0);
		lv_obj_add_style(elements.rl_temp_bar, &styles->barindstyle, LV_PART_INDICATOR);
		elements.rl_temp_label = lv_label_create(screen);
		lv_label_set_text(elements.rl_temp_label, "?? C");
		lv_obj_align_to(elements.rl_temp_label, elements.rl_temp_bar, LV_ALIGN_CENTER, 0, 0);

		elements.rr_temp_bar = lv_bar_create(screen);
		lv_bar_set_range(elements.rr_temp_bar, 0, 100);
		lv_obj_set_size(elements.rr_temp_bar, 175, 110);
		lv_obj_align(elements.rr_temp_bar, LV_ALIGN_TOP_LEFT, 205, 260);
		lv_obj_add_style(elements.rr_temp_bar, &styles->barstyle, 0);
		lv_obj_add_style(elements.rr_temp_bar, &styles->barindstyle, LV_PART_INDICATOR);
		elements.rr_temp_label = lv_label_create(screen);
		lv_label_set_text(elements.rr_temp_label, "?? C");
		lv_obj_align_to(elements.rr_temp_label, elements.rr_temp_bar, LV_ALIGN_CENTER, 0, 0);

		// HV Stuff
		lv_obj_t* hv_area = lv_obj_create(screen);
		lv_obj_set_size(hv_area, 390, 180);
		lv_obj_align(hv_area, LV_ALIGN_TOP_RIGHT, -20, 20);
		lv_obj_add_style(hv_area, &styles->style, LV_PART_MAIN);

		elements.hv_voltage_label = lv_label_create(hv_area);
		lv_obj_align(elements.hv_voltage_label, LV_ALIGN_TOP_MID, 0, -5);
		lv_label_set_recolor(elements.hv_voltage_label, true);
		lv_label_set_text(elements.hv_voltage_label, "HV: ???V (??%)");

		elements.hv_cells_label = lv_label_create(hv_area);
		lv_obj_align(elements.hv_cells_label, LV_ALIGN_CENTER, 0, 0);
		lv_label_set_recolor(elements.hv_cells_label, true);
		lv_label_set_text(elements.hv_cells_label, "CELLS: ?.??-?.?? V");

		elements.hv_temp_label = lv_label_create(hv_area);
		lv_obj_align(elements.hv_temp_label, LV_ALIGN_BOTTOM_MID, 0, 5);
		lv_label_set_recolor(elements.hv_temp_label, true);
		lv_label_set_text(elements.hv_temp_label, "MAX TEMP: ?? C");

		// Fault text area
		elements.faults_area = lv_textarea_create(screen);
		lv_obj_set_size(elements.faults_area, 390, 160);
		lv_obj_align(elements.faults_area, LV_ALIGN_RIGHT_MID, -20, 50);
		lv_obj_add_style(elements.faults_area, &styles->faultstyle, LV_PART_MAIN);

		Serial.printf("Initialized Drive Screen\n");
		return screen;
	}

	void update(DataManager::car_data_t data) {
		// Temporary stuff
		lv_textarea_set_text(elements.faults_area, "FAULTS: ");

		// Status elements
		if(data.vc_status != lastdata.vc_status) {
			if(data.vc_status == 2) {
				lv_label_set_text(elements.rtd_label, "#00ff00 READY TO DRIVE#");
			}
			else {
				lv_label_set_text(elements.rtd_label, "#ff0000 NOT READY#");
			}
		}
		if (data.mcu_wheelspeed != lastdata.mcu_wheelspeed) {
			lv_label_set_text_fmt(elements.mph_label, "MPH: %2.0f V", data.mcu_wheelspeed);
		}
		if (data.lv_voltage != lastdata.lv_voltage) {
			lv_label_set_text_fmt(elements.lv_voltage_label, "LV: %2.1f V", data.lv_voltage);
		}

		// Temperature elements
		if (data.rotortemp != lastdata.rotortemp) {
			lv_bar_set_value(elements.brake_temp_bar, data.rotortemp, LV_ANIM_OFF);
			lv_label_set_text_fmt(elements.brake_temp_label, "%0.0f C", data.rotortemp);
		}
		if (data.tiretemp_fl != lastdata.tiretemp_fl || data.tiretemp_fr != lastdata.tiretemp_fr ||
		        data.tiretemp_rl != lastdata.tiretemp_rl || data.tiretemp_rr != lastdata.tiretemp_rr) {
			lv_bar_set_value(elements.fl_temp_bar, data.tiretemp_fl, LV_ANIM_OFF);
			lv_label_set_text_fmt(elements.fl_temp_label, "%0.0f C", data.tiretemp_fl);
			lv_bar_set_value(elements.fr_temp_bar, data.tiretemp_fr, LV_ANIM_OFF);
			lv_label_set_text_fmt(elements.fr_temp_label, "%0.0f C", data.tiretemp_fr);
			lv_bar_set_value(elements.rl_temp_bar, data.tiretemp_rl, LV_ANIM_OFF);
			lv_label_set_text_fmt(elements.rl_temp_label, "%0.0f C", data.tiretemp_rl);
			lv_bar_set_value(elements.rr_temp_bar, data.tiretemp_rr, LV_ANIM_OFF);
			lv_label_set_text_fmt(elements.rr_temp_label, "%0.0f C", data.tiretemp_rr);
		}

		// HV Elements
		if(data.bms_packvoltage != lastdata.bms_packvoltage || data.bms_soc != lastdata.bms_soc) {
			lv_label_set_text_fmt(elements.hv_voltage_label, "HV: %3.0f (%d%%)", data.bms_packvoltage, data.bms_soc);
		}
		if(data.bms_cellvoltages_min != lastdata.bms_cellvoltages_min ||
				data.bms_cellvoltages_max != lastdata.bms_cellvoltages_max) {
			lv_label_set_text_fmt(elements.hv_cells_label, "CELLS: %1.2f-%1.2f V",
				data.bms_cellvoltages_min, data.bms_cellvoltages_max);
		}
		if(data.bms_maxtemp != lastdata.bms_maxtemp) {
			lv_label_set_text_fmt(elements.hv_temp_label, "TEMP %3.0f C", data.bms_maxtemp);
		}

		lastdata = data;
	}
}
