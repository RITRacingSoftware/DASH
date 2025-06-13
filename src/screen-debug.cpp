#include "screen-debug.h"

#ifdef DASH_TESTING
#include "testing_arduino.h"
#else
#include <Arduino.h>
#endif

#include "data-manager.h"
#include "display-manager.h"

#include "lvgl.h"
#include <math.h>

#define UPDATE_FLOAT(PARAM, ELEMENT, FMT) if((data.PARAM != lastdata.PARAM) && (!isnan(data.PARAM))) lv_label_set_text_fmt(elements.ELEMENT, FMT, data.PARAM)

//static const uint16_t motor_offsets[4] = {210, 419, 596, 788};
static const int motor_offsets[4] = {788, 596, 419, 210};
static const int motor_centers[4] = {692, 508, 315, 111};

lv_obj_t** createImgObjArray(lv_obj_t** imgArray, const lv_img_dsc_t* src, lv_obj_t* parent, int x, int y) {
    for (int i = 0; i < 4; i++) {
        imgArray[i] = lv_img_create(parent);
        lv_img_set_src(imgArray[i], src);
        lv_obj_align(imgArray[i], LV_ALIGN_TOP_LEFT, x + motor_offsets[i], y);
        lv_obj_set_size(imgArray[i], LV_SIZE_CONTENT,LV_SIZE_CONTENT);

    }
    return imgArray;
}

void insert_by_motor(lv_obj_t *parent, lv_obj_t **objects, lv_style_t *style, const int *pos, int width, int x, int y, const char *fmt) {
    for (int i=0; i < 4; i++) {
        objects[i] = lv_label_create(parent);
        lv_obj_set_size(objects[i], width, LV_SIZE_CONTENT);
        lv_obj_align(objects[i], LV_ALIGN_TOP_LEFT, x + pos[i], y);
        if (style) lv_obj_add_style(objects[i], style, LV_PART_MAIN);
        lv_label_set_text(objects[i], fmt);
    }
}

namespace ScreenDebug {
    lv_obj_t* screen;
    DataManager::car_data_t lastdata;

    DisplayManager::debug_styles_t* styles;

    // Display elements to keep
    struct elements_s {
        lv_obj_t* rpmbar;
        lv_obj_t* rpmlabel;
        lv_obj_t* mphlabel;
        lv_obj_t* bms_soc_label;
        lv_obj_t* bms_cellvoltage_min_label;
        lv_obj_t* bms_cellvoltage_max_label;
        lv_obj_t* bms_cellvoltage_avg_label;
        lv_obj_t* bms_packvoltage_label;
        lv_obj_t* bms_current_label;
        lv_obj_t* bms_maxcurrent_label;
        lv_obj_t* bms_maxtemp_label;
        lv_obj_t* bms_avgtemp_label;
        lv_obj_t* status_overall;
        lv_obj_t* status_vcstatus;
        lv_obj_t* status_mcustatus;
        lv_obj_t* status_bmsstatus;
        lv_obj_t* vsm_state_label;
        lv_obj_t* faults_textarea;
        lv_style_t limp_style;

        lv_obj_t* faultlog[10][2]; // 10 lines, 2 columns (time, fault)
        lv_obj_t* v_min;
        lv_obj_t* v_max;
        lv_obj_t* v_avg;

        lv_obj_t* temp_min;
        lv_obj_t* temp_max;

        lv_obj_t* latch_max_current;

        lv_obj_t* fbps_label;
        lv_obj_t* rbps_label;

        lv_obj_t* tire_temps[4];
        lv_obj_t* rotor_temps[4];
        lv_obj_t* motor_temp[4];
        lv_obj_t* inv_temp[4];
        lv_obj_t* motor_torque[4];
        lv_obj_t* motor_state[4];

        lv_obj_t* tc_status;
        lv_obj_t *vc_status;
        
        lv_obj_t* wheelNomIndicator[4];
        lv_obj_t* wheelErrIndicator[4];

    } elements;

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

    const lv_color_t LIMP_COLORS[] = {
        lv_palette_main(LV_PALETTE_NONE),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_LIGHT_BLUE),
        lv_palette_main(LV_PALETTE_YELLOW),
        lv_palette_main(LV_PALETTE_ORANGE),
        lv_palette_main(LV_PALETTE_RED),
    };

    lv_obj_t* init(DisplayManager::debug_styles_t* de) {
        styles = de;
        Serial.printf("Initializing Debug Screen\n");

        screen = lv_obj_create(NULL);
        lv_obj_add_style(screen, &(styles->mainTextSmall), LV_PART_MAIN);
        
        // Custom styles

        lv_style_init(&elements.limp_style);
        lv_style_set_bg_color(&elements.limp_style, LIMP_COLORS[0]);
        lv_style_set_text_color(&elements.limp_style, lv_color_white());
        lv_obj_add_style(screen, &elements.limp_style, LV_PART_MAIN);

        // Background image
        lv_obj_t * bg = lv_img_create(screen);
        lv_img_set_src(bg, &DebugScreen);
        lv_obj_align(bg, LV_ALIGN_CENTER,0, 0);
        lv_obj_set_size(bg, LV_SIZE_CONTENT,LV_SIZE_CONTENT);


        //Ready To Drive Path
        /*lv_obj_t * stepSPrecharge = lv_img_create(screen);
        lv_img_set_src(stepSPrecharge, &rtdStartPrecharge);
        lv_obj_align(stepSPrecharge, LV_ALIGN_TOP_LEFT, 600, 115);
        lv_obj_set_size(stepSPrecharge, LV_SIZE_CONTENT,LV_SIZE_CONTENT);*/
        
        /*lv_style_t teststyle;
        lv_style_init(&teststyle);

        lv_obj_add_style(stepSPrecharge, &teststyle, LV_PART_MAIN);
        lv_obj_set_style_img_recolor_opa(stepSPrecharge, LV_OPA_COVER, 0);
        lv_obj_set_style_img_recolor(stepSPrecharge, lv_palette_main(LV_PALETTE_BLUE), 0);*/


        /*lv_obj_t * stepHV = lv_img_create(screen);
        lv_img_set_src(stepHV, &rtdHVEnabled);
        lv_obj_align(stepHV, LV_ALIGN_TOP_LEFT, 600, 135);
        lv_obj_set_size(stepHV, LV_SIZE_CONTENT,LV_SIZE_CONTENT);

        lv_obj_t * stepEPrecharge = lv_img_create(screen);
        lv_img_set_src(stepEPrecharge, &rtdEndPrecharge);
        lv_obj_align(stepEPrecharge, LV_ALIGN_TOP_LEFT, 600, 155);
        lv_obj_set_size(stepEPrecharge, LV_SIZE_CONTENT,LV_SIZE_CONTENT);
        
        lv_obj_t * stepRTD = lv_img_create(screen);
        lv_img_set_src(stepRTD, &rtdReadyToDrive);
        lv_obj_align(stepRTD, LV_ALIGN_TOP_LEFT, 600, 175);
        lv_obj_set_size(stepRTD, LV_SIZE_CONTENT,LV_SIZE_CONTENT);*/
        
        // Vehicle state
        elements.vc_status = lv_label_create(screen);
        lv_obj_set_size(elements.vc_status, 170, LV_SIZE_CONTENT);
        lv_obj_align(elements.vc_status, LV_ALIGN_TOP_LEFT, 600, 115);
        lv_obj_add_style(elements.vc_status, &(de->rect), LV_PART_MAIN);
        lv_label_set_text(elements.vc_status, "???");

        // TC status
        elements.tc_status = lv_label_create(screen);
        lv_obj_set_size(elements.tc_status, 170, LV_SIZE_CONTENT);
        lv_obj_align(elements.tc_status, LV_ALIGN_TOP_LEFT, 600, 138);
        lv_obj_add_style(elements.tc_status, &(de->rect), LV_PART_MAIN);
        lv_label_set_text(elements.tc_status, "TC: ?");
        
        // Inverter states
        insert_by_motor(screen, elements.motor_state, &(de->rect), motor_centers, 100, -50, 290, "????");

        // Motor temps
        insert_by_motor(screen, elements.motor_temp, NULL, motor_offsets, 100, -110, 384, "??.? C");
        // Inverter feedback torques
        insert_by_motor(screen, elements.motor_torque, NULL, motor_offsets, 100, -110, 409, "??.? C");
        // Inverter temps
        insert_by_motor(screen, elements.inv_temp, NULL, motor_offsets, 100, -110, 434, "??.? C");
        
        // Front brake pressure
        elements.fbps_label = lv_label_create(screen);
        lv_obj_set_size(elements.fbps_label, 60, LV_SIZE_CONTENT);
        lv_obj_align(elements.fbps_label, LV_ALIGN_TOP_LEFT, 613, 75);
        lv_label_set_text(elements.fbps_label, "????");
        
        // Rear break pressure
        elements.rbps_label = lv_label_create(screen);
        lv_obj_set_size(elements.rbps_label, 60, LV_SIZE_CONTENT);
        lv_obj_align(elements.rbps_label, LV_ALIGN_TOP_LEFT, 683, 75);
        lv_label_set_text(elements.rbps_label, "????");

        /*lv_obj_t *test = lv_obj_create(screen);
        lv_obj_add_style(test, &(de->rect), LV_PART_MAIN);
        lv_obj_set_size(test, 780, 235);
        lv_obj_set_pos(test, 10, 10);*/


        // RPM Bar
        // elements.rpmbar = lv_bar_create(screen);
        // lv_bar_set_range(elements.rpmbar, 0, 5000);
        // lv_obj_set_size(elements.rpmbar, 90, 320);
        // lv_obj_align(elements.rpmbar, LV_ALIGN_TOP_LEFT, 10, 10);
        // lv_obj_add_style(elements.rpmbar, styles->barstyle, 0);
        // lv_obj_add_style(elements.rpmbar, styles->barindstyle, LV_PART_INDICATOR);
        // // RPM Bar Label
        // elements.rpmlabel = lv_label_create(screen);
        // lv_label_set_text(elements.rpmlabel, "????\nRPM");
        // lv_obj_align_to(elements.rpmlabel, elements.rpmbar, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
        // // MPH Label
        // elements.mphlabel = lv_label_create(screen);
        // lv_label_set_text(elements.mphlabel, "??.?\nMPH");
        // lv_obj_align_to(elements.mphlabel, elements.rpmlabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

        // // Status display elements
        // lv_obj_t* status_area = lv_obj_create(screen);
        // lv_obj_set_size(status_area, 350, 320);
        // lv_obj_align(status_area, LV_ALIGN_TOP_MID, -115, 10);
        // lv_obj_add_style(status_area, styles->style, LV_PART_MAIN);
        // // Disable scroll bars
        // lv_obj_set_scrollbar_mode(status_area, LV_SCROLLBAR_MODE_OFF);

        // elements.status_overall = lv_label_create(status_area);
        // lv_obj_align(elements.status_overall, LV_ALIGN_CENTER, 0, -45);
        // lv_label_set_recolor(elements.status_overall, true);
        // lv_label_set_text(elements.status_overall, "#ff0000 NOT READY#");

        // elements.status_vcstatus = lv_label_create(status_area);
        // lv_obj_align(elements.status_vcstatus, LV_ALIGN_CENTER, 0, -15);
        // lv_label_set_recolor(elements.status_vcstatus, true);
        // lv_label_set_text(elements.status_vcstatus, "VC: ???");

        // elements.status_mcustatus = lv_label_create(status_area);
        // lv_obj_align(elements.status_mcustatus, LV_ALIGN_CENTER, 0, 15);
        // lv_label_set_recolor(elements.status_mcustatus, true);
        // lv_label_set_text(elements.status_mcustatus, "MC: ???");

        // elements.status_bmsstatus = lv_label_create(status_area);
        // lv_obj_align(elements.status_bmsstatus, LV_ALIGN_CENTER, 0, 45);
        // lv_label_set_recolor(elements.status_bmsstatus, true);
        // lv_label_set_text(elements.status_bmsstatus, "BMS: ???");

        // // VSM state label
        // elements.vsm_state_label = lv_label_create(status_area);
        // lv_obj_align(elements.vsm_state_label, LV_ALIGN_CENTER, 0, 75);
        // lv_label_set_text(elements.vsm_state_label, "VSM = ?");

        // // SOC Label
        // elements.bms_soc_label = lv_label_create(bms_area);
        // lv_obj_align(elements.bms_soc_label, LV_ALIGN_CENTER, 0, -75);
        // lv_label_set_text(elements.bms_soc_label, "SOC = ???%");

        // Pack voltage
        elements.bms_packvoltage_label = lv_label_create(screen);
        lv_obj_align(elements.bms_packvoltage_label, LV_ALIGN_TOP_LEFT, 240, 92);
        lv_label_set_text(elements.bms_packvoltage_label, "???.? V");
        
        // Minimum cell voltage
        elements.bms_cellvoltage_min_label = lv_label_create(screen);
        lv_obj_align(elements.bms_cellvoltage_min_label, LV_ALIGN_TOP_LEFT, 330, 92);
        lv_label_set_text(elements.bms_cellvoltage_min_label, "?.?? V");

        // Maximum cell voltage
        elements.bms_cellvoltage_max_label = lv_label_create(screen);
        lv_obj_align(elements.bms_cellvoltage_max_label, LV_ALIGN_TOP_LEFT, 420, 92);
        lv_label_set_text(elements.bms_cellvoltage_max_label, "?.?? V");
        
        // Average cell voltage
        elements.bms_cellvoltage_avg_label = lv_label_create(screen);
        lv_obj_align(elements.bms_cellvoltage_avg_label, LV_ALIGN_TOP_LEFT, 510, 92);
        lv_label_set_text(elements.bms_cellvoltage_avg_label, "?.?? V");

        // // Cell Voltages Label
        // elements.bms_packvoltage_label = lv_label_create(bms_area);
        // lv_obj_align(elements.bms_packvoltage_label, LV_ALIGN_CENTER, 0, -15);
        // lv_label_set_text(elements.bms_packvoltage_label, "PACK = ???.? V");

        // // Current Label
        // elements.bms_current_label = lv_label_create(bms_area);
        // lv_obj_align(elements.bms_current_label, LV_ALIGN_CENTER, 0, 15);
        // lv_label_set_text(elements.bms_current_label, "I = ?.?? A");

        // // Peak current Label
        // elements.bms_maxcurrent_label = lv_label_create(bms_area);
        // lv_obj_align(elements.bms_maxcurrent_label, LV_ALIGN_CENTER, 0, 45);
        // lv_label_set_text(elements.bms_maxcurrent_label, "MAX I = ?.?? A");

        // Max temp
        elements.bms_maxtemp_label = lv_label_create(screen);
        lv_obj_align(elements.bms_maxtemp_label, LV_ALIGN_TOP_LEFT, 260, 180);
        lv_label_set_text(elements.bms_maxtemp_label, "??? C");
        
        // Average temp
        elements.bms_avgtemp_label = lv_label_create(screen);
        lv_obj_align(elements.bms_avgtemp_label, LV_ALIGN_TOP_LEFT, 380, 180);
        lv_label_set_text(elements.bms_avgtemp_label, "??? C");

        // Fault text area
        elements.faults_textarea = lv_textarea_create(screen);
        lv_obj_set_size(elements.faults_textarea, 180, 165);
        lv_obj_align(elements.faults_textarea, LV_ALIGN_TOP_LEFT, 20, 50);
        lv_obj_add_style(elements.faults_textarea, &(de->faultText), LV_PART_MAIN);
        lv_obj_add_style(lv_textarea_get_label(elements.faults_textarea), &(de->warn), LV_PART_SELECTED);

        // Put garbage data in lastdata so initial values are loaded
        memset(&lastdata, 0xff, sizeof(lastdata));

        Serial.printf("Initialized Debug Screen\n");

        return screen;
    }

    void update(DataManager::car_data_t data) {
        for (int i=0; i < 4; i++) {
            if (data.torque[i] != lastdata.torque[i]) {
                lv_label_set_text_fmt(elements.motor_torque[i], "%3.1f%%", data.torque[i]);
            }
            if (data.inv_state[i] != lastdata.inv_state[i]) {
                lv_label_set_text_static(elements.motor_state[i], INVERTER_STATES[data.inv_state[i]]);
                lv_obj_remove_style(elements.motor_state[i], &(styles->warn), LV_PART_ANY);
                lv_obj_remove_style(elements.motor_state[i], &(styles->error), LV_PART_ANY);
                if ((data.inv_state[i] == 1) || (data.inv_state[i] == 4)) {
                    lv_obj_add_style(elements.motor_state[i], &(styles->warn), LV_PART_MAIN);
                }
                if ((data.inv_state[i] == 2) || (data.inv_state[i] == 3) || (data.inv_state[i] == 5)) {
                    lv_obj_add_style(elements.motor_state[i], &(styles->error), LV_PART_MAIN);
                }
            }
            UPDATE_FLOAT(inv_temp[i], inv_temp[i], "%2.1f C");
            UPDATE_FLOAT(motor_temp[i], motor_temp[i], "%2.1f C");
        }
        // Traction control status
        if ((data.tv_msg != lastdata.tv_msg) || (data.tc_msg != lastdata.tc_msg)) {
            lv_label_set_text_static(elements.tc_status, (data.tv_msg == data.tc_msg ? "TC: -" : "TC: active"));
        }
        // if(data.mcu_motorrpm != lastdata.mcu_motorrpm
        //     || data.bms_limplevel != lastdata.bms_limplevel) {
        //     lv_style_set_bg_color(&elements.limp_style, LIMP_COLORS[data.bms_limplevel]);
        //     lv_bar_set_value(elements.rpmbar, data.mcu_motorrpm, LV_ANIM_OFF);
        //     lv_label_set_text_fmt(elements.rpmlabel, "%04d\nRPM", data.mcu_motorrpm);
        //     lv_label_set_text_fmt(elements.mphlabel, "%04.1f\nMPH", data.mcu_carspeed);
        // }

        // // Status Elements
        // // TODO: Re-evaluate readiness detection
        // /*if(data.vc_status != lastdata.vc_status ||
        //     data.mcu_status != lastdata.mcu_status ||
        //     data.bms_faultvector != lastdata.bms_faultvector) {

        //     if(data.vc_status == 2 && data.mcu_status == 3 &&
        //         data.bms_faultvector == 0) {
        //         lv_label_set_text(elements.status_overall, "#00ff00 READY TO DRIVE#");
        //     }
        //     else {
        //         lv_label_set_text(elements.status_overall, "#ff0000 NOT READY#");
        //     }

        // }*/
        if(data.vc_status != lastdata.vc_status) {
            lv_label_set_text_static(elements.vc_status, VC_STATUS_MESSAGES[data.vc_status]);
            lv_obj_remove_style(elements.vc_status, &(styles->warn), LV_PART_ANY);
            lv_obj_remove_style(elements.vc_status, &(styles->error), LV_PART_ANY);
            if (data.vc_status == 2) {
                lv_obj_add_style(elements.vc_status, &(styles->warn), LV_PART_MAIN);
            }
            if ((data.vc_status >= 3) && (data.vc_status <= 5)) {
                lv_obj_add_style(elements.vc_status, &(styles->error), LV_PART_MAIN);
            }
        }

        // if(data.mcu_status != lastdata.mcu_status) {
        //     if(data.mcu_status >= 1 && data.mcu_status <= 4) {
        //         lv_label_set_text_fmt(elements.status_mcustatus, "MC: %s",
        //             MCU_STATUS_MESSAGES[data.mcu_status]);
        //     }
        //     else {
        //         lv_label_set_text_fmt(elements.status_mcustatus, "MC: %s",
        //             MCU_STATUS_MESSAGES[0]);
        //     }
        // }

        // if(data.mcu_vsm_state != lastdata.mcu_vsm_state) {
        //     lv_label_set_text_fmt(elements.vsm_state_label, "VSM = %d", data.mcu_vsm_state);
        // }

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

        //     if(data.bms_faultvector == 0) {
        //         lv_label_set_text(elements.status_bmsstatus, "BMS: READY");
        //     }
        //     else {
        //         lv_label_set_text_fmt(elements.status_bmsstatus, "BMS: %d FAULTS", bms_faultnum);
        //     }

        //     if(vc_faultnum == 0 && bms_faultnum == 0) {
        //         lv_textarea_set_text(elements.faults_textarea, "");
        //     }
        }
        
        if(data.rbps != lastdata.rbps) {
            lv_label_set_text_fmt(elements.rbps_label, "%4.0f", data.rbps);
        }
        if(data.fbps != lastdata.fbps) {
            lv_label_set_text_fmt(elements.fbps_label, "%4.0f", data.fbps);
        }

        // BMS Elements
        // if(data.bms_soc != lastdata.bms_soc) {
        //     lv_label_set_text_fmt(elements.bms_soc_label, "SOC = %d%%", data.bms_soc);
        // }
        UPDATE_FLOAT(bms_cellvoltages_min, bms_cellvoltage_min_label, "%1.2fV");
        UPDATE_FLOAT(bms_cellvoltages_max, bms_cellvoltage_max_label, "%1.2fV");
        UPDATE_FLOAT(bms_cellvoltages_avg, bms_cellvoltage_avg_label, "%1.2fV");
        UPDATE_FLOAT(bms_packvoltage, bms_packvoltage_label, "%3.1fV");
        // if(data.bms_buscurrent != lastdata.bms_buscurrent) {
        //     lv_label_set_text_fmt(elements.bms_current_label, "I = %1.2f A", data.bms_buscurrent);
        //     if(data.bms_maxcurrent != lastdata.bms_maxcurrent) {
        //         lv_label_set_text_fmt(elements.bms_maxcurrent_label, "MAX I = %1.2f A", data.bms_maxcurrent);
        //     }
        // }
        if(data.bms_maxtemp != lastdata.bms_maxtemp) {
            lv_label_set_text_fmt(elements.bms_maxtemp_label, "%3.0f C", data.bms_maxtemp);
        }
        if(data.bms_avgtemp != lastdata.bms_avgtemp) {
            lv_label_set_text_fmt(elements.bms_avgtemp_label, "%3.0f C", data.bms_avgtemp);
        }

        lastdata = data;
    }
}
