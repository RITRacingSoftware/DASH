#include "tft-manager.h"
#include "screen-debug.h"

#include <Arduino.h>

#include "data-manager.h"
#include "display-manager.h"

#include "lvgl.h"

namespace ScreenDebug {
	lv_obj_t* screen;
	DataManager::car_data_t lastdata;

	// Display elements to keep
	struct elements_s {
		lv_obj_t* rpmbar;
		lv_obj_t* rpmlabel;
		lv_obj_t* mphlabel;

		lv_obj_t* bms_soc_label;
		lv_obj_t* bms_cellvoltage_label;
		lv_obj_t* bms_packvoltage_label;
		lv_obj_t* bms_current_label;
		lv_obj_t* bms_maxcurrent_label;

		lv_obj_t* status_overall;
		lv_obj_t* status_vcstatus;
		lv_obj_t* status_mcustatus;
		lv_obj_t* status_bmsstatus;
		lv_obj_t* bms_test_label1;
		lv_obj_t* bms_test_label2;
		lv_obj_t* bms_test_label3;

		lv_obj_t* faults_textarea;

		lv_style_t limp_style;
	} elements;

	const char* VC_STATUS_MESSAGES[] = {
		"NOT READY",
		"STARTUP",
		"READY",
		"FAULTED",
	};

	// const char* VC_FAULT_MESSAGES[] = {
	// 	"VC brake sensor irrational",
	// 	"VC accelerator irrational",
	// 	"VC APPS sensor disagreement",
	// 	"VC APPS double pedal",
	// 	"VC hardfault",
	// };

	// The dbc seems to be outdated/not match what the VC is sending, I know this
	// isn't the correct way to fix it but I'm doing it for now to make it match the VC
	const char* VC_FAULT_MESSAGES[] = {
		"VC brake sensor irrational",
		// "VC accelerator irrational",
		"VC APPS sensor disagreement",
		"VC APPS double pedal",
		// "VC hardfault",
	};

	const char* MCU_STATUS_MESSAGES[] = {
		"DISCONN.",
		"DISABLED",
		"UNLOCKING",
		"ENABLED",
		"READY",
	};

	const char* BMS_FAULT_MESSAGES[] = {
		"BMS slave comm cells",
		"BMS slave comm temps",
		"BMS slave comm drain request",
		"BMS current sensor comm",
		"BMS over current",
		"BMS cell voltage irrational",
		"BMS cell voltage diff",
		"BMS out of juice",
		"BMS temperature irrational",
		"BMS over temperature",
		"BMS drain failure",
	};

	const lv_color_t LIMP_COLORS[] = {
		lv_palette_main(LV_PALETTE_NONE),
		lv_palette_main(LV_PALETTE_GREEN),
		lv_palette_main(LV_PALETTE_LIGHT_BLUE),
		lv_palette_main(LV_PALETTE_YELLOW),
		lv_palette_main(LV_PALETTE_ORANGE),
		lv_palette_main(LV_PALETTE_RED),
	};

	lv_obj_t* init(DisplayManager::styles_t* styles) {
		Serial.printf("Initializing Debug Screen\n");

		screen = lv_obj_create(NULL);
		// lv_obj_add_style(screen, &styles->style, LV_PART_MAIN);

		// Custom styles
		lv_style_init(&elements.limp_style);
		lv_style_set_bg_color(&elements.limp_style, LIMP_COLORS[0]);
		lv_style_set_text_color(&elements.limp_style, lv_color_white());
		lv_obj_add_style(screen, &elements.limp_style, LV_PART_MAIN);
		
		// RPM Bar
		elements.rpmbar = lv_bar_create(screen);
		lv_bar_set_range(elements.rpmbar,  0, 5000);
		lv_obj_set_size(elements.rpmbar, rpm_bar_sizeX, rpm_bar_sizeY);
		lv_obj_align(elements.rpmbar, LV_ALIGN_TOP_LEFT, rpm_bar_align_offsetX, rpm_bar_align_offsetY);
		lv_obj_add_style(elements.rpmbar, &styles->barstyle, 0);
		lv_obj_add_style(elements.rpmbar, &styles->barindstyle, LV_PART_INDICATOR);
		// RPM Bar Label
		elements.rpmlabel = lv_label_create(screen);
		lv_label_set_text(elements.rpmlabel, "????\nRPM");
		lv_obj_align_to(elements.rpmlabel, elements.rpmbar, LV_ALIGN_OUT_BOTTOM_MID, rpmlabel_align_offsetX, rpmlabel_align_offsetY);
		// MPH Label
		elements.mphlabel = lv_label_create(screen);
		lv_label_set_text(elements.mphlabel, "??.?\nMPH");
		lv_obj_align_to(elements.mphlabel, elements.rpmlabel, LV_ALIGN_OUT_BOTTOM_MID, mphlabel_align_offsetX, mphlabel_align_offsetY);
		
		// Status display elements
		lv_obj_t* status_area = lv_obj_create(screen);
		lv_obj_set_size(status_area, status_area_sizeX, status_area_sizeY);
		lv_obj_align(status_area, LV_ALIGN_TOP_MID, status_area_align_offsetX, status_area_align_offsetY);
		lv_obj_add_style(status_area, &styles->style, LV_PART_MAIN);
		// Disable scroll bars
		lv_obj_set_scrollbar_mode(status_area, LV_SCROLLBAR_MODE_OFF);

		elements.status_overall = lv_label_create(status_area);
		lv_obj_align(elements.status_overall, LV_ALIGN_CENTER, status_overall_align_offsetX, status_overall_align_offsetY);
		lv_label_set_recolor(elements.status_overall, true);
		lv_label_set_text(elements.status_overall, "#ff0000 NOT READY#");

		elements.status_vcstatus = lv_label_create(status_area);
		lv_obj_align(elements.status_vcstatus, LV_ALIGN_CENTER, status_vcstatus_align_offsetX, status_vcstatus_align_offsetY);
		lv_label_set_recolor(elements.status_vcstatus, true);
		lv_label_set_text(elements.status_vcstatus, "VC: ???");

		elements.status_mcustatus = lv_label_create(status_area);
		lv_obj_align(elements.status_mcustatus, LV_ALIGN_CENTER, status_mcustatus_align_offsetX, status_mcustatus_align_offsetY);
		lv_label_set_recolor(elements.status_mcustatus, true);
		lv_label_set_text(elements.status_mcustatus, "MC: ???");

		elements.status_bmsstatus = lv_label_create(status_area);
		lv_obj_align(elements.status_bmsstatus, LV_ALIGN_CENTER, status_bmsstatus_align_offsetX, status_bmsstatus_align_offsetY);
		lv_label_set_recolor(elements.status_bmsstatus, true);
		lv_label_set_text(elements.status_bmsstatus, "BMS: ???");
		
		// BMS-related display elements
		lv_obj_t* bms_area = lv_obj_create(screen);
		lv_obj_set_size(bms_area, bms_area_sizeX, bms_area_sizeY);
		lv_obj_align(bms_area, LV_ALIGN_TOP_RIGHT, bms_area_align_offsetX, bms_area_align_offsetY);
		lv_obj_add_style(bms_area, &styles->style, LV_PART_MAIN);
		// Disable scroll bars
		lv_obj_set_scrollbar_mode(bms_area, LV_SCROLLBAR_MODE_OFF);

		// SOC Label
		elements.bms_soc_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_soc_label, LV_ALIGN_CENTER, bms_soc_label_align_offsetX, bms_soc_label_align_offsetY);
		lv_label_set_text(elements.bms_soc_label, "SOC = ???%");

		// Cell Voltages Label
		elements.bms_cellvoltage_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_cellvoltage_label, LV_ALIGN_CENTER, bms_cellvoltage_label_align_offsetX, bms_cellvoltage_label_align_offsetY);
		lv_label_set_text(elements.bms_cellvoltage_label, "V = ?.?? - ?.?? V");

		// Cell Voltages Label
		elements.bms_packvoltage_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_packvoltage_label, LV_ALIGN_CENTER, bms_packvoltage_label_align_offsetX, bms_packvoltage_label_align_offsetY);
		lv_label_set_text(elements.bms_packvoltage_label, "PACK = ???.? V");

		// Current Label
		elements.bms_current_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_current_label, LV_ALIGN_CENTER, bms_current_label_align_offsetX, bms_current_label_align_offsetY);
		lv_label_set_text(elements.bms_current_label, "I = ?.?? A");

		// Current Label
		elements.bms_maxcurrent_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_maxcurrent_label, LV_ALIGN_CENTER, bms_maxcurrent_label_align_offsetX, bms_maxcurrent_label_align_offsetY);
		lv_label_set_text(elements.bms_maxcurrent_label, "MAX I = ?.?? A");
		//test label
		elements.bms_test_label1 = lv_label_create(bms_area);
		lv_obj_align(elements.bms_test_label1, LV_ALIGN_CENTER, bms_maxcurrent_label_align_offsetX, bms_maxcurrent_label_align_offsetY - 10);
		lv_label_set_text(elements.bms_test_label1, "some value = ?.?? A");

		elements.bms_test_label2 = lv_label_create(bms_area);
		lv_obj_align(elements.bms_test_label1, LV_ALIGN_CENTER, bms_maxcurrent_label_align_offsetX, bms_maxcurrent_label_align_offsetY - 20);
		lv_label_set_text(elements.bms_test_label1, "some value = ?.?? A");

		elements.bms_test_label3 = lv_label_create(bms_area);
		lv_obj_align(elements.bms_test_label1, LV_ALIGN_CENTER, bms_maxcurrent_label_align_offsetX, bms_maxcurrent_label_align_offsetY - 30);
		lv_label_set_text(elements.bms_test_label1, "some value = ?.?? A");

		
		// Fault text area
		elements.faults_textarea = lv_textarea_create(screen);
		lv_obj_set_size(elements.faults_textarea, fault_area_sizeX, fault_area_sizeY);
		lv_obj_align(elements.faults_textarea, LV_ALIGN_BOTTOM_RIGHT, fault_area_align_offsetX, fault_area_align_offsetY);
		lv_obj_add_style(elements.faults_textarea, &styles->faultstyle, LV_PART_MAIN);
		
		Serial.printf("Initialized Debug Screen\n");

		return screen;
	}

	void update(DataManager::car_data_t data) {
		if(data.mcu_motorrpm != lastdata.mcu_motorrpm
			|| data.bms_limplevel != lastdata.bms_limplevel) {
			lv_style_set_bg_color(&elements.limp_style, LIMP_COLORS[data.bms_limplevel]);
			lv_bar_set_value(elements.rpmbar, data.mcu_motorrpm, LV_ANIM_OFF);
			lv_label_set_text_fmt(elements.rpmlabel, "%04d\nRPM", data.mcu_motorrpm);
			lv_label_set_text_fmt(elements.mphlabel, "%04.1f\nMPH", data.mcu_wheelspeed);
		}

		// Status Elements
		// TODO: Re-evaluate readiness detection
		/*if(data.vc_status != lastdata.vc_status ||
			data.mcu_status != lastdata.mcu_status ||
			data.bms_faultvector != lastdata.bms_faultvector) {

			if(data.vc_status == 2 && data.mcu_status == 3 &&
				data.bms_faultvector == 0) {
				lv_label_set_text(elements.status_overall, "#00ff00 READY TO DRIVE#");
			}
			else {
				lv_label_set_text(elements.status_overall, "#ff0000 NOT READY#");
			}

		}*/

		//tmp
		if(data.vc_status != lastdata.vc_status) {
			if(data.vc_status == 2) {
				lv_label_set_text(elements.status_overall, "#00ff00 READY TO DRIVE#");
			}
			else {
				lv_label_set_text(elements.status_overall, "#ff0000 NOT READY#");
			}
		}

		if(data.vc_status != lastdata.vc_status) {
			if(data.vc_status >= 0 && data.vc_status <= 2) {
				lv_label_set_text_fmt(elements.status_vcstatus, "VC: %s",
					VC_STATUS_MESSAGES[data.vc_status]);
			}
			else {
				lv_label_set_text_fmt(elements.status_vcstatus, "VC: %s",
					VC_STATUS_MESSAGES[3]);
			}
		}

		if(data.mcu_status != lastdata.mcu_status) {
			if(data.mcu_status >= 1 && data.mcu_status <= 4) {
				lv_label_set_text_fmt(elements.status_mcustatus, "MC: %s",
					MCU_STATUS_MESSAGES[data.mcu_status]);
			}
			else {
				lv_label_set_text_fmt(elements.status_mcustatus, "MC: %s",
					MCU_STATUS_MESSAGES[0]);
			}
		}

		if(data.vc_faultvector != lastdata.vc_faultvector ||
			data.bms_faultvector != lastdata.bms_faultvector) {
			// If any fault message changes, we must update them all...

			bool firstfault = true; // Used for pretty-printing
			uint8_t vc_faultnum = 0;
			uint8_t bms_faultnum = 0;

			lv_textarea_set_text(elements.faults_textarea, "Faults: ");

			// Loop over possible VC faults
			for(int i = 0; i < 5; i++) {
				bool faulted = (data.vc_faultvector >> i) & 1;
				if(faulted) {
					if(!firstfault) {
						// Pretty printing
						lv_textarea_add_text(elements.faults_textarea, ", ");
					}
					firstfault = false;
					lv_textarea_add_text(elements.faults_textarea, VC_FAULT_MESSAGES[i]);
					vc_faultnum++;
				}
			}

			// Loop over possible BMS faults
			for(int i = 0; i < 11; i++) {
				bool faulted = (data.bms_faultvector >> i) & 1;
				if(faulted) {
					if(!firstfault) {
						// Pretty printing
						lv_textarea_add_text(elements.faults_textarea, ", ");
					}
					firstfault = false;
					lv_textarea_add_text(elements.faults_textarea, BMS_FAULT_MESSAGES[i]);
					bms_faultnum++;
				}
			}

			if(data.bms_faultvector == 0) {
				lv_label_set_text(elements.status_bmsstatus, "BMS: READY");
			}
			else {
				lv_label_set_text_fmt(elements.status_bmsstatus, "BMS: %d FAULTS", bms_faultnum);
			}

			if(vc_faultnum == 0 && bms_faultnum == 0) {
				lv_textarea_set_text(elements.faults_textarea, "");
			}
		}

		// BMS Elements
		if(data.bms_soc != lastdata.bms_soc) {
			lv_label_set_text_fmt(elements.bms_soc_label, "SOC = %d%%", data.bms_soc);
		}
		if(data.bms_cellvoltages_min != lastdata.bms_cellvoltages_min ||
				data.bms_cellvoltages_max != lastdata.bms_cellvoltages_max) {
			lv_label_set_text_fmt(elements.bms_cellvoltage_label, "V = %1.2f - %1.2f V",
				data.bms_cellvoltages_min, data.bms_cellvoltages_max);
		}
		if(data.bms_packvoltage != lastdata.bms_packvoltage) {
			lv_label_set_text_fmt(elements.bms_packvoltage_label, "PACK = %3.3f V", data.bms_packvoltage);
		}
		if(data.bms_buscurrent != lastdata.bms_buscurrent) {
			lv_label_set_text_fmt(elements.bms_current_label, "I = %1.2f A", data.bms_buscurrent);
			if(data.bms_maxcurrent != lastdata.bms_maxcurrent) {
				lv_label_set_text_fmt(elements.bms_maxcurrent_label, "MAX I = %1.2f A", data.bms_maxcurrent);
			}
		}
		/*
		if(data.vc_status != lastdata.bms_buscurrent) {
			lv_label_set_text_fmt(elements.bms_current_label, "I = %1.2f A", data.bms_buscurrent);
			if(data.bms_maxcurrent != lastdata.bms_maxcurrent) {
				lv_label_set_text_fmt(elements.bms_maxcurrent_label, "MAX I = %1.2f A", data.bms_maxcurrent);
			}
		}		
		
		float lv_voltage; //voltage of lv batt how get? need dbc update perhaps
		float goPedal_A;
		float goPedal_B;
		float goPedal_AVG;
		float nogo_pressure;
		*/

		if(data.goPedal_AVG != lastdata.goPedal_AVG ){
			lv_label_set_text_fmt(elements.bms_test_label1, "go pedal= %1.3f ", data.goPedal_A);
			lv_label_set_text_fmt(elements.bms_test_label2, "go pedal= %1.3f ", data.goPedal_B);
			lv_label_set_text_fmt(elements.bms_test_label3, "go pedal= %1.3f ", data.goPedal_AVG);
		}
		
	}
}
