#include <Arduino.h>

#include "tft-manager.h"
#include "../lib/lvgl/lvgl.h"

#define TARGET_LOOP_TIME_MS 10
#define DRAW_BUFFER_SIZE (TFT_SCREEN_PIXELS / 10)

TFTManager man;

lv_disp_draw_buf_t drawbuf;
lv_color_t drawbuf1[DRAW_BUFFER_SIZE];
lv_disp_drv_t disp_drv;
lv_style_t style;
lv_obj_t* label1;
uint16_t color = 0xffff;

void disp_flush(lv_disp_drv_t* disp, const lv_area_t* area, lv_color_t* color_p) {
	man.drawTexturedRect(area->x1, area->x2, area->y1, area->y2, (uint16_t*) color_p);

	lv_disp_flush_ready(disp);
}

uint32_t i = 0;
uint64_t starttime;

void label_updater(void* p) {
	lv_label_set_text_fmt(label1, "i = %d", i);
}

void setup()
{
	Serial.begin(9600);
	Serial.printf("Starting!\n");

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

	label1 = lv_label_create(lv_scr_act());
	lv_label_set_text(label1, "Testing");
	lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);
	lv_obj_add_style(label1, &style, LV_PART_MAIN);

	lv_timer_create(label_updater, 100, NULL);

	starttime = millis();
}

void loop()
{
	Serial.print("Update\n");

	lv_timer_handler();
	delay(5);

	i = (millis() - starttime) / 1000;
}
