#include "screen-debug.h"

#include "data-manager.h"
#include "display-manager.h"
#include "../lib/lvgl/lvgl.h"

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

		lv_obj_t* faults_textarea;
	} elements;

	const char* VC_STATUS_MESSAGES[] = {
		"NOT READY",
		"STARTUP",
		"READY",
		"FAULTED",
	};

	const char* VC_FAULT_MESSAGES[] = {
		"VC brake sensor irrational",
		"VC accelerator irrational",
		"VC APPS sensor disagreement",
		"VC APPS double pedal",
		"VC hardfault",
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

	lv_obj_t* init(DisplayManager::styles_t styles) {
		screen = lv_obj_create(NULL);

		lv_obj_add_style(screen, &styles.style, LV_PART_MAIN);

		// RPM Bar
		elements.rpmbar = lv_bar_create(screen);
		lv_bar_set_range(elements.rpmbar, 0, 5000);
		lv_obj_set_size(elements.rpmbar, 47, 156);
		lv_obj_align(elements.rpmbar, LV_ALIGN_TOP_LEFT, 10, 10);
		lv_obj_add_style(elements.rpmbar, &styles.barstyle, 0);
		lv_obj_add_style(elements.rpmbar, &styles.barindstyle, LV_PART_INDICATOR);
		// RPM Bar Label
		elements.rpmlabel = lv_label_create(screen);
		lv_label_set_text(elements.rpmlabel, "????\nRPM");
		lv_obj_align_to(elements.rpmlabel, elements.rpmbar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
		// MPH Label
		elements.mphlabel = lv_label_create(screen);
		lv_label_set_text(elements.mphlabel, "??.?\nMPH");
		lv_obj_align_to(elements.mphlabel, elements.rpmlabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

		// Status display elements
		lv_obj_t* status_area = lv_obj_create(screen);
		lv_obj_set_size(status_area, 210, 180);
		lv_obj_align(status_area, LV_ALIGN_TOP_MID, -70, 10);
		lv_obj_add_style(status_area, &styles.style, LV_PART_MAIN);
		// Disable scroll bars
		lv_obj_set_scrollbar_mode(status_area, LV_SCROLLBAR_MODE_OFF);

		elements.status_overall = lv_label_create(status_area);
		lv_obj_align(elements.status_overall, LV_ALIGN_CENTER, 0, -38);
		lv_label_set_recolor(elements.status_overall, true);
		lv_label_set_text(elements.status_overall, "#ff0000 NOT READY#");

		elements.status_vcstatus = lv_label_create(status_area);
		lv_obj_align(elements.status_vcstatus, LV_ALIGN_CENTER, 0, -13);
		lv_label_set_recolor(elements.status_vcstatus, true);
		lv_label_set_text(elements.status_vcstatus, "VC: ???");

		elements.status_mcustatus = lv_label_create(status_area);
		lv_obj_align(elements.status_mcustatus, LV_ALIGN_CENTER, 0, 12);
		lv_label_set_recolor(elements.status_mcustatus, true);
		lv_label_set_text(elements.status_mcustatus, "MC: ???");

		elements.status_bmsstatus = lv_label_create(status_area);
		lv_obj_align(elements.status_bmsstatus, LV_ALIGN_CENTER, 0, 37);
		lv_label_set_recolor(elements.status_bmsstatus, true);
		lv_label_set_text(elements.status_bmsstatus, "BMS: ???");

		// BMS-related display elements
		lv_obj_t* bms_area = lv_obj_create(screen);
		lv_obj_set_size(bms_area, 190, 180);
		lv_obj_align(bms_area, LV_ALIGN_TOP_RIGHT, -10, 10);
		lv_obj_add_style(bms_area, &styles.style, LV_PART_MAIN);
		// Disable scroll bars
		lv_obj_set_scrollbar_mode(bms_area, LV_SCROLLBAR_MODE_OFF);

		// SOC Label
		elements.bms_soc_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_soc_label, LV_ALIGN_CENTER, 0, -50);
		lv_label_set_text(elements.bms_soc_label, "SOC = ???%");

		// Cell Voltages Label
		elements.bms_cellvoltage_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_cellvoltage_label, LV_ALIGN_CENTER, 0, -25);
		lv_label_set_text(elements.bms_cellvoltage_label, "V = ?.?? - ?.?? V");

		// Cell Voltages Label
		elements.bms_packvoltage_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_packvoltage_label, LV_ALIGN_CENTER, 0, 0);
		lv_label_set_text(elements.bms_packvoltage_label, "PACK = ???.? V");

		// Current Label
		elements.bms_current_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_current_label, LV_ALIGN_CENTER, 0, 25);
		lv_label_set_text(elements.bms_current_label, "I = ?.?? A");

		// Current Label
		elements.bms_maxcurrent_label = lv_label_create(bms_area);
		lv_obj_align(elements.bms_maxcurrent_label, LV_ALIGN_CENTER, 0, 50);
		lv_label_set_text(elements.bms_maxcurrent_label, "MAX I = ?.?? A");

		// Fault text area
		elements.faults_textarea = lv_textarea_create(screen);
		lv_obj_set_size(elements.faults_textarea, 405, 60);
		lv_obj_align(elements.faults_textarea, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
		lv_obj_add_style(elements.faults_textarea, &styles.faultstyle, LV_PART_MAIN);

		return screen;
	}

	void update(DataManager::car_data_t data) {
		if(data.mcu_motorrpm != lastdata.mcu_motorrpm) {
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
				data.bms_cellvoltages_min * 0.01, data.bms_cellvoltages_max * 0.01);
		}
		if(data.bms_packvoltage != lastdata.bms_packvoltage) {
			lv_label_set_text_fmt(elements.bms_packvoltage_label, "PACK = %3.1f V", data.bms_packvoltage * 0.1);
		}
		if(data.bms_buscurrent != lastdata.bms_buscurrent) {
			lv_label_set_text_fmt(elements.bms_current_label, "I = %1.2f A", data.bms_buscurrent * 0.001);
			if(data.bms_maxcurrent != lastdata.bms_maxcurrent) {
				lv_label_set_text_fmt(elements.bms_maxcurrent_label, "MAX I = %1.2f A", data.bms_maxcurrent * 0.001);
			}
		}

		lastdata = data;
	}
}
