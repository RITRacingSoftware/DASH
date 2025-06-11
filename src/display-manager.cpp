#include "display-manager.h"

#ifdef DASH_TESTING
#include "testing_arduino.h"
#else
#include <Arduino.h>
#endif


#include "tft-manager.h"
#include "screen-debug.h"
#include "screen-drive.h"


#include "lvgl.h"
#include "config.h"

#define DRAW_BUFFER_SIZE (TFT_SCREEN_PIXELS / 16) //default /10 for 480x800

LV_FONT_DECLARE(helvetica_bold_13);
LV_FONT_DECLARE(helvetica_bold_18);
LV_FONT_DECLARE(helvetica_bold_48);
LV_FONT_DECLARE(helvetica_oblique_13);
LV_FONT_DECLARE(eurostileextended_bold_40);

namespace DisplayManager {


	int active_screen = 0; // 0 = debug, 1 = drive
	lv_obj_t* screen_debug;
	lv_obj_t* screen_drive;

	lv_disp_draw_buf_t drawbuf;
	lv_color_t drawbuf1[DRAW_BUFFER_SIZE];
	lv_disp_drv_t disp_drv;

	debug_styles_t de;
	drive_styles_t dr;

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

	void initStyles() {
		if(active_screen == 0) {
			//DEBUG STYLES
			// Main text style
			lv_style_init(&de.mainTextSmall);
			lv_style_set_bg_color(&de.mainTextSmall, lv_color_black());
			lv_style_set_text_color(&de.mainTextSmall, lv_color_white());
			lv_style_set_radius(&de.mainTextSmall, 2);
			lv_style_set_text_font(&de.mainTextSmall, &helvetica_bold_18);
			

			// Main Large text style
			lv_style_init(&de.mainTextLarge);
			lv_style_set_bg_color(&de.mainTextLarge, lv_color_black());
			lv_style_set_text_color(&de.mainTextLarge, lv_color_white());
			lv_style_set_radius(&de.mainTextLarge, 2);
			lv_style_set_text_font(&de.mainTextLarge, &helvetica_bold_13);
			lv_style_set_text_letter_space(&de.mainTextLarge, -6);

			// Fault timecode style
			lv_style_init(&de.faultTimecode);
			lv_style_set_bg_color(&de.faultTimecode, lv_color_black());
			lv_style_set_text_color(&de.faultTimecode, lv_color_white());
			lv_style_set_radius(&de.faultTimecode, 2);
			lv_style_set_text_font(&de.faultTimecode, &helvetica_oblique_13);


		}
		else if(active_screen == 1) {

			//DRIVE STYLES 
			// Temperature text style
			lv_style_init(&dr.tempText);
			lv_style_set_bg_color(&dr.tempText, lv_color_black());
			lv_style_set_text_color(&dr.tempText, lv_color_white());
			lv_style_set_radius(&dr.tempText, 2);
			lv_style_set_text_font(&dr.tempText, &helvetica_bold_48);
			lv_style_set_text_letter_space(&dr.tempText, -5);

			// Middle text style
			lv_style_init(&dr.middleText);
			lv_style_set_bg_color(&dr.middleText, lv_color_black());
			lv_style_set_text_color(&dr.middleText, lv_color_white());
			lv_style_set_radius(&dr.middleText, 2);
			lv_style_set_text_font(&dr.middleText, &eurostileextended_bold_40);


			// BMS text style
			lv_style_init(&dr.bmsText);
			lv_style_set_bg_color(&dr.bmsText, lv_color_black());
			lv_style_set_text_color(&dr.bmsText, lv_color_white());
			lv_style_set_radius(&dr.bmsText, 2);
			lv_style_set_text_font(&dr.bmsText, &helvetica_bold_48);
			lv_style_set_text_letter_space(&dr.bmsText, -5);
		}

	}

	void init() {
		Serial.printf("Initializing DisplayManager\n");
		TFTManager::init();
		Serial.printf("Initializing LVGL\n");
		initLVGL();
		Serial.printf("Initializing Screens\n");
		initStyles();
		
	
		if (active_screen == 0) {
			 
			screen_debug = ScreenDebug::init(&de);
			lv_scr_load(screen_debug);
		}
		else if (active_screen == 1) {
			screen_drive = ScreenDrive::init(&dr);
			lv_scr_load(screen_drive);
		}

		Serial.printf("Initialized Screens\n");
		Serial.printf("Initialized DisplayManager\n");
	}

	void update(DataManager::car_data_t data) {
		if(active_screen == 0) {
			
			ScreenDebug::update(data);
		}
		else if(active_screen == 1) {
			ScreenDrive::update(data);
		}

		// Force display refresh with new data
		lv_refr_now(NULL);
	}

	void switchScreens(int new_screen) {
		if(new_screen != active_screen) {
			if(new_screen == 0) {
				lv_obj_clean(lv_scr_act());
				lv_scr_load(screen_debug);
			}
			else if (new_screen == 1){
				lv_obj_clean(lv_scr_act());
				lv_scr_load(screen_drive);
			}

			active_screen = new_screen;
		}
	}
}
