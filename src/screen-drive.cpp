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
		lv_obj_t* hv_capac_label;
		lv_obj_t* hv_avg_label;
		lv_obj_t* lv_label;
		lv_obj_t* current_label;

		lv_obj_t* max_batt_temp_label;
		lv_obj_t* avg_batt_temp_label;

		lv_obj_t* charge_label;
		lv_obj_t* mph_label;

		lv_obj_t* faults_textarea; //temporary until we have a proper fault implementation
		lv_obj_t* vc_status;

	} elements;

	lv_style_t temp_cold_style;
	lv_style_t temp_optimal_style;
	lv_style_t temp_hot_style;
	lv_style_t text_style;

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

		int width_center = 155;


        // // Background image
        lv_obj_t * imgtest = lv_img_create(screen);
        lv_img_set_src(imgtest, &DriveScreen);
        lv_obj_align(imgtest, LV_ALIGN_CENTER,0, 0);
        lv_obj_set_size(imgtest, LV_SIZE_CONTENT,LV_SIZE_CONTENT);

		// Vehicle state TEMP UNTIL WE CAN GET THE FANCY STUFF WORKING
        elements.vc_status = lv_label_create(screen);
        lv_obj_set_size(elements.vc_status, 170, LV_SIZE_CONTENT);
        lv_obj_align(elements.vc_status, LV_ALIGN_TOP_LEFT, 114 + width_center, 300);
        lv_obj_add_style(elements.vc_status, &(dr->rect), LV_PART_MAIN);
        lv_label_set_text(elements.vc_status, "???");


		//
		// Left Column (Levels)
		//

		
		elements.hv_capac_label = lv_label_create(screen);
        lv_obj_align(elements.hv_capac_label, LV_ALIGN_TOP_LEFT, 10, 150);
        lv_label_set_text(elements.hv_capac_label, "?");
		lv_obj_add_style(elements.hv_capac_label, &dr->bmsText, LV_PART_MAIN);

		elements.hv_avg_label = lv_label_create(screen); // MINIMUM CELL VOLTAGE FOR NOW
		lv_obj_align(elements.hv_avg_label, LV_ALIGN_TOP_LEFT, 10, 235);
		lv_label_set_text(elements.hv_avg_label, "?");
		lv_obj_add_style(elements.hv_avg_label, &dr->bmsText, LV_PART_MAIN);

		elements.lv_label = lv_label_create(screen);
		lv_obj_align(elements.lv_label, LV_ALIGN_TOP_LEFT, 10, 320);
		lv_label_set_text(elements.lv_label, "?");
		lv_obj_add_style(elements.lv_label, &dr->bmsText, LV_PART_MAIN);

		elements.current_label = lv_label_create(screen);
		lv_obj_align(elements.current_label, LV_ALIGN_TOP_LEFT, 10, 405);
		lv_label_set_text(elements.current_label, "?");
		lv_obj_add_style(elements.current_label, &dr->bmsText, LV_PART_MAIN);

		//
		// Center Column
		//
		
		elements.charge_label = lv_label_create(screen);
		lv_obj_align(elements.charge_label, LV_ALIGN_TOP_LEFT, 395 - (width_center / 2), 142);
		lv_label_set_text(elements.charge_label, "?");
		lv_obj_add_style(elements.charge_label, &dr->middleText, LV_PART_MAIN);
		lv_obj_set_width(elements.charge_label, width_center);
		lv_obj_set_style_text_align(elements.charge_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

		elements.mph_label = lv_label_create(screen);
		lv_obj_align(elements.mph_label, LV_ALIGN_TOP_LEFT, 395 - (width_center / 2), 218);
		lv_label_set_text(elements.mph_label, "?");
		lv_obj_add_style(elements.mph_label, &dr->middleText, LV_PART_MAIN);
		lv_obj_set_width(elements.mph_label, width_center);
		lv_obj_set_style_text_align(elements.mph_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

		//
		// Right Column (Temps)
		//
		int width_temp = 151;
		elements.max_batt_temp_label = lv_label_create(screen);
		lv_obj_align(elements.max_batt_temp_label, LV_ALIGN_TOP_LEFT, 707 - (width_temp / 2), 175);
		lv_label_set_text(elements.max_batt_temp_label, "??");
		lv_obj_add_style(elements.max_batt_temp_label, &dr->tempText, LV_PART_MAIN);
		lv_obj_set_width(elements.max_batt_temp_label, width_temp);
		lv_obj_set_style_text_align(elements.max_batt_temp_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

		elements.avg_batt_temp_label = lv_label_create(screen);
		lv_obj_align(elements.avg_batt_temp_label, LV_ALIGN_TOP_LEFT, 707 - (width_temp / 2), 237);
		lv_label_set_text(elements.avg_batt_temp_label, "??");
		lv_obj_add_style(elements.avg_batt_temp_label, &dr->tempText, LV_PART_MAIN);
		lv_obj_set_width(elements.avg_batt_temp_label, width_temp);
		lv_obj_set_style_text_align(elements.avg_batt_temp_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

		elements.faults_textarea = lv_textarea_create(screen);
        lv_obj_set_size(elements.faults_textarea, 780, 100);
        lv_obj_align(elements.faults_textarea, LV_ALIGN_TOP_LEFT, 10, 10);
        lv_obj_add_style(elements.faults_textarea, &(dr->faultText), LV_PART_MAIN);
        lv_obj_add_style(lv_textarea_get_label(elements.faults_textarea), &(dr->error), LV_PART_SELECTED);


		memset(&lastdata, 0xff, sizeof(lastdata));

		Serial.printf("Initialized Drive Screen\n");
		return screen;
	}

	void update(DataManager::car_data_t data) {
		int min_pack_voltage = 60; // Minimum pack voltage
		int max_pack_voltage = 560; // Maximum pack voltage

		if (data.bms_packvoltage != lastdata.bms_packvoltage) {
			lv_label_set_text_fmt(elements.hv_capac_label, "%2.1f V", data.bms_packvoltage);
			lv_label_set_text_fmt(elements.charge_label, "%2.0f%%", (data.bms_packvoltage - min_pack_voltage)/(max_pack_voltage - min_pack_voltage) * 100.0f);
		}

		if (data.bms_cellvoltages_min != lastdata.bms_cellvoltages_min) {
			lv_label_set_text_fmt(elements.hv_avg_label, "%2.1f V", data.bms_cellvoltages_min);
		}

		if (data.lv_voltage != lastdata.lv_voltage) {
			lv_label_set_text_fmt(elements.lv_label, "%2.1f V", data.lv_voltage);
		}

		if (data.bms_maxcurrent != lastdata.bms_maxcurrent) {
			lv_label_set_text_fmt(elements.current_label, "%2.1f A", data.bms_maxcurrent);
		}
        
        if (data.vel != lastdata.vel) {
        	lv_label_set_text_fmt(elements.mph_label, "%2.0f", data.vel);
        }

		if (data.bms_maxtemp != lastdata.bms_maxtemp) {
			lv_label_set_text_fmt(elements.max_batt_temp_label, "%2.0f C", data.bms_maxtemp);
			if (data.bms_maxtemp < 50) {
				// lv_obj_add_style(elements.max_batt_temp_label, &temp_cold_style, LV_PART_MAIN);
			}
			else if (data.bms_maxtemp < 70) {
				// lv_obj_add_style(elements.max_batt_temp_label, &temp_optimal_style, LV_PART_MAIN);
			}
			else {
				// lv_obj_add_style(elements.max_batt_temp_label, &temp_hot_style, LV_PART_MAIN);
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

		if(data.vc_status != lastdata.vc_status) {
            lv_label_set_text_static(elements.vc_status, VC_STATUS_MESSAGES[data.vc_status]);
            lv_obj_remove_style(elements.vc_status, &(styles->warn), LV_PART_ANY);
            lv_obj_remove_style(elements.vc_status, &(styles->error), LV_PART_ANY);
			lv_obj_remove_style(elements.vc_status, &(styles->nominal), LV_PART_ANY);
            if (data.vc_status == 2) {
                lv_obj_add_style(elements.vc_status, &(styles->warn), LV_PART_MAIN);
            }
            if ((data.vc_status >= 3) && (data.vc_status == 4)&& (data.vc_status == 6)) {
                lv_obj_add_style(elements.vc_status, &(styles->error), LV_PART_MAIN);
            }
			if(data.vc_status == 5) {
				lv_obj_add_style(elements.vc_status, &(styles->nominal), LV_PART_MAIN);
			}
        }

		if(data.vc_faultvector != lastdata.vc_faultvector ||
            data.bms_faultvector != lastdata.bms_faultvector) {
            // If any fault message changes, we must update them all...

            bool firstfault = true; // Used for pretty-printing
            uint8_t vc_faultnum = 0;
            uint8_t bms_faultnum = 0;
            lv_obj_t *ta_label = lv_textarea_get_label(elements.faults_textarea);

            lv_textarea_set_text(elements.faults_textarea, "");

            // Loop over possible VC faults
            for(int i = 0; i < 17; i++) {
                bool faulted = (data.vc_faultvector >> i) & 1;
                if(faulted) {
                    if(!firstfault) {
                        // Pretty printing
                        lv_textarea_add_text(elements.faults_textarea, ", ");
                    }
                    firstfault = false;
                    lv_textarea_add_text(elements.faults_textarea, VC_FAULT_MESSAGES[i]);
                    vc_faultnum++;
                }
            }
            int startpos = strlen(lv_label_get_text(ta_label));

            // Loop over possible BMS faults
            for(int i = 0; i < 11; i++) {
                bool faulted = (data.bms_faultvector >> i) & 1;
                if(faulted) {
                    if(!firstfault) {
                        // Pretty printing
                        lv_textarea_add_text(elements.faults_textarea, ", ");
                    }
                    firstfault = false;
                    lv_textarea_add_text(elements.faults_textarea, BMS_FAULT_MESSAGES[i]);
                    bms_faultnum++;
                }
            }
            int endpos = strlen(lv_label_get_text(ta_label));
            lv_label_set_text_sel_start(ta_label, startpos);
            lv_label_set_text_sel_end(ta_label, endpos);
		}

		
		lastdata = data;
	}
}