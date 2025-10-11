#include "data-manager.h"

#include <cstdint>
#include <cmath>

#ifdef DASH_TESTING
#include "testing_arduino.h"
#include <cstdio>
#else
#include <Arduino.h>
#endif


#include "can-manager.h"
#include "display-manager.h"
#include "c_files/main_dbc.h"

#include <math.h>



/*

What is needed for 

Drive Screen
vectored mph
temps
current
voltages


Debug Screen
Drive Screen plus 
all temps and stuff











*/

#define DATA_MANAGER_VC_FAULT_MASK  0x007fffff


namespace DataManager {
    car_data_t data;
    double northVec = 0;
    double eastVec = 0;

    // Calculated using GNU units to prevent precision loss
    // > units "1/minute * 11/45 * 16in * pi" "mph"
    const double MS_TO_MPH = .011635528;

    void init() {
        Serial.printf("Initializing DataManager\n");
        DisplayManager::init();
        CANManager::init();
        data.bms_maxcurrent = -100000;
        //Assume we have a fault until we certianly don't
        data.bms_faultvector = 0x8000;
        data.lv_voltage = 0.0;
        memset(&data, 0xff, sizeof(data));
        Serial.printf("Initialized DataManager\n");
    }

    void update() {
        CANManager::can_message_t message;
        float vel_x, vel_y;
        while(CANManager::hasMessage()) {
            message = CANManager::getMessage();
            //Serial.printf("Got CAN message, ID=0x%04x\n", message.id);
            switch(message.id) {
                // vector_nav_vel_ned_e and vector_nav_vel_ned_n
                case MAIN_DBC_VECTOR_NAV6_FRAME_ID: {
                    main_dbc_vector_nav6_t sensor;
                    main_dbc_vector_nav6_unpack(&sensor, message.data, message.len);
                    vel_x = sensor.vector_nav_vel_body_x;
                    vel_y = sensor.vector_nav_vel_body_y;
                    data.vel = sqrt(vel_x * vel_x + vel_y * vel_y);
                    break; }
                case MAIN_DBC_BMS_STATUS_FRAME_ID: {
                    main_dbc_bms_status_t bms_status;
                    main_dbc_bms_status_unpack(&bms_status, message.data, message.len);
                    data.bms_soc = bms_status.bms_status_soc;
                    data.bms_packvoltage = bms_status.bms_status_pack_voltage * 0.1;
                    break; }
                case MAIN_DBC_BMS_CELL_OVERVIEW_FRAME_ID: {
                    main_dbc_bms_cell_overview_t cells;
                    main_dbc_bms_cell_overview_unpack(&cells, message.data, message.len);
                    data.bms_cellvoltages_min = main_dbc_bms_cell_overview_bms_overview_volt_min_decode(cells.bms_overview_volt_min);
                    data.bms_cellvoltages_max = main_dbc_bms_cell_overview_bms_overview_volt_max_decode(cells.bms_overview_volt_max);
                    data.bms_cellvoltages_avg = main_dbc_bms_cell_overview_bms_overview_volt_avg_decode(cells.bms_overview_volt_avg);
                    data.bms_maxtemp = main_dbc_bms_cell_overview_bms_overview_temp_max_decode(cells.bms_overview_temp_max);
                    data.bms_avgtemp = main_dbc_bms_cell_overview_bms_overview_temp_avg_decode(cells.bms_overview_temp_avg);
                    break; }
                case MAIN_DBC_BMS_CURRENT_FRAME_ID: {
                    main_dbc_bms_current_t current;
                    main_dbc_bms_current_unpack(&current, message.data, message.len);
                    data.bms_buscurrent = main_dbc_bms_current_bms_inst_current_filt_decode(current.bms_inst_current_filt);
                    if ((data.bms_buscurrent > data.bms_maxcurrent) || (isnan(data.bms_maxcurrent))) {
                        data.bms_maxcurrent = data.bms_buscurrent;
                    }
                    break; }
                case MAIN_DBC_VC_STATUS_FRAME_ID: {
                    main_dbc_vc_status_t vcstatus;
                    main_dbc_vc_status_unpack(&vcstatus, message.data, message.len);
                    data.vc_status = vcstatus.vc_status_vehicle_state;
                    break; }
                case  MAIN_DBC_VC_FAULT_VECTOR_FRAME_ID:
                    // Masked to keep other bits
                    data.vc_faultvector = *((uint32_t*)(message.data));
                    break;
                case MAIN_DBC_VC_INVERTER_STATUS_FRAME_ID: {
                    main_dbc_vc_inverter_status_t invstatus;
                    main_dbc_vc_inverter_status_unpack(&invstatus, message.data, message.len);
                    data.inv_state[INV_RR] = invstatus.vc_rr_status;
                    data.inv_state[INV_RL] = invstatus.vc_rl_status;
                    data.inv_state[INV_FR] = invstatus.vc_fr_status;
                    data.inv_state[INV_FL] = invstatus.vc_fl_status;
                    break; }
                case MAIN_DBC_VC_PROCESSED_INPUTS_FRAME_ID: {
                    main_dbc_vc_processed_inputs_t inputs;
                    main_dbc_vc_processed_inputs_unpack(&inputs, message.data, message.len);
                    data.fbps = main_dbc_vc_processed_inputs_vc_p_inputs_brakes_front_psi_decode(inputs.vc_p_inputs_brakes_front_psi);
                    data.rbps = main_dbc_vc_processed_inputs_vc_p_inputs_brakes_rear_psi_decode(inputs.vc_p_inputs_brakes_rear_psi);
                    break; }
                /*case MAIN_DBC_VC_HARD_FAULT_INDICATOR_FRAME_ID: {
                    main_dbc_vc_hard_fault_indicator_t fault;
                    main_dbc_vc_hard_fault_indicator_unpack(&fault, message.data, message.len);
                    uint8_t faulted = fault.vc_hard_fault_indicator_task != 0;
                    uint8_t mask = 0x10;
                    data.vc_faultvector = (data.vc_faultvector & ~mask) |
                                                ((faulted << 4) & mask);
                    Serial.printf("VC hardfault = %d\n", fault.vc_hard_fault_indicator_task);
                    Serial.printf("VC fault vec = 0x%02x\n", data.vc_faultvector);
                    // Masked to keep other bits
                    break; }*/
                case MAIN_DBC_BMS_FAULT_VECTOR_FRAME_ID: {
                    // This is not as DBC-agnostic as I would like,
                    //        but it's the best way I see to do it.
                    data.bms_faultvector = message.data[0] + (message.data[1] << 8);
                    break; }
                case MAIN_DBC_BMS_LIMP_MODE_FRAME_ID: {
                    main_dbc_bms_limp_mode_t limpmode;
                    main_dbc_bms_limp_mode_unpack(&limpmode, message.data, message.len);
                    uint8_t volt_limp = limpmode.bms_limp_mode_voltage_limp_level;
                    uint8_t temp_limp = limpmode.bms_limp_mode_temp_limp_level;
                    uint8_t overall_limp;

                    if(volt_limp > temp_limp) {
                        data.bms_limplevel = volt_limp;
                    }
                    else {
                        data.bms_limplevel = temp_limp;
                    }
                    break; }
                /*case MAIN_DBC_C70_TIRE_TEMPS_FRAME_ID: {
                    main_dbc_c70_tire_temps_t temps;
                    main_dbc_c70_tire_temps_unpack(&temps, message.data, message.len);
                    data.tiretemp_fl = main_dbc_c70_tire_temps_tire_temp_fl_max_decode(temps.tire_temp_fl_max);
                    data.tiretemp_fr = main_dbc_c70_tire_temps_tire_temp_fr_max_decode(temps.tire_temp_fr_max);
                    data.tiretemp_rl = main_dbc_c70_tire_temps_tire_temp_rl_max_decode(temps.tire_temp_rl_max);
                    data.tiretemp_rr = main_dbc_c70_tire_temps_tire_temp_rr_max_decode(temps.tire_temp_rr_max);
                    break;
                }
                case MAIN_DBC_C70_ROTOR_TEMPS_FRAME_ID: {
                    main_dbc_c70_rotor_temps_t temps;
                    main_dbc_c70_rotor_temps_unpack(&temps, message.data, message.len);
                    float rotortemp_fl = main_dbc_c70_rotor_temps_rotor_temp_fl_max_decode(temps.rotor_temp_fl_max);
                    float rotortemp_fr = main_dbc_c70_rotor_temps_rotor_temp_fr_max_decode(temps.rotor_temp_fr_max);
                    float rotortemp_rl = main_dbc_c70_rotor_temps_rotor_temp_rl_max_decode(temps.rotor_temp_rl_max);
                    float rotortemp_rr = main_dbc_c70_rotor_temps_rotor_temp_rr_max_decode(temps.rotor_temp_rr_max);
                    float rotortemp_max = fmax(fmax(rotortemp_fl, rotortemp_fr), fmax(rotortemp_rl, rotortemp_rr));
                    data.rotortemp = rotortemp_max;
                    break;
                }*/
                case MAIN_DBC_PBX_STATUS_FRAME_ID: {
                    main_dbc_pbx_status_t status;
                    main_dbc_pbx_status_unpack(&status, message.data, message.len);
                    data.lv_voltage = main_dbc_pbx_status_pbx_lv_voltage_decode(status.pbx_lv_voltage);
                    break; }

                case MAIN_DBC_VC_FL_AMK_ACTUAL_1_FRAME_ID: {
                    main_dbc_vc_fl_amk_actual_1_t val_fl;
                    main_dbc_vc_fl_amk_actual_1_unpack(&val_fl, message.data, message.len);
                    data.torque[INV_FL] = main_dbc_vc_fl_amk_actual_1_vc_fl_feedback_torque_decode(val_fl.vc_fl_feedback_torque);
                    break; }
                case MAIN_DBC_VC_FL_INFO_1_FRAME_ID: {
                    main_dbc_vc_fl_info_1_t val1_fl;
                    main_dbc_vc_fl_info_1_unpack(&val1_fl, message.data, message.len);
                    data.inv_fault[INV_FL].error_list1 = val1_fl.vc_fl_error_list1;
                    data.inv_fault[INV_FL].error_list2 = val1_fl.vc_fl_error_list2;
                    break; }
                case MAIN_DBC_VC_FL_INFO_2_FRAME_ID: {
                    main_dbc_vc_fl_info_2_t val2_fl;
                    main_dbc_vc_fl_info_2_unpack(&val2_fl, message.data, message.len);
                    data.inv_fault[INV_FL].error_info = val2_fl.vc_fl_error_info;
                    data.inv_fault[INV_FL].error_list3 = val2_fl.vc_fl_error_list3;
                    data.inv_temp[INV_FL] = main_dbc_vc_fl_info_2_vc_fl_temp_inverter_decode(val2_fl.vc_fl_temp_inverter);
                    break; }
                case MAIN_DBC_VC_FL_INFO_3_FRAME_ID: {
                    main_dbc_vc_fl_info_3_t val3_fl;
                    main_dbc_vc_fl_info_3_unpack(&val3_fl, message.data, message.len);
                    data.motor_temp[INV_FL] = main_dbc_vc_fl_info_3_vc_fl_temp_motor_decode(val3_fl.vc_fl_temp_motor);
                    break; }

                case MAIN_DBC_VC_FR_AMK_ACTUAL_1_FRAME_ID: {
                    main_dbc_vc_fr_amk_actual_1_t val_fr;
                    main_dbc_vc_fr_amk_actual_1_unpack(&val_fr, message.data, message.len);
                    data.torque[INV_FR] = main_dbc_vc_fr_amk_actual_1_vc_fr_feedback_torque_decode(val_fr.vc_fr_feedback_torque);
                    break; }
                case MAIN_DBC_VC_FR_INFO_1_FRAME_ID: {
                    main_dbc_vc_fr_info_1_t val1_fr;
                    main_dbc_vc_fr_info_1_unpack(&val1_fr, message.data, message.len);
                    data.inv_fault[INV_FR].error_list1 = val1_fr.vc_fr_error_list1;
                    data.inv_fault[INV_FR].error_list2 = val1_fr.vc_fr_error_list2;
                    break; }
                case MAIN_DBC_VC_FR_INFO_2_FRAME_ID: {
                    main_dbc_vc_fr_info_2_t val2_fr;
                    main_dbc_vc_fr_info_2_unpack(&val2_fr, message.data, message.len);
                    data.inv_fault[INV_FR].error_info = val2_fr.vc_fr_error_info;
                    data.inv_fault[INV_FR].error_list3 = val2_fr.vc_fr_error_list3;
                    data.inv_temp[INV_FR] = main_dbc_vc_fr_info_2_vc_fr_temp_inverter_decode(val2_fr.vc_fr_temp_inverter);
                    break; }
                case MAIN_DBC_VC_FR_INFO_3_FRAME_ID: {
                    main_dbc_vc_fr_info_3_t val3_fr;
                    main_dbc_vc_fr_info_3_unpack(&val3_fr, message.data, message.len);
                    data.motor_temp[INV_FR] = main_dbc_vc_fr_info_3_vc_fr_temp_motor_decode(val3_fr.vc_fr_temp_motor);
                    break; }

                case MAIN_DBC_VC_RL_AMK_ACTUAL_1_FRAME_ID: {
                    main_dbc_vc_rl_amk_actual_1_t val_rl;
                    main_dbc_vc_rl_amk_actual_1_unpack(&val_rl, message.data, message.len);
                    data.torque[INV_RL] = main_dbc_vc_rl_amk_actual_1_vc_rl_feedback_torque_decode(val_rl.vc_rl_feedback_torque);
                    break; }
                case MAIN_DBC_VC_RL_INFO_1_FRAME_ID: {
                    main_dbc_vc_rl_info_1_t val1_rl;
                    main_dbc_vc_rl_info_1_unpack(&val1_rl, message.data, message.len);
                    data.inv_fault[INV_RL].error_list1 = val1_rl.vc_rl_error_list1;
                    data.inv_fault[INV_RL].error_list2 = val1_rl.vc_rl_error_list2;
                    break; }
                case MAIN_DBC_VC_RL_INFO_2_FRAME_ID: {
                    main_dbc_vc_rl_info_2_t val2_rl;
                    main_dbc_vc_rl_info_2_unpack(&val2_rl, message.data, message.len);
                    data.inv_fault[INV_RL].error_info = val2_rl.vc_rl_error_info;
                    data.inv_fault[INV_RL].error_list3 = val2_rl.vc_rl_error_list3;
                    data.inv_temp[INV_RL] = main_dbc_vc_rl_info_2_vc_rl_temp_inverter_decode(val2_rl.vc_rl_temp_inverter);
                    break; }
                case MAIN_DBC_VC_RL_INFO_3_FRAME_ID: {
                    main_dbc_vc_rl_info_3_t val3_rl;
                    main_dbc_vc_rl_info_3_unpack(&val3_rl, message.data, message.len);
                    data.motor_temp[INV_RL] = main_dbc_vc_rl_info_3_vc_rl_temp_motor_decode(val3_rl.vc_rl_temp_motor);
                    break; }

                case MAIN_DBC_VC_RR_AMK_ACTUAL_1_FRAME_ID: {
                    main_dbc_vc_rr_amk_actual_1_t val_rr;
                    main_dbc_vc_rr_amk_actual_1_unpack(&val_rr, message.data, message.len);
                    data.torque[INV_RR] = main_dbc_vc_rr_amk_actual_1_vc_rr_feedback_torque_decode(val_rr.vc_rr_feedback_torque);
                    break; }
                case MAIN_DBC_VC_RR_INFO_1_FRAME_ID: {
                    main_dbc_vc_rr_info_1_t val1_rr;
                    main_dbc_vc_rr_info_1_unpack(&val1_rr, message.data, message.len);
                    data.inv_fault[INV_RR].error_list1 = val1_rr.vc_rr_error_list1;
                    data.inv_fault[INV_RR].error_list2 = val1_rr.vc_rr_error_list2;
                    break; }
                case MAIN_DBC_VC_RR_INFO_2_FRAME_ID: {
                    main_dbc_vc_rr_info_2_t val2_rr;
                    main_dbc_vc_rr_info_2_unpack(&val2_rr, message.data, message.len);
                    data.inv_fault[INV_RR].error_info = val2_rr.vc_rr_error_info;
                    data.inv_fault[INV_RR].error_list3 = val2_rr.vc_rr_error_list3;
                    data.inv_temp[INV_RR] = main_dbc_vc_rr_info_2_vc_rr_temp_inverter_decode(val2_rr.vc_rr_temp_inverter);
                    break; }
                case MAIN_DBC_VC_RR_INFO_3_FRAME_ID: {
                    main_dbc_vc_rr_info_3_t val3_rr;
                    main_dbc_vc_rr_info_3_unpack(&val3_rr, message.data, message.len);
                    data.motor_temp[INV_RR] = main_dbc_vc_rr_info_3_vc_rr_temp_motor_decode(val3_rr.vc_rr_temp_motor);
                    break; }

                case MAIN_DBC_VC_TV_OUT_FRAME_ID:
                    data.tv_msg = *((uint64_t*)(message.data));
                    break;
                case MAIN_DBC_VC_TC_OUT_FRAME_ID:
                    data.tc_msg = *((uint64_t*)(message.data));
                    break;
            }
        }

        

        DisplayManager::update(data);
    }
}
