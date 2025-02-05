#include "lvgl.h"
#include "display-manager.h"
#include <Arduino.h>
#include "tft-manager.h"
#include "screen-debug.h"
#include "screen-drive.h"
#include "io-manager.h"

#define DRAW_BUFFER_SIZE (TFT_SCREEN_PIXELS / 16) // Adjust buffer size as needed

namespace DisplayManager {

    bool active_screen = 1; // 0 = debug, 1 = drive
    lv_obj_t* screen_debug;
    lv_obj_t* screen_drive;

    lv_draw_buf_t drawbuf;
    lv_color_t drawbuf1[DRAW_BUFFER_SIZE];
    lv_display_t* disp = lv_display_create(TFT_SCREEN_WIDTH, TFT_SCREEN_HEIGHT);

    styles_t styles;

    // Function to flush the display buffer
    void disp_flush(lv_display_t* disp, const lv_area_t* area, lv_color_t* color_p) {
        TFTManager::drawTexturedRect(area->x1, area->x2, area->y1, area->y2, (uint16_t*)color_p);
        lv_disp_flush_ready(disp); // Notify LVGL that flushing is done
    }

    void disp_flush_cb(lv_display_t* disp, const lv_area_t* area, unsigned char* px_map) {
        disp_flush(disp, area, (lv_color_t*)px_map);
    }

    void initLVGL() {
        lv_init();

        lv_display_set_flush_cb(disp, disp_flush_cb);
        lv_display_set_buffers(disp, drawbuf1, NULL, DRAW_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);

        // Set the flush callback
        // lv_display_set_flush_cb(disp, disp_flush);
    }

    void initStyles() {
        // Global style, can be overridden
        lv_style_init(&styles.style);
        lv_style_set_bg_color(&styles.style, lv_color_black());
        lv_style_set_text_color(&styles.style, lv_color_white());
        lv_style_set_radius(&styles.style, 2);
        lv_style_set_text_font(&styles.style, &lv_font_montserrat_40);
        lv_style_set_border_width(&styles.style, 2);
        lv_style_set_border_color(&styles.style, lv_palette_main(LV_PALETTE_BLUE));

        // Progress bar style
        lv_style_init(&styles.bar_style);
        lv_style_set_border_color(&styles.bar_style, lv_color_white());
        lv_style_set_radius(&styles.bar_style, 2);
        lv_style_set_pad_all(&styles.bar_style, 4);

        lv_style_init(&styles.barind_style);
        lv_style_set_radius(&styles.barind_style, 2);
        lv_style_set_pad_all(&styles.barind_style, 4);

        // Fault font style
        lv_style_init(&styles.fault_style);
        lv_style_set_bg_color(&styles.fault_style, lv_color_black());
        lv_style_set_text_color(&styles.fault_style, lv_color_white());
        lv_style_set_text_font(&styles.fault_style, &lv_font_montserrat_16);
        lv_style_set_radius(&styles.fault_style, 2);

        // Container style
        lv_style_init(&styles.container_style);
        lv_style_set_border_width(&styles.container_style, 2);
        lv_style_set_border_color(&styles.container_style, lv_palette_main(LV_PALETTE_BLUE));
    }

    void init() {
        Serial.printf("Initializing DisplayManager\n");
        TFTManager::init(); // Initialize the TFT display
        Serial.printf("Initializing LVGL\n");
        initLVGL(); // Initialize LVGL
        Serial.printf("Initializing Screens\n");
        initStyles(); // Initialize styles
        screen_debug = ScreenDebug::init(&styles); // Initialize debug screen
        screen_drive = ScreenDrive::init(&styles); // Initialize drive screen

        // Load the active screen with a fade-in animation
        if (active_screen == 0) {
            lv_scr_load_anim(screen_debug, LV_SCR_LOAD_ANIM_FADE_IN, 1, 0, true);
        } else {
            lv_scr_load_anim(screen_drive, LV_SCR_LOAD_ANIM_FADE_IN, 1, 0, true);
        }

        Serial.printf("Initialized Screens\n");
        Serial.printf("Initialized DisplayManager\n");
    }

    void update(DataManager::car_data_t data) {
        if (active_screen == 0) {
            ScreenDebug::update(data); // Update debug screen
        } else if (active_screen == 1) {
            ScreenDrive::update(data); // Update drive screen
        }

        // Force display refresh with new data
        lv_refr_now(NULL);
    }

    void switchScreens(int new_screen) {
        if (new_screen != active_screen) {
            if (new_screen == 0) {
                lv_scr_load_anim(screen_debug, LV_SCR_LOAD_ANIM_FADE_IN, 1, 0, true);
            } else {
                lv_scr_load_anim(screen_drive, LV_SCR_LOAD_ANIM_FADE_IN, 1, 0, true);
            }

            active_screen = new_screen; // Update the active screen
        }
    }
}	