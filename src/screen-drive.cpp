#include "screen-drive.h"

#ifdef DASH_TESTING
#include "testing_arduino.h"
#else
#include <Arduino.h>
#endif

#include "data-manager.h"
#include "display-manager.h"

#include "lvgl.h"

namespace ScreenDrive {
    DataManager::car_data_t lastdata;
	DisplayManager::drive_styles_t* styles;
	// The dbc seems to be outdated/not match what the VC is sending, I know this
	// isn't the correct way to fix it but I'm doing it for now to make it match the VC
    const char* VC_STATUS_MESSAGES[] = {
        "NOT READY",
        "INV POWER",
        "PRECHARGING",
        "WAIT",
        "STANDBY",
        "RTD",
        "SHUTDOWN"
    };

    const char* VC_FAULT_MESSAGES[] = {
        "",
        "BMS",
        "APPS A IR",
        "APPS B IR",
        "APPS DISAG",
        "APPS DP",
        "FSSDB LOST",
        "FBPS IR",
        "RBPS IR",
        "STEER IR",
        "RSSDB LOST",
        "RR",
        "RL",
        "FR",
        "FL",
        "PRECH",
        "APPS SOFT DP"
    };

    const char* MCU_STATUS_MESSAGES[] = {
        "DISCONN.",
        "DISABLED",
        "UNLOCKING",
        "ENABLED",
        "READY",
    };

    const char* BMS_FAULT_MESSAGES[] = {
        "SLAVE COMM CELLS",
        "SLAVE COMM TEMPS",
        "SLAVE COMM DRAIN REQUEST",
        "CURRENT SENSOR COMM",
        "OVER CURRENT",
        "CELL VOLTAGE IRRATIONAL",
        "CELL VOLTAGE DIFF",
        "OUT OF JUICE",
        "TEMPERATURE IRRATIONAL",
        "OVER TEMPERATURE",
        "DRAIN FAILURE",
    };

    const char* INVERTER_STATES[] = {
        "NORMAL",
        "ESOFT",
        "EHARD",
        "ELOST",
        "EPSOFT",
        "EPHARD"
    };


	lv_obj_t* screen;

	// Display elements to keep
	struct elements_s {
        lv_obj_t* background_image;

		lv_obj_t* hv_capac_label;
		lv_obj_t* hv_cell_min_label;
		lv_obj_t* current_label;

		lv_obj_t* max_batt_temp_label;
		lv_obj_t* avg_batt_temp_label;
		lv_obj_t* batt_temp_bg;
        lv_obj_t* batt_temp_divider;

		lv_obj_t* charge_label;
		lv_obj_t* mph_label;
		lv_obj_t* status_bar;

        lv_obj_t* faults_container;
		lv_obj_t* faults_text; //temporary until we have a proper fault implementation
		lv_obj_t* vc_status;

	} elements;

	lv_style_t temp_cold_style;
	lv_style_t temp_optimal_style;
	lv_style_t temp_hot_style;
	lv_style_t text_style;

    bool fault_color = false; // Used to toggle the fault color on the status bar

	lv_obj_t* init(DisplayManager::drive_styles_t* dr) {
		Serial.printf("Initializing Drive Screen\n");
        styles = dr;
        //Styles
        lv_style_init(&temp_cold_style);
        lv_style_set_bg_color(&temp_cold_style, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_init(&temp_optimal_style);
        lv_style_set_bg_color(&temp_optimal_style, lv_palette_main(LV_PALETTE_GREEN));
        lv_style_init(&temp_hot_style);
        lv_style_set_bg_color(&temp_hot_style, lv_palette_main(LV_PALETTE_RED));


        screen = lv_obj_create(NULL);
        // lv_obj_add_style(screen, &styles->style, LV_PART_MAIN);

		
		
        // Background image
        elements.background_image = lv_img_create(screen);
        lv_img_set_src(elements.background_image, &spriteBGDrive);
        lv_obj_align(elements.background_image, LV_ALIGN_CENTER,0, 0);
        lv_obj_set_size(elements.background_image, LV_SIZE_CONTENT,LV_SIZE_CONTENT);
		
		
		
		//
		// Left Column (Levels)
		//
		elements.hv_capac_label = lv_label_create(screen);
        lv_obj_align(elements.hv_capac_label, LV_ALIGN_TOP_LEFT, 10, 206);
        lv_label_set_text(elements.hv_capac_label, "?");
		lv_obj_add_style(elements.hv_capac_label, &dr->bmsText, LV_PART_MAIN);
		
		elements.hv_cell_min_label = lv_label_create(screen); // MINIMUM CELL VOLTAGE FOR NOW
		lv_obj_align(elements.hv_cell_min_label, LV_ALIGN_TOP_LEFT, 10, 302);
		lv_label_set_text(elements.hv_cell_min_label, "?");
		lv_obj_add_style(elements.hv_cell_min_label, &dr->bmsText, LV_PART_MAIN);
		
		elements.current_label = lv_label_create(screen);
		lv_obj_align(elements.current_label, LV_ALIGN_TOP_LEFT, 10, 399);
		lv_label_set_text(elements.current_label, "?");
		lv_obj_add_style(elements.current_label, &dr->bmsText, LV_PART_MAIN);
		
		//
		// Center Column
		//
		int width_center = 137;
		elements.charge_label = lv_label_create(screen);
		lv_obj_align(elements.charge_label, LV_ALIGN_TOP_LEFT, 496, 192);
		lv_label_set_text(elements.charge_label, "?");
		lv_obj_add_style(elements.charge_label, &dr->middleText, LV_PART_MAIN);
		lv_obj_set_width(elements.charge_label, width_center);
		lv_obj_set_style_text_align(elements.charge_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
		
		elements.mph_label = lv_label_create(screen);
		lv_obj_align(elements.mph_label, LV_ALIGN_TOP_LEFT, 496, 266);
		lv_label_set_text(elements.mph_label, "?");
		lv_obj_add_style(elements.mph_label, &dr->middleText, LV_PART_MAIN);
		lv_obj_set_width(elements.mph_label, width_center);
		lv_obj_set_style_text_align(elements.mph_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
		
		elements.status_bar = lv_img_create(screen);
		lv_img_set_src(elements.status_bar, &spriteIndexedSBRTD);
		lv_obj_align(elements.status_bar, LV_ALIGN_TOP_LEFT, 496, 325);
		lv_obj_set_size(elements.status_bar, 137, 135);
		// lv_obj_add_flag(elements.status_bar, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(elements.status_bar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_bg_color(elements.status_bar, lv_color_white(), LV_PART_MAIN);
		
		//
		// Right Column (Temps)
		//
		elements.batt_temp_bg = lv_obj_create(screen);
		lv_obj_set_size(elements.batt_temp_bg, 147, 248);
		lv_obj_align(elements.batt_temp_bg, LV_ALIGN_TOP_LEFT, 643, 212);
        lv_obj_set_style_border_width(elements.batt_temp_bg, 0, LV_PART_MAIN);
        lv_obj_set_style_radius(elements.batt_temp_bg, 0, LV_PART_MAIN);
        
        elements.batt_temp_divider = lv_obj_create(screen);
        lv_obj_set_size(elements.batt_temp_divider, 147 - 20, 2);
		lv_obj_align(elements.batt_temp_divider, LV_ALIGN_TOP_LEFT, 643 + 10, 212 + 124);
        lv_obj_set_style_bg_color(elements.batt_temp_divider, lv_color_make(0, 0, 0), LV_PART_MAIN);
        lv_obj_set_style_border_width(elements.batt_temp_divider, 0, LV_PART_MAIN);
		
		int width_temp = 151;
		elements.max_batt_temp_label = lv_label_create(screen);
		lv_obj_align(elements.max_batt_temp_label, LV_ALIGN_TOP_LEFT, 717 - (width_temp / 2), 256);
		lv_label_set_text(elements.max_batt_temp_label, "??");
		lv_obj_add_style(elements.max_batt_temp_label, &dr->tempText, LV_PART_MAIN);
		lv_obj_set_width(elements.max_batt_temp_label, width_temp);
		lv_obj_set_style_text_align(elements.max_batt_temp_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
        // lv_obj_set_style_border_color(elements.max_batt_temp_label, lv_color_black(), LV_PART_MAIN);
        // lv_obj_set_style_border_width(elements.max_batt_temp_label, 2, LV_PART_MAIN);
        
		elements.avg_batt_temp_label = lv_label_create(screen);
		lv_obj_align(elements.avg_batt_temp_label, LV_ALIGN_TOP_LEFT, 717 - (width_temp / 2), 380);
		lv_label_set_text(elements.avg_batt_temp_label, "??");
		lv_obj_add_style(elements.avg_batt_temp_label, &dr->tempText, LV_PART_MAIN);
		lv_obj_set_width(elements.avg_batt_temp_label, width_temp);
		lv_obj_set_style_text_align(elements.avg_batt_temp_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
		
		
		//
		// Faults
		//
        elements.faults_container = lv_obj_create(screen);
        lv_obj_set_size(elements.faults_container, 780, 150);
        lv_obj_align(elements.faults_container, LV_ALIGN_TOP_LEFT, 10, 10);
        lv_obj_set_style_radius(elements.faults_container, 40, LV_PART_MAIN);
        lv_obj_set_style_border_width(elements.faults_container, 0, LV_PART_MAIN);
		
		elements.faults_text = lv_label_create(elements.faults_container);
		lv_obj_remove_style_all(elements.faults_text);
        lv_obj_add_style(elements.faults_text, &(dr->redBlackFault), LV_PART_SELECTED);
		lv_obj_add_style(elements.faults_text, &(dr->redBlackFault), LV_PART_MAIN);
        lv_obj_set_size(elements.faults_text, 780, LV_SIZE_CONTENT);
        lv_obj_align(elements.faults_text, LV_ALIGN_CENTER, 0, 10);
        lv_label_set_long_mode(elements.faults_text, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_style_radius(elements.faults_text, 60, LV_PART_MAIN);
		lv_obj_set_scrollbar_mode(elements.faults_text,LV_SCROLLBAR_MODE_OFF);
		lv_obj_set_scrollbar_mode(elements.faults_container,LV_SCROLLBAR_MODE_OFF);




		lv_obj_set_scrollbar_mode(screen,LV_SCROLLBAR_MODE_OFF);




		
		memset(&lastdata, 0xff, sizeof(lastdata));
        
		Serial.printf("Initialized Drive Screen\n");
		return screen;
	}
	
	void update(DataManager::car_data_t data) {
        //
        // Left Column (Levels)
        //
		int min_pack_voltage = 408; // Minimum pack voltage
		int max_pack_voltage = 591.6; // Maximum pack voltage
		
		if (data.bms_packvoltage != lastdata.bms_packvoltage) {
			lv_label_set_text_fmt(elements.hv_capac_label, "%2.3f H", (data.bms_packvoltage * 11.58f) / 1000);
			lv_label_set_text_fmt(elements.charge_label, "%2.0f%%", (data.bms_packvoltage - min_pack_voltage)/(max_pack_voltage - min_pack_voltage) * 100.0f);
		}

		if (data.bms_cellvoltages_min != lastdata.bms_cellvoltages_min) {
			lv_label_set_text_fmt(elements.hv_cell_min_label, "%2.1f V", data.bms_cellvoltages_min);
		}

		if (data.bms_maxcurrent != lastdata.bms_maxcurrent) {
			lv_label_set_text_fmt(elements.current_label, "%2.1f A", data.bms_maxcurrent);
		}

        //
        // Center Column
        //
        if (data.vel != lastdata.vel) {
        	lv_label_set_text_fmt(elements.mph_label, "%2.0f", data.vel);
        }

        if(data.vc_status != lastdata.vc_status) {
            switch (data.vc_status) {
                case 0:
                    lv_img_set_src(elements.status_bar, &spriteIndexedSBOff);
                    break;
                case 1:
                    lv_img_set_src(elements.status_bar, &spriteIndexedSBInv);
                    break;
                case 2:
                    lv_img_set_src(elements.status_bar, &spriteIndexedSBPcrg);
                    break;
                case 3:
                    lv_img_set_src(elements.status_bar, &spriteIndexedSBWait);
                    break;
                case 4:
                    lv_img_set_src(elements.status_bar, &spriteIndexedSBStby);
                    break;
                case 5:
                    lv_img_set_src(elements.status_bar, &spriteIndexedSBRTD);
                    break;
                case 6:
                    lv_img_set_src(elements.status_bar, &spriteIndexedSBShut);
                    break;
            }
        }

        //
        // Right Column (Temps)
        //
		if (data.bms_maxtemp != lastdata.bms_maxtemp) {
			lv_label_set_text_fmt(elements.max_batt_temp_label, "%2.0f C", data.bms_maxtemp);
			if (data.bms_maxtemp > 90) {
				lv_obj_add_style(elements.batt_temp_bg, &styles->hot, LV_PART_MAIN);
			}
			else if (data.bms_maxtemp > 45) {
				lv_obj_add_style(elements.batt_temp_bg, &styles->nominal, LV_PART_MAIN);
			}
            else {
                lv_obj_add_style(elements.batt_temp_bg, &styles->cold, LV_PART_MAIN);
            }
		}

		if (data.bms_avgtemp != lastdata.bms_avgtemp) {
			lv_label_set_text_fmt(elements.avg_batt_temp_label, "%2.0f C", data.bms_avgtemp);
			if (data.bms_avgtemp < 50) {
				// lv_obj_add_style(elements.avg_batt_temp_label, &temp_cold_style, LV_PART_MAIN);
			}
			else if (data.bms_avgtemp < 70) {
				// lv_obj_add_style(elements.avg_batt_temp_label, &temp_optimal_style, LV_PART_MAIN);
			}
			else {
				// lv_obj_add_style(elements.avg_batt_temp_label, &temp_hot_style, LV_PART_MAIN);
			}
		}

        //
        // Faults
        //
		// if (data.vc_faultvector != lastdata.vc_faultvector) {
        //     uint8_t vc_faultnum = 0;

        //     // Loop over possible VC faults
        //     for(int i = 0; i < 17; i++) {
        //         bool faulted = (data.vc_faultvector >> i) & 1;
        //         if(faulted) {
        //             lv_label_set_text(elements.faults_text, VC_FAULT_MESSAGES[i]);
        //             vc_faultnum++;
    
        //         }
        //     }
        // }
        
        if (data.bms_faultvector != lastdata.bms_faultvector) {
            // If any fault message changes, we must update them all...
            
            uint8_t bms_faultnum = 0;
            lv_label_set_text(elements.faults_text, "");


            // Loop over possible BMS faults
            for(int i = 0; i < 11; i++) {
                bool faulted = (data.bms_faultvector >> i) & 1;
                if(faulted) {
                    lv_label_set_text(elements.faults_text, BMS_FAULT_MESSAGES[i]);
                    bms_faultnum++;
                }
            }

		}
        
        unsigned int speed = 800;
        lv_coord_t scrollx;
		if(lv_tick_get() % speed >= 0 && lv_tick_get() % speed <= speed / 2 && fault_color == false) {
            scrollx = lv_obj_get_scroll_x(elements.faults_text);
			lv_obj_remove_style(elements.faults_text, &(styles->redBlackFault), LV_PART_MAIN);
            lv_obj_remove_style(elements.faults_container, &(styles->redBlackFault), LV_PART_MAIN);
			lv_obj_add_style(elements.faults_text, &(styles->blackRedFault), LV_PART_MAIN);
			lv_obj_add_style(elements.faults_container, &(styles->blackRedFault), LV_PART_MAIN);
            lv_obj_scroll_to_x(elements.faults_text, scrollx - 50, LV_ANIM_OFF);
            
            fault_color = true;
		}
		else if (lv_tick_get() % speed >= speed / 2 && lv_tick_get() % speed <= speed && fault_color == true) {
            scrollx = lv_obj_get_scroll_x(elements.faults_text);
			lv_obj_remove_style(elements.faults_text, &(styles->blackRedFault), LV_PART_MAIN);
			lv_obj_remove_style(elements.faults_container, &(styles->blackRedFault), LV_PART_MAIN);
			lv_obj_add_style(elements.faults_text, &(styles->redBlackFault), LV_PART_MAIN);
			lv_obj_add_style(elements.faults_container, &(styles->redBlackFault), LV_PART_MAIN);
            lv_obj_scroll_to_x(elements.faults_text, scrollx - 50, LV_ANIM_OFF);

            fault_color = false;
		}
		
		lastdata = data;
	}
}