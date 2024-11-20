#include "display-manager.h"

#include <Arduino.h>


#include "tft-manager.h"
#include "screen-debug.h"
#include "screen-drive.h"
#include "io-manager.h"

#include "lvgl.h"

#define DRAW_BUFFER_SIZE (TFT_SCREEN_PIXELS / 16) //default /10 for 480x800

LV_FONT_DECLARE(font_montserrat_40_compressed);

namespace DisplayManager {

	bool active_screen = 1; // 0 = debug, 1 = drive
	lv_obj_t* screen_debug;
	lv_obj_t* screen_drive;

	lv_disp_draw_buf_t drawbuf;
	lv_color_t drawbuf1[DRAW_BUFFER_SIZE];
	lv_disp_drv_t disp_drv;

	styles_t styles;

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
		// Global style, can be overridden
		lv_style_init(&styles.style);
		lv_style_set_bg_color(&styles.style, lv_color_black());
		lv_style_set_text_color(&styles.style, lv_color_white());
		lv_style_set_radius(&styles.style, 2);
		lv_style_set_text_font(&styles.style, &font_montserrat_40_compressed);
		lv_style_set_border_width(&styles.style, 2);
		lv_style_set_border_color(&styles.style, lv_palette_main(LV_PALETTE_BLUE));

		// Progress bar style
		lv_style_init(&styles.bar_style);
		lv_style_set_border_color(&styles.bar_style, lv_color_white());
		// lv_style_set_border_width(&styles.bar_style, 2);
		lv_style_set_radius(&styles.bar_style, 2);
		lv_style_set_pad_all(&styles.bar_style, 4);

		lv_style_init(&styles.barind_style);
		lv_style_set_radius(&styles.barind_style, 2);
		lv_style_set_pad_all(&styles.barind_style, 4);

		// Fault font style
		lv_style_init(&styles.fault_style);
		lv_style_set_bg_color(&styles.fault_style, lv_color_black());
		lv_style_set_text_color(&styles.fault_style, lv_color_white());
		lv_style_set_text_font(&styles.fault_style, &font_montserrat_16_compressed);
		lv_style_set_radius(&styles.fault_style, 2);

		// Container style
		lv_style_init(&styles.container_style);
		lv_style_set_border_width(&styles.container_style, 2);
		lv_style_set_border_color(&styles.container_style, lv_palette_main(LV_PALETTE_BLUE));
	}

	void init() {
		Serial.printf("Initializing DisplayManager\n");
		TFTManager::init();
		Serial.printf("Initializing LVGL\n");
		initLVGL();
		Serial.printf("Initializing Screens\n");
		initStyles();
		screen_debug = ScreenDebug::init(&styles);
		screen_drive = ScreenDrive::init(&styles);
		if (active_screen == 0) {
			lv_scr_load_anim(screen_debug, LV_SCR_LOAD_ANIM_FADE_IN, 1, 0, true);
		}
		else {
			lv_scr_load_anim(screen_drive, LV_SCR_LOAD_ANIM_FADE_IN, 1, 0, true);
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
				lv_scr_load_anim(screen_debug, LV_SCR_LOAD_ANIM_FADE_IN, 1, 0, true);
			}
			else {
				lv_scr_load_anim(screen_drive, LV_SCR_LOAD_ANIM_FADE_IN, 1, 0, true);
			}

			active_screen = new_screen;
		}
	}
}
