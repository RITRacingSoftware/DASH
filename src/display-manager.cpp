#include "display-manager.h"

#include <Arduino.h>

#include "../lib/lvgl/lvgl.h"
#include "tft-manager.h"

#define DRAW_BUFFER_SIZE (TFT_SCREEN_PIXELS / 10)

namespace DisplayManager {
	struct display_elements_s {
		lv_obj_t* rpmbar;
		lv_obj_t* rpmlabel;

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
	} display_elements;

	lv_disp_draw_buf_t drawbuf;
	lv_color_t drawbuf1[DRAW_BUFFER_SIZE];
	lv_disp_drv_t disp_drv;

	DataManager::car_data_t curdata;
	DataManager::car_data_t lastdata;

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

	void disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
		TFTManager::drawTexturedRect(area->x1, area->x2, area->y1, area->y2, (uint16_t*) color_p);
		lv_disp_flush_ready(disp);
	}

	void initLVGL() {
		lv_init();

		lv_disp_draw_buf_init(&drawbuf, drawbuf1, NULL, DRAW_BUFFER_SIZE);

		lv_disp_drv_init(&disp_drv);
		disp_drv.flush_cb = disp_flush;
		disp_drv.draw_buf = &drawbuf;
		disp_drv.hor_res = TFT_SCREEN_WIDTH;
		disp_drv.ver_res = TFT_SCREEN_HEIGHT;
		lv_disp_drv_register(&disp_drv);
	}

	lv_style_t style;
	lv_style_t barstyle;
	lv_style_t barindstyle;
	lv_style_t faultstyle;

	void initDisplayElements() {
		// Global style, can be overridden
		lv_style_init(&style);
		lv_style_set_bg_color(&style, lv_color_black());
		lv_style_set_text_color(&style, lv_color_white());
		lv_obj_add_style(lv_scr_act(), &style, LV_PART_MAIN);

		// Progress bar style
		lv_style_init(&barstyle);
		lv_style_set_border_color(&barstyle, lv_color_white());
		lv_style_set_border_width(&barstyle, 2);
		lv_style_set_radius(&barstyle, 1);
		lv_style_set_pad_all(&barstyle, 4);
		lv_style_init(&barindstyle);
		//lv_style_set_border_color(&barindstyle, lv_color_white());
		//lv_style_set_border_width(&barindstyle, 2);
		lv_style_set_radius(&barindstyle, 1);
		lv_style_set_pad_all(&barindstyle, 4);

		// RPM Bar
		display_elements.rpmbar = lv_bar_create(lv_scr_act());
		lv_bar_set_range(display_elements.rpmbar, 0, 5000);
		lv_obj_set_size(display_elements.rpmbar, 47, 135);
		lv_obj_align(display_elements.rpmbar, LV_ALIGN_TOP_LEFT, 10, 10);
		lv_obj_add_style(display_elements.rpmbar, &barstyle, 0);
		lv_obj_add_style(display_elements.rpmbar, &barindstyle, LV_PART_INDICATOR);
		// RPM Bar Label
		display_elements.rpmlabel = lv_label_create(lv_scr_act());
		lv_label_set_text(display_elements.rpmlabel, "????\nRPM");
		lv_obj_align_to(display_elements.rpmlabel, display_elements.rpmbar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

		// Status display elements
		lv_obj_t* status_area = lv_obj_create(lv_scr_act());
		lv_obj_set_size(status_area, 210, 180);
		lv_obj_align(status_area, LV_ALIGN_TOP_MID, -70, 10);
		lv_obj_add_style(status_area, &style, LV_PART_MAIN);
		// Disable scroll bars
		lv_obj_set_scrollbar_mode(status_area, LV_SCROLLBAR_MODE_OFF);

		display_elements.status_overall = lv_label_create(status_area);
		lv_obj_align(display_elements.status_overall, LV_ALIGN_CENTER, 0, -38);
		lv_label_set_recolor(display_elements.status_overall, true);
		lv_label_set_text(display_elements.status_overall, "#00ff00 READY TO DRIVE#");

		display_elements.status_vcstatus = lv_label_create(status_area);
		lv_obj_align(display_elements.status_vcstatus, LV_ALIGN_CENTER, 0, -13);
		lv_label_set_recolor(display_elements.status_vcstatus, true);
		lv_label_set_text(display_elements.status_vcstatus, "VC: READY");

		display_elements.status_mcustatus = lv_label_create(status_area);
		lv_obj_align(display_elements.status_mcustatus, LV_ALIGN_CENTER, 0, 12);
		lv_label_set_recolor(display_elements.status_mcustatus, true);
		lv_label_set_text(display_elements.status_mcustatus, "MCU: READY");

		display_elements.status_bmsstatus = lv_label_create(status_area);
		lv_obj_align(display_elements.status_bmsstatus, LV_ALIGN_CENTER, 0, 37);
		lv_label_set_recolor(display_elements.status_bmsstatus, true);
		lv_label_set_text(display_elements.status_bmsstatus, "BMS: READY");

		// BMS-related display elements
		lv_obj_t* bms_area = lv_obj_create(lv_scr_act());
		lv_obj_set_size(bms_area, 190, 180);
		lv_obj_align(bms_area, LV_ALIGN_TOP_RIGHT, -10, 10);
		lv_obj_add_style(bms_area, &style, LV_PART_MAIN);

		// SOC Label
		display_elements.bms_soc_label = lv_label_create(bms_area);
		lv_obj_align(display_elements.bms_soc_label, LV_ALIGN_CENTER, 0, -50);
		lv_label_set_text(display_elements.bms_soc_label, "SOC = ???%");

		// Cell Voltages Label
		display_elements.bms_cellvoltage_label = lv_label_create(bms_area);
		lv_obj_align(display_elements.bms_cellvoltage_label, LV_ALIGN_CENTER, 0, -25);
		lv_label_set_text(display_elements.bms_cellvoltage_label, "V = ?.?? - ?.?? V");

		// Cell Voltages Label
		display_elements.bms_packvoltage_label = lv_label_create(bms_area);
		lv_obj_align(display_elements.bms_packvoltage_label, LV_ALIGN_CENTER, 0, 0);
		lv_label_set_text(display_elements.bms_packvoltage_label, "PACK = ?.?? V");

		// Current Label
		display_elements.bms_current_label = lv_label_create(bms_area);
		lv_obj_align(display_elements.bms_current_label, LV_ALIGN_CENTER, 0, 25);
		lv_label_set_text(display_elements.bms_current_label, "I = ?.?? A");

		// Current Label
		display_elements.bms_maxcurrent_label = lv_label_create(bms_area);
		lv_obj_align(display_elements.bms_maxcurrent_label, LV_ALIGN_CENTER, 0, 50);
		lv_label_set_text(display_elements.bms_maxcurrent_label, "MAX I = ?.?? A");

		// Fault font style
		lv_style_init(&faultstyle);
		lv_style_set_bg_color(&faultstyle, lv_color_black());
		lv_style_set_text_color(&faultstyle, lv_color_white());
		lv_style_set_text_font(&faultstyle, &lv_font_montserrat_14);

		// Fault text area
		display_elements.faults_textarea = lv_textarea_create(lv_scr_act());
		lv_obj_set_size(display_elements.faults_textarea, 460, 60);
		lv_obj_align(display_elements.faults_textarea, LV_ALIGN_BOTTOM_MID, 0, -10);
		lv_obj_add_style(display_elements.faults_textarea, &faultstyle, LV_PART_MAIN);
	}

	void updateDisplayElements() {
		if(curdata.mcu_motorrpm != lastdata.mcu_motorrpm) {
			lv_bar_set_value(display_elements.rpmbar, curdata.mcu_motorrpm, LV_ANIM_OFF);
			lv_label_set_text_fmt(display_elements.rpmlabel, "%04d\nRPM", curdata.mcu_motorrpm);
		}

		// Status Elements
		if(curdata.bms_faultvector != lastdata.bms_faultvector) {
			Serial.printf("BMS fault vector = 0x%04x\n", curdata.bms_faultvector);
			if(curdata.bms_faultvector == 0) {
				lv_label_set_text(display_elements.status_bmsstatus, "BMS: READY");
				lv_textarea_set_text(display_elements.faults_textarea, "");
			}
			else {
				lv_textarea_set_text(display_elements.faults_textarea, "Faults: ");
				uint8_t faultnum = 0;
				for(int i = 0; i < 11; i++) {
					bool faulted = (curdata.bms_faultvector >> i) & 1;
					if(faulted) {
						lv_textarea_add_text(display_elements.faults_textarea, BMS_FAULT_MESSAGES[i]);
						Serial.printf("BMS fault #%d\n", i);
						faultnum++;
					}
				}
				lv_label_set_text_fmt(display_elements.status_bmsstatus, "BMS: %d FAULTS", faultnum);
			}
		}

		// BMS Elements
		if(curdata.bms_soc != lastdata.bms_soc) {
			lv_label_set_text_fmt(display_elements.bms_soc_label, "SOC = %d%%", curdata.bms_soc);
		}
		if(curdata.bms_cellvoltages_min != lastdata.bms_cellvoltages_min ||
				curdata.bms_cellvoltages_max != lastdata.bms_cellvoltages_max) {
			lv_label_set_text_fmt(display_elements.bms_cellvoltage_label, "V = %1.2f - %1.2f V",
				curdata.bms_cellvoltages_min * 0.01, curdata.bms_cellvoltages_max * 0.01);
		}
		if(curdata.bms_buscurrent != lastdata.bms_buscurrent) {
			lv_label_set_text_fmt(display_elements.bms_current_label, "I = %1.2f A", curdata.bms_buscurrent * 0.001);
			if(curdata.bms_maxcurrent != lastdata.bms_maxcurrent) {
				lv_label_set_text_fmt(display_elements.bms_maxcurrent_label, "MAX I = %1.2f A", curdata.bms_maxcurrent * 0.001);
			}
		}


		lastdata = curdata;
	}

	void init() {
		Serial.printf("Initializing DisplayManager\n");
		TFTManager::init();
		Serial.printf("Initializing LVGL\n");
		initLVGL();
		Serial.printf("Initializing Display Elements\n");
		initDisplayElements();
		Serial.printf("Initialized DisplayManager\n");
	}

	void update(DataManager::car_data_t data) {
		curdata = data;

		updateDisplayElements();

		// Force display refresh with new data
		lv_refr_now(NULL);
	}
}
