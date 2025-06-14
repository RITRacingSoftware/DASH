#pragma once

#include "lvgl.h"
#include "data-manager.h"

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
        lv_style_t middleText;
        lv_style_t bmsText;
        // Temporary until we have a proper fault implementation
        lv_style_t faultText;
        lv_style_t rect;
        lv_style_t warn;
        lv_style_t error;
    

    } drive_styles_t;
    // F8920F



    void init();
    void update(DataManager::car_data_t data);
    void switchScreens(int new_screen);
}
