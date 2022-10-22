#include "display-manager.h"

#include <Arduino.h>

#include "../lib/lvgl/lvgl.h"
#include "tft-manager.h"

#define DRAW_BUFFER_SIZE (TFT_SCREEN_PIXELS / 10)

namespace DisplayManager {
	struct display_elements_s {
		lv_obj_t* rpmbar;
		lv_obj_t* rpmlabel;
		lv_obj_t* chargebar;
		lv_obj_t* chargelabel;
		lv_obj_t* currentlabel;
		lv_obj_t* cellvoltagelabel;
	} display_elements;

	lv_disp_draw_buf_t drawbuf;
	lv_color_t drawbuf1[DRAW_BUFFER_SIZE];
	lv_disp_drv_t disp_drv;

	DataManager::car_data_t curdata;
	DataManager::car_data_t lastdata;

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

	void initDisplayElements() {
		// Global style, can be overridden
		lv_style_init(&style);
		lv_style_set_bg_color(&style, lv_color_black());
		lv_style_set_text_color(&style, lv_color_white());
		lv_obj_add_style(lv_scr_act(), &style, LV_PART_MAIN);

		// Progress bar style
		lv_style_init(&barstyle);
		lv_style_set_border_color(&barstyle, lv_palette_main(LV_PALETTE_BLUE));
		lv_style_set_border_width(&barstyle, 2);
		lv_style_set_radius(&barstyle, 1);
		lv_style_set_pad_all(&barstyle, 4);

		// RPM Bar
		display_elements.rpmbar = lv_bar_create(lv_scr_act());
		lv_bar_set_range(display_elements.rpmbar, 0, 5000);
		lv_obj_set_width(display_elements.rpmbar, 32);
		lv_obj_set_height(display_elements.rpmbar, 100);
		lv_obj_align(display_elements.rpmbar, LV_ALIGN_CENTER, -100, 0);
		lv_obj_add_style(display_elements.rpmbar, &barstyle, 0);
		lv_obj_add_style(display_elements.rpmbar, &barstyle, LV_PART_INDICATOR);
		// RPM Bar Label
		display_elements.rpmlabel = lv_label_create(lv_scr_act());
		lv_label_set_text(display_elements.rpmlabel, "Motor RPM = ??? RPM");
		lv_obj_align_to(display_elements.rpmlabel, display_elements.rpmbar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

		// SOC Bar
		display_elements.chargebar = lv_bar_create(lv_scr_act());
		lv_bar_set_range(display_elements.chargebar, 0, 100);
		lv_obj_set_width(display_elements.chargebar, 32);
		lv_obj_set_height(display_elements.chargebar, 100);
		lv_obj_align(display_elements.chargebar, LV_ALIGN_CENTER, +100, 0);
		lv_obj_add_style(display_elements.chargebar, &barstyle, 0);
		lv_obj_add_style(display_elements.chargebar, &barstyle, LV_PART_INDICATOR);
		// SOC Bar Label
		display_elements.chargelabel = lv_label_create(lv_scr_act());
		lv_label_set_text(display_elements.chargelabel, "Battery SOC = ??? %");
		lv_obj_align_to(display_elements.chargelabel, display_elements.chargebar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

		// Current Label
		display_elements.currentlabel = lv_label_create(lv_scr_act());
		lv_obj_align(display_elements.currentlabel, LV_ALIGN_CENTER, 0, 0);
		lv_label_set_text(display_elements.currentlabel, "Current = ??? A");

		// Cell Voltages Label
		display_elements.cellvoltagelabel = lv_label_create(lv_scr_act());
		lv_obj_align(display_elements.cellvoltagelabel, LV_ALIGN_CENTER, 0, -40);
		lv_label_set_text(display_elements.cellvoltagelabel, "Voltage = ?? - ?? V");
	}

	void updateDisplayElements() {
		if(curdata.mcu_motorrpm != lastdata.mcu_motorrpm) {
			lv_bar_set_value(display_elements.rpmbar, curdata.mcu_motorrpm, LV_ANIM_OFF);
			lv_label_set_text_fmt(display_elements.rpmlabel, "Motor RPM = %d RPM", curdata.mcu_motorrpm);
		}
		if(curdata.bms_soc != lastdata.bms_soc) {
			lv_bar_set_value(display_elements.chargebar, curdata.bms_soc, LV_ANIM_OFF);
			lv_label_set_text_fmt(display_elements.chargelabel, "Battery SOC = %d%%", curdata.bms_soc);
		}
		if(curdata.bms_buscurrent != lastdata.bms_buscurrent) {
			lv_label_set_text_fmt(display_elements.currentlabel, "Current = %3.3f A", curdata.bms_buscurrent * 0.001);
		}
		if(curdata.bms_cellvoltages_min != lastdata.bms_cellvoltages_min ||
				curdata.bms_cellvoltages_max != lastdata.bms_cellvoltages_max) {
			lv_label_set_text_fmt(display_elements.cellvoltagelabel, "Cells = %1.2f - %1.2f V",
				curdata.bms_cellvoltages_min * 0.01, curdata.bms_cellvoltages_max * 0.01);
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
