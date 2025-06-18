#pragma once

#include "lvgl.h"
#include "data-manager.h"


static const lv_color_t EVA_ORANGE = lv_color_hex(0xF8920F);
static const lv_color_t EVA_GREEN = lv_color_hex(0x5AFEA3);
static const lv_color_t EVA_MEDIUM_GREEN = lv_color_hex(0x4A956B);
static const lv_color_t EVA_BLUE = lv_color_hex(0x5CB4EB);
static const lv_color_t EVA_RED = lv_color_hex(0xC62B2B);
static const lv_color_t EVA_MEDIUM_RED = lv_color_hex(0x912020);



namespace DisplayManager {
    typedef struct debug_styles_s {
        lv_style_t mainTextSmall;
        lv_style_t mainTextLarge;
        lv_style_t faultTimecode;
        lv_style_t faultText;
        lv_style_t rect;
        lv_style_t warn;
        lv_style_t error;
    } debug_styles_t;

    typedef struct drive_styles_s {
        lv_style_t tempText;
        lv_style_t tempInit;
        lv_style_t middleText;
        lv_style_t bmsText;
        lv_style_t redBlackFault;
        lv_style_t blackRedFault;
        lv_style_t hot;
        lv_style_t nominal;
        lv_style_t cold;
        lv_style_t noOpacity;

    } drive_styles_t;

    typedef struct endurance_styles_s {
        lv_style_t text;
        lv_style_t small;
        lv_style_t bar;
    } endurance_styles_t;
    // F8920F



    void init();
    void update(DataManager::car_data_t data);
    void switchScreens(int new_screen);
}
