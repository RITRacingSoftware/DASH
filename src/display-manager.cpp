#include "display-manager.h"

#include "../lib/lvgl/lvgl.h"
#include "tft-manager.h"

#define DRAW_BUFFER_SIZE (TFT_SCREEN_PIXELS / 10)

namespace DisplayManager {
	lv_disp_draw_buf_t drawbuf;
	lv_color_t drawbuf1[DRAW_BUFFER_SIZE];
	lv_disp_drv_t disp_drv;
	lv_style_t style;

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

		lv_style_init(&style);
		lv_style_set_bg_color(&style, lv_color_black());
		lv_style_set_text_color(&style, lv_color_white());
		lv_obj_add_style(lv_scr_act(), &style, LV_PART_MAIN);
	}

	void init() {
		TFTManager::init();
		initLVGL();
	}

	void destroy() {
		TFTManager::destroy();
	}

	lv_style_t* getStyle() {
		return &style;
	}
}
