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
LV_FONT_DECLARE(font_montserrat_40_compressed);
LV_FONT_DECLARE(big_fault_font);
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
        //DEBUG STYLES
        // Main text style
        lv_style_init(&de.mainTextSmall);
        lv_style_set_bg_color(&de.mainTextSmall, lv_color_black());
        lv_style_set_text_color(&de.mainTextSmall, lv_color_white());
        lv_style_set_radius(&de.mainTextSmall, 2);
        //lv_style_set_text_font(&de.mainTextSmall, &helvetica_bold_18);
        lv_style_set_text_align(&(de.mainTextSmall), LV_TEXT_ALIGN_RIGHT);
        lv_style_set_text_font(&de.mainTextSmall, &font_montserrat_16_compressed);
        

        // Main Large text style
        /*lv_style_init(&de.mainTextLarge);
        lv_style_set_bg_color(&de.mainTextLarge, lv_color_black());
        lv_style_set_text_color(&de.mainTextLarge, lv_color_white());
        lv_style_set_radius(&de.mainTextLarge, 2);
        lv_style_set_text_font(&de.mainTextLarge, &helvetica_bold_13);
        lv_style_set_text_letter_space(&de.mainTextLarge, -6);*/
        lv_style_init(&de.faultText);
        lv_style_set_bg_color(&de.faultText, lv_color_black());
        lv_style_set_text_color(&de.faultText, lv_color_white());
        lv_style_set_radius(&de.faultText, 2);
        lv_style_set_text_align(&(de.faultText), LV_TEXT_ALIGN_LEFT);
        lv_style_set_text_font(&de.faultText, &lv_font_montserrat_12);

        // Fault timecode style
        lv_style_init(&de.faultTimecode);
        lv_style_set_bg_color(&de.faultTimecode, lv_color_black());
        lv_style_set_text_color(&de.faultTimecode, lv_color_white());
        lv_style_set_radius(&de.faultTimecode, 2);
        lv_style_set_text_font(&de.faultTimecode, &helvetica_oblique_13);

        // Rectangle style
        lv_style_init(&(de.rect));
        lv_style_set_bg_color(&(de.rect), lv_color_black());
        lv_style_set_text_color(&(de.rect), lv_color_white());
        //lv_style_set_border_color(&(de.rect), LV_COLOR_MAKE(248, 146, 15));
        lv_style_set_border_color(&(de.rect), lv_color_white());
        lv_style_set_border_width(&(de.rect), 1);
        lv_style_set_pad_top(&(de.rect), 3);
        lv_style_set_pad_bottom(&(de.rect), 0);
        lv_style_set_radius(&(de.rect), 5);
        lv_style_set_text_align(&(de.rect), LV_TEXT_ALIGN_CENTER);
        lv_style_set_text_font(&(de.rect), &font_montserrat_16_compressed);
        
        // Warning/soft fault style
        lv_style_init(&(de.warn));
        lv_style_set_bg_color(&(de.warn), lv_color_black());
        lv_style_set_text_color(&(de.warn), lv_palette_main(LV_PALETTE_YELLOW));
        lv_style_set_border_color(&(de.warn), lv_palette_main(LV_PALETTE_YELLOW));
        
        // Error/hard fault style
        lv_style_init(&(de.error));
        lv_style_set_text_color(&(de.error), lv_palette_main(LV_PALETTE_RED));
        lv_style_set_border_color(&(de.error), lv_palette_main(LV_PALETTE_RED));


        //DRIVE STYLES 

        // Temp text
        lv_style_init(&dr.tempText);
        lv_style_set_text_color(&dr.tempText, lv_color_black());//white for now
        lv_style_set_text_font(&dr.tempText, &helvetica_bold_48);
        lv_style_set_text_letter_space(&dr.tempText, -5); 

        // Temprature init
        lv_style_init(&dr.tempInit);
        lv_style_set_radius(&(dr.tempInit), 0);
        lv_style_set_bg_color(&dr.tempInit, lv_palette_main(LV_PALETTE_GREY));
        // Hot
        lv_style_init(&(dr.hot));
        lv_style_set_bg_color(&dr.hot, EVA_RED);

        // Nominal 
        lv_style_init(&(dr.nominal));
        lv_style_set_bg_color(&dr.nominal, EVA_GREEN);

        // Cold
        lv_style_init(&(dr.cold));
        lv_style_set_bg_color(&dr.cold, EVA_BLUE);
        
        // Middle text style
        lv_style_init(&dr.middleText);
        lv_style_set_bg_color(&dr.middleText, lv_color_black());
        lv_style_set_text_color(&dr.middleText, lv_color_black());
        lv_style_set_radius(&dr.middleText, 2);
        lv_style_set_text_letter_space(&dr.middleText, -5);
        lv_style_set_text_font(&dr.middleText, &eurostileextended_bold_40);


        // BMS text style
    
        lv_style_init(&dr.bmsText);
        lv_style_set_bg_color(&dr.bmsText, lv_color_black());
        lv_style_set_text_color(&dr.bmsText, EVA_ORANGE);
        lv_style_set_radius(&dr.bmsText, 2);
        lv_style_set_text_font(&dr.bmsText, &helvetica_bold_48);


        // BIG FLASHY FAULT STYLE

        //RED BG
        lv_style_init(&(dr.redBlackFault));
        lv_style_set_bg_color(&(dr.redBlackFault), EVA_RED);
        lv_style_set_bg_opa(&(dr.redBlackFault), LV_OPA_COVER);
        lv_style_set_bg_opa(&(dr.redBlackFault), LV_OPA_COVER);
        lv_style_set_text_color(&(dr.redBlackFault), lv_color_black());
        lv_style_set_pad_all(&(dr.redBlackFault), 10);
        lv_style_set_radius(&(dr.redBlackFault), 0);
        lv_style_set_text_align(&(dr.redBlackFault), LV_TEXT_ALIGN_LEFT);
        lv_style_set_text_font(&(dr.redBlackFault), &big_fault_font );
        lv_style_set_text_letter_space(&(dr.redBlackFault), -5);


        //BLACK BG
        lv_style_init(&(dr.blackRedFault));
        lv_style_set_bg_color(&(dr.blackRedFault), lv_color_black());
        lv_style_set_bg_opa(&(dr.blackRedFault), LV_OPA_100);
        lv_style_set_text_color(&(dr.blackRedFault), EVA_RED);
        lv_style_set_pad_all(&(dr.blackRedFault), 10);
        lv_style_set_radius(&(dr.blackRedFault), 0);
        lv_style_set_text_align(&(dr.blackRedFault), LV_TEXT_ALIGN_LEFT);
        lv_style_set_text_font(&(dr.blackRedFault), &big_fault_font);
        lv_style_set_text_letter_space(&(dr.blackRedFault), -5);

        //BLANK BACKGROUND
        lv_style_init(&(dr.noOpacity));
        lv_style_set_bg_opa(&(dr.noOpacity), LV_OPA_0);
    }

    void init() {
        Serial.printf("Initializing DisplayManager\n");
        TFTManager::init();
        Serial.printf("Initializing LVGL\n");
        initLVGL();
        Serial.printf("Initializing Screens\n");
        initStyles();
        
        lv_obj_clean(lv_scr_act());
        screen_debug = ScreenDebug::init(&de);
        screen_drive = ScreenDrive::init(&dr);
        if (active_screen == 0) {
            lv_scr_load(screen_debug);
        } else if (active_screen == 1) {
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
