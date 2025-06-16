#include "screen-drive.h"

#ifdef DASH_TESTING
#include "testing_arduino.h"
#else
#include <Arduino.h>
#endif

#include "data-manager.h"
#include "display-manager.h"
#include <algorithm>

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
        lv_obj_t* hv_capac_bar;
		lv_obj_t* hv_cell_min_label;
        lv_obj_t* hv_cell_min_bar;
		lv_obj_t* current_label;
        lv_obj_t* current_bar;

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

        lv_obj_t* pack_blink;
        lv_obj_t* cell_blink;
	} elements;

	lv_style_t temp_cold_style;
	lv_style_t temp_optimal_style;
	lv_style_t temp_hot_style;
	lv_style_t text_style;

    bool fault_color = false; // Used to toggle the fault color on the status bar

    // Linear interpolation between points a, b, and c where a outputs 0, b outputs 0.5, and c outputs 1
    float lerp2(float a, float b, float c, float t) {
        if (t <= a) return 0.0f;
        if (t >= c) return 1.0f;
        if (t <= b) {
            return (t - a) / (b - a) * 0.5f;
        } else {
            return 0.5f + (t - b) / (c - b) * 0.5f;
        }
    }

    // Outputs a value between a and b for inputs between 0 and 0.5, and b and c for inputs between 0.5 and 1
    float invlerp2(float a, float b, float c, float t) {
        if (t <= 0.0f) return a;
        if (t >= 1.0f) return c;
        if (t <= 0.5f) {
            return a + (b - a) * (t / 0.5f);
        } else {
            return b + (c - b) * ((t - 0.5f) / 0.5f);
        }
    }

    // Calculate the segment index based on the danger index and the data value given bounds and nominal value and position
    int calculateSegment(int nom_index, int total_segments, float min, float nom, float max, float data) {
        if (data <= min) {
            return 0; // Below minimum, return first segment
        } else if (data >= max) {
            return total_segments - 1; // Above maximum, return last segment
        } else {
            float progress = lerp2(min, nom, max, data); // Normalize data to [0, 1]
            float segment_progress = invlerp2(0, nom_index, total_segments - 1, progress); // Map to segment index
            return int(segment_progress); // Return the segment index
        }
    }

    void setBarSegOpacity(lv_obj_t* bar, int progress) {
        int total_segments = lv_obj_get_child_cnt(bar);
        for (int i = 0; i < total_segments; i++) {
            lv_obj_t* segment = lv_obj_get_child(bar, i);
            if (i < progress) {
                lv_obj_set_style_bg_opa(segment, LV_OPA_COVER, LV_PART_MAIN);
            } else {
                lv_obj_set_style_bg_opa(segment, 0, LV_PART_MAIN);
            }
        }
    }

    void setBarSegOpacity(lv_obj_t* bar, float progress, int danger_index) {
        int total_segments = lv_obj_get_child_cnt(bar);
        int filled_segments = invlerp2(0, danger_index, total_segments,(int)(total_segments * progress));
        for (int i = 0; i < total_segments; i++) {
            lv_obj_t* segment = lv_obj_get_child(bar, i);
            if (i < filled_segments) {
                lv_obj_set_style_bg_opa(segment, LV_OPA_COVER, LV_PART_MAIN);
            } else {
                lv_obj_set_style_bg_opa(segment, 0, LV_PART_MAIN);
            }
        }
    }

    void setBarSegOpacity(lv_obj_t* bar, float progress) {
        for (int i = 0; i < lv_obj_get_child_cnt(bar); i++) {
            if (i < (int)(progress * lv_obj_get_child_cnt(bar))) {
                lv_obj_t* segment = lv_obj_get_child(bar, i);
                lv_obj_set_style_bg_opa(segment, LV_OPA_COVER, LV_PART_MAIN);
            } else {
                lv_obj_t* segment = lv_obj_get_child(bar, i);
                lv_obj_set_style_bg_opa(segment, 0, LV_PART_MAIN);
            }
        }
    }

    void setBarSegOpacityCentered(lv_obj_t* bar, float progress) {
        int total_segments = lv_obj_get_child_cnt(bar);
        int centered_index = total_segments / 2;
        int start_index = std::min(centered_index, (int)(progress * total_segments) - 1);
        int end_index = std::max(centered_index, (int)(progress * total_segments));
        
        for (int i = 0; i < total_segments; i++) {
            if (i < end_index && i >= start_index) {
                lv_obj_t* segment = lv_obj_get_child(bar, i);
                lv_obj_set_style_bg_opa(segment, LV_OPA_COVER, LV_PART_MAIN);
            } else {
                lv_obj_t* segment = lv_obj_get_child(bar, i);
                lv_obj_set_style_bg_opa(segment, 0, LV_PART_MAIN);
            }
        }
    }


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

        elements.hv_capac_bar = lv_obj_create(screen);
        lv_obj_set_size(elements.hv_capac_bar, 281, 86.67);
        lv_obj_align(elements.hv_capac_bar, LV_ALIGN_TOP_LEFT, 205, 180);
        lv_obj_set_style_bg_color(elements.hv_capac_bar, lv_color_make(0, 0, 0), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(elements.hv_capac_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(elements.hv_capac_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_all(elements.hv_capac_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_gap(elements.hv_capac_bar, 3, LV_PART_MAIN);
        lv_obj_set_layout(elements.hv_capac_bar, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(elements.hv_capac_bar, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(elements.hv_capac_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_radius(elements.hv_capac_bar, 0, LV_PART_MAIN);

        elements.hv_cell_min_bar = lv_obj_create(screen);
        lv_obj_set_size(elements.hv_cell_min_bar, 281, 86.67);
        lv_obj_align(elements.hv_cell_min_bar, LV_ALIGN_TOP_LEFT, 205, 180 + 106.67 - 10);
        lv_obj_set_style_bg_color(elements.hv_cell_min_bar, lv_color_make(0, 0, 0), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(elements.hv_cell_min_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(elements.hv_cell_min_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_all(elements.hv_cell_min_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_gap(elements.hv_cell_min_bar, 3, LV_PART_MAIN);
        lv_obj_set_layout(elements.hv_cell_min_bar, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(elements.hv_cell_min_bar, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(elements.hv_cell_min_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_radius(elements.hv_cell_min_bar, 0, LV_PART_MAIN);

        elements.current_bar = lv_obj_create(screen);
        lv_obj_set_size(elements.current_bar, 281, 86.67);
        lv_obj_align(elements.current_bar, LV_ALIGN_TOP_LEFT, 205, 180 + 203.33 - 10);
        lv_obj_set_style_bg_color(elements.current_bar, lv_color_make(0, 0, 0), LV_PART_MAIN);
        lv_obj_set_style_bg_opa(elements.current_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(elements.current_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_all(elements.current_bar, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_gap(elements.current_bar, 3, LV_PART_MAIN);
        lv_obj_set_layout(elements.current_bar, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(elements.current_bar, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(elements.current_bar, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_radius(elements.current_bar, 0, LV_PART_MAIN);
        
        int segment_count = 10;
        for (int i = 0; i < segment_count; i++) {
            lv_obj_t* segment = lv_obj_create(elements.hv_capac_bar);
            lv_obj_set_style_pad_all(segment, 0, LV_PART_MAIN);
            lv_obj_set_size(segment, 10, lv_pct(100));
            lv_obj_set_style_radius(segment, 2, LV_PART_MAIN);
            // lv_obj_set_size(segment, lv_pct(100 / segment_count), lv_pct(100));
            // lv_obj_align(segment, LV_ALIGN_TOP_LEFT, lv_pct(100 / count) * i, 0);
            if (i < 2) {
                lv_obj_set_style_bg_color(segment, EVA_RED, LV_PART_MAIN);
            } else {
                lv_obj_set_style_bg_color(segment, EVA_GREEN, LV_PART_MAIN);
            }
            lv_obj_set_style_border_width(segment, 0, LV_PART_MAIN);
            lv_obj_set_flex_grow(segment, 1);
        }

        lv_obj_t* hv_capac_bar_delimiter = lv_obj_create(screen);
        lv_obj_set_size(hv_capac_bar_delimiter, 4, 50);
        lv_obj_set_style_border_width(hv_capac_bar_delimiter, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_color(hv_capac_bar_delimiter, lv_color_make(0, 0, 0), LV_PART_MAIN);
        lv_obj_set_style_radius(hv_capac_bar_delimiter, 2, LV_PART_MAIN);
        lv_obj_align(hv_capac_bar_delimiter, LV_ALIGN_TOP_LEFT, 205 - 2, 180 + 18.38);
        
        for (int i = 0; i < segment_count; i++) {
            lv_obj_t* segment = lv_obj_create(elements.hv_cell_min_bar);
            lv_obj_set_style_pad_all(segment, 0, LV_PART_MAIN);
            lv_obj_set_size(segment, 10, lv_pct(100));
            lv_obj_set_style_radius(segment, 2, LV_PART_MAIN);
            // lv_obj_set_size(segment, lv_pct(100 / segment_count), lv_pct(100));
            // lv_obj_align(segment, LV_ALIGN_TOP_LEFT, lv_pct(100 / count) * i, 0);
            if (i < 2) {
                lv_obj_set_style_bg_color(segment, EVA_RED, LV_PART_MAIN);
            } else {
                lv_obj_set_style_bg_color(segment, EVA_GREEN, LV_PART_MAIN);
            }
            lv_obj_set_style_border_width(segment, 0, LV_PART_MAIN);
            lv_obj_set_flex_grow(segment, 1);
        }

        lv_obj_t* hv_cell_min_bar_delimiter = lv_obj_create(screen);
        lv_obj_set_size(hv_cell_min_bar_delimiter, 4, 50);
        lv_obj_set_style_border_width(hv_cell_min_bar_delimiter, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_color(hv_cell_min_bar_delimiter, lv_color_make(0, 0, 0), LV_PART_MAIN);
        lv_obj_set_style_radius(hv_cell_min_bar_delimiter, 2, LV_PART_MAIN);
        lv_obj_align(hv_cell_min_bar_delimiter, LV_ALIGN_TOP_LEFT, 205 - 2, 180 + 106.67 - 10 + 18.38);
        
        for (int i = 0; i < segment_count; i++) {
            lv_obj_t* segment = lv_obj_create(elements.current_bar);
            lv_obj_set_style_pad_all(segment, 0, LV_PART_MAIN);
            lv_obj_set_size(segment, 10, lv_pct(100));
            lv_obj_set_style_radius(segment, 2, LV_PART_MAIN);
            // lv_obj_set_size(segment, lv_pct(100 / segment_count), lv_pct(100));
            // lv_obj_align(segment, LV_ALIGN_TOP_LEFT, lv_pct(100 / count) * i, 0);
            if (i < 2 || i > 7) {
                lv_obj_set_style_bg_color(segment, EVA_RED, LV_PART_MAIN);
            } else {
                lv_obj_set_style_bg_color(segment, EVA_GREEN, LV_PART_MAIN);
            }
            lv_obj_set_style_border_width(segment, 0, LV_PART_MAIN);
            lv_obj_set_flex_grow(segment, 1);
        }

        elements.pack_blink = lv_obj_get_child(elements.hv_capac_bar, 0);
        elements.cell_blink = lv_obj_get_child(elements.hv_cell_min_bar, 0);
        lv_obj_t* current_bar_delimiter = lv_obj_create(screen);
        lv_obj_set_size(current_bar_delimiter, 8, 50);
        lv_obj_set_style_border_width(current_bar_delimiter, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_color(current_bar_delimiter, lv_color_make(0, 0, 0), LV_PART_MAIN);
        lv_obj_set_style_radius(current_bar_delimiter, 2, LV_PART_MAIN);
        lv_obj_align(current_bar_delimiter, LV_ALIGN_TOP_LEFT, 205 - 6 + 141, 180 + 203.33 - 10 + 18.38);

        
        
        
        





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
        lv_obj_add_flag(elements.faults_container, LV_OBJ_FLAG_HIDDEN);
		
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

    bool vc_fault = false;
    bool bms_fault = false;

	void update(DataManager::car_data_t data) {
        //
        // Left Column (Levels)
        //
		int min_pack_voltage = 408; // Minimum pack voltage
		int danger_pack_voltage = 430; // Minimum pack voltage
		int max_pack_voltage = 591.6; // Maximum pack voltage
        // 
		if (data.bms_packvoltage != lastdata.bms_packvoltage) {
            lv_label_set_text_fmt(elements.hv_capac_label, "%2.3f H", (data.bms_packvoltage * 11.58f) / 1000);
			lv_label_set_text_fmt(elements.charge_label, "%2.0f%%", (data.bms_packvoltage - min_pack_voltage)/(max_pack_voltage - min_pack_voltage) * 100.0f);
            setBarSegOpacity(elements.hv_capac_bar, calculateSegment(2, lv_obj_get_child_cnt(elements.hv_capac_bar), min_pack_voltage, danger_pack_voltage, max_pack_voltage, data.bms_packvoltage));
            int packBlinkIndex = calculateSegment(2, lv_obj_get_child_cnt(elements.hv_capac_bar), min_pack_voltage, danger_pack_voltage, max_pack_voltage, data.bms_packvoltage) - 1;
            if (packBlinkIndex < 0) {
                packBlinkIndex = 0; // Ensure we don't go out of bounds
            }
            if (packBlinkIndex >= lv_obj_get_child_cnt(elements.hv_capac_bar)) {
                packBlinkIndex = lv_obj_get_child_cnt(elements.hv_capac_bar) - 1; // Ensure we don't go out of bounds
            }
            Serial.printf("Pack Blink Index: %d\n", packBlinkIndex);
            Serial.printf("Pack Index: %d\n", calculateSegment(2, lv_obj_get_child_cnt(elements.hv_cell_min_bar), min_pack_voltage, danger_pack_voltage, max_pack_voltage, data.bms_packvoltage));
            elements.pack_blink = lv_obj_get_child(elements.hv_capac_bar, packBlinkIndex);

			if(packBlinkIndex - 1 < 0){
				lv_obj_t* after_blink = lv_obj_get_child(elements.hv_capac_bar, packBlinkIndex + 1);
				lv_obj_set_style_bg_color(after_blink, EVA_RED, LV_PART_MAIN);
			}
			else if(packBlinkIndex + 1 >= lv_obj_get_child_cnt(elements.hv_capac_bar)){
				lv_obj_t* before_blink = lv_obj_get_child(elements.hv_capac_bar, packBlinkIndex - 1);
				lv_obj_set_style_bg_color(before_blink, EVA_GREEN, LV_PART_MAIN);
			}
			else if (packBlinkIndex == 2){
				lv_obj_t* after_blink = lv_obj_get_child(elements.hv_capac_bar, packBlinkIndex + 1);
				lv_obj_t* before_blink = lv_obj_get_child(elements.hv_capac_bar, packBlinkIndex - 1);
				lv_obj_set_style_bg_color(after_blink, EVA_GREEN, LV_PART_MAIN);
				lv_obj_set_style_bg_color(before_blink, EVA_RED, LV_PART_MAIN);
			}
			else{
				lv_obj_t* after_blink = lv_obj_get_child(elements.hv_capac_bar, packBlinkIndex + 1);
				lv_obj_t* before_blink = lv_obj_get_child(elements.hv_capac_bar, packBlinkIndex - 1);
				lv_obj_set_style_bg_color(after_blink, EVA_GREEN, LV_PART_MAIN);
				lv_obj_set_style_bg_color(before_blink, EVA_GREEN, LV_PART_MAIN);
			}

        }

        float min_cell_voltage = 2.5f; // Minimum cell voltage in V
        float danger_cell_voltage = 2.5f; // Minimum cell voltage in V
        float max_cell_voltage = 4.2f; // Maximum cell voltage in V
        
        
		if (data.bms_cellvoltages_min != lastdata.bms_cellvoltages_min) {
			lv_label_set_text_fmt(elements.hv_cell_min_label, "%2.1f V", data.bms_cellvoltages_min);
            setBarSegOpacity(elements.hv_cell_min_bar, calculateSegment(2, lv_obj_get_child_cnt(elements.hv_cell_min_bar), min_cell_voltage, danger_cell_voltage, max_cell_voltage, data.bms_cellvoltages_min));
            int cellBlinkIndex = calculateSegment(2, lv_obj_get_child_cnt(elements.hv_cell_min_bar), min_cell_voltage, danger_cell_voltage, max_cell_voltage, data.bms_cellvoltages_min) - 1;
            if (cellBlinkIndex < 0) {
                cellBlinkIndex = 0; // Ensure we don't go out of bounds
            }
            if (cellBlinkIndex >= lv_obj_get_child_cnt(elements.hv_cell_min_bar)) {
                cellBlinkIndex = lv_obj_get_child_cnt(elements.hv_cell_min_bar) - 1; // Ensure we don't go out of bounds
            }
            Serial.printf("Cell Blink Index: %d\n", cellBlinkIndex);
            Serial.printf("Cell Index: %d\n", calculateSegment(2, lv_obj_get_child_cnt(elements.hv_cell_min_bar), min_cell_voltage, danger_cell_voltage, max_cell_voltage, data.bms_cellvoltages_min));

            elements.cell_blink = lv_obj_get_child(elements.hv_cell_min_bar, cellBlinkIndex);
		}

        float min_current = 0.0f; // Minimum current in A
        float nominal_current = 6.0f; // Nominal current in A
        float max_current = 10.0f; // Maximum current in A

		if (data.bms_maxcurrent != lastdata.bms_maxcurrent) {
			lv_label_set_text_fmt(elements.current_label, "%2.1f A", data.bms_maxcurrent);
            setBarSegOpacityCentered(elements.current_bar, (data.bms_maxcurrent) / 10.0f);
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
		if (data.vc_faultvector != lastdata.vc_faultvector) {
            uint8_t vc_faultnum = 0;
            // Loop over possible VC faults
            for(int i = 0; i < 17; i++) {
                bool faulted = (data.vc_faultvector >> i) & 1;
                if(faulted) {
                    lv_obj_clear_flag(elements.faults_container, LV_OBJ_FLAG_HIDDEN);
                    
                    lv_label_set_text(elements.faults_text, VC_FAULT_MESSAGES[i]);
                    vc_faultnum++;
                    vc_fault = true;
                }
            }
        }
        
        if (data.bms_faultvector != lastdata.bms_faultvector) {
            // If any fault message changes, we must update them all...
            
            uint8_t bms_faultnum = 0;
            lv_label_set_text(elements.faults_text, "");
            
            
            // Loop over possible BMS faults
            for(int i = 0; i < 11; i++) {
                bool faulted = (data.bms_faultvector >> i) & 1;
                if(faulted) {
                    lv_obj_clear_flag(elements.faults_container, LV_OBJ_FLAG_HIDDEN);
                    lv_label_set_text(elements.faults_text, BMS_FAULT_MESSAGES[i]);
                    bms_faultnum++;
                    bms_fault = true;
                }
            }
		}

        if (data.vc_faultvector != lastdata.vc_faultvector || data.bms_faultvector != lastdata.bms_faultvector) {
            if (bms_fault || vc_fault) {
                lv_obj_clear_flag(elements.status_bar, LV_OBJ_FLAG_HIDDEN);
            }
            else {
                lv_obj_add_flag(elements.status_bar, LV_OBJ_FLAG_HIDDEN);
            }
        }

        //
        // Blinking animation
        //
        unsigned int speed = 800;
        lv_coord_t scrollx;
		if(lv_tick_get() % speed >= 0 && lv_tick_get() % speed <= speed / 2 && fault_color == false) {
            scrollx = lv_obj_get_scroll_x(elements.faults_text);
			lv_obj_remove_style(elements.faults_text, &(styles->redBlackFault), LV_PART_MAIN);
            lv_obj_remove_style(elements.faults_container, &(styles->redBlackFault), LV_PART_MAIN);
			lv_obj_add_style(elements.faults_text, &(styles->blackRedFault), LV_PART_MAIN);
			lv_obj_add_style(elements.faults_container, &(styles->blackRedFault), LV_PART_MAIN);
            lv_obj_scroll_to_x(elements.faults_text, scrollx - 50, LV_ANIM_OFF);

            if(data.bms_packvoltage < danger_pack_voltage){lv_obj_set_style_bg_color(elements.pack_blink, EVA_MEDIUM_RED, LV_PART_MAIN);}
            else{lv_obj_set_style_bg_color(elements.pack_blink, EVA_MEDIUM_GREEN, LV_PART_MAIN);}
            
            if(data.bms_packvoltage < danger_cell_voltage){lv_obj_set_style_bg_color(elements.cell_blink, EVA_MEDIUM_RED, LV_PART_MAIN);}
            else{lv_obj_set_style_bg_color(elements.cell_blink, EVA_MEDIUM_GREEN, LV_PART_MAIN);}
            
            
            fault_color = true;
		}
		else if (lv_tick_get() % speed >= speed / 2 && lv_tick_get() % speed <= speed && fault_color == true) {
            scrollx = lv_obj_get_scroll_x(elements.faults_text);
			lv_obj_remove_style(elements.faults_text, &(styles->blackRedFault), LV_PART_MAIN);
			lv_obj_remove_style(elements.faults_container, &(styles->blackRedFault), LV_PART_MAIN);
			lv_obj_add_style(elements.faults_text, &(styles->redBlackFault), LV_PART_MAIN);
			lv_obj_add_style(elements.faults_container, &(styles->redBlackFault), LV_PART_MAIN);
            lv_obj_scroll_to_x(elements.faults_text, scrollx - 50, LV_ANIM_OFF);
            
            if(data.bms_packvoltage < danger_pack_voltage){lv_obj_set_style_bg_color(elements.pack_blink, EVA_RED, LV_PART_MAIN);}
            else{lv_obj_set_style_bg_color(elements.pack_blink, EVA_GREEN, LV_PART_MAIN);}
            
            if(data.bms_packvoltage < danger_cell_voltage){lv_obj_set_style_bg_color(elements.cell_blink, EVA_RED, LV_PART_MAIN);}
            else{lv_obj_set_style_bg_color(elements.cell_blink, EVA_GREEN, LV_PART_MAIN);}


            fault_color = false;
		}
		
		lastdata = data;
	}
}