//#pragma once

#include "tft-manager.h"
#include "data-manager.h"
#include "display-manager.h"
#include "lvgl.h"

namespace ScreenDebug {
	#ifdef LCD_res_480x272
		//left thing
		#define rpm_bar_sizeX 47*2
		#define rpm_bar_sizeY 156
		#define rpm_bar_align_offsetX 10
		#define rpm_bar_align_offsetY 10

		#define rpmlabel_align_offsetX 0
		#define rpmlabel_align_offsetY 5

		#define mphlabel_align_offsetX 0
		#define mphlabel_align_offsetY 5

		//statis display
		#define status_area_sizeX 210
		#define status_area_sizeY 180
		#define status_area_align_offsetX -70
		#define status_area_align_offsetY 10

		#define status_overall_align_offsetX 0
		#define status_overall_align_offsetY -38

		#define status_vcstatus_align_offsetX 0
		#define status_vcstatus_align_offsetY -13

		#define status_mcustatus_align_offsetX 0
		#define status_mcustatus_align_offsetY 12

		#define status_bmsstatus_align_offsetX 0
		#define status_bmsstatus_align_offsetY 37

		//BMS area box 
		#define bms_area_sizeX 190
		#define bms_area_sizeY 180
		#define bms_area_align_offsetX -10
		#define bms_area_align_offsetY 10

		#define bms_soc_label_align_offsetX 0
		#define bms_soc_label_align_offsetY -50

		#define bms_cellvoltage_label_align_offsetX 0
		#define bms_cellvoltage_label_align_offsetY -25

		#define bms_packvoltage_label_align_offsetX 0
		#define bms_packvoltage_label_align_offsetY 0

		#define bms_current_label_align_offsetX 0
		#define bms_current_label_align_offsetY 25

		#define bms_maxcurrent_label_align_offsetX 0
		#define bms_maxcurrent_label_align_offsetY 25


		//fault text area
		#define fault_area_sizeX 405	
		#define fault_area_sizeY 60
		#define fault_area_align_offsetX -10
		#define fault_area_align_offsetY -10
	#endif

	#ifdef LCD_res_800x480 
		//left thing
		#define rpm_bar_sizeX int(47*1.66)
		#define rpm_bar_sizeY int(156*1.77)
		#define rpm_bar_align_offsetX 10
		#define rpm_bar_align_offsetY 10

		#define rpmlabel_align_offsetX 0
		#define rpmlabel_align_offsetY 5

		#define mphlabel_align_offsetX 0
		#define mphlabel_align_offsetY 5

		//statis display
		#define status_area_sizeX 210*1.66 
		#define status_area_sizeY 180*1.77
		#define status_area_align_offsetX -70
		#define status_area_align_offsetY 10

		#define status_overall_align_offsetX 0
		#define status_overall_align_offsetY -38

		#define status_vcstatus_align_offsetX 0
		#define status_vcstatus_align_offsetY -13

		#define status_mcustatus_align_offsetX 0
		#define status_mcustatus_align_offsetY 12

		#define status_bmsstatus_align_offsetX 0
		#define status_bmsstatus_align_offsetY 37

		//BMS area box 
		#define bms_area_sizeX 190*1.66
		#define bms_area_sizeY 180*1.77
		#define bms_area_align_offsetX -10
		#define bms_area_align_offsetY 10

		#define bms_soc_label_align_offsetX 0
		#define bms_soc_label_align_offsetY -50

		#define bms_cellvoltage_label_align_offsetX 0
		#define bms_cellvoltage_label_align_offsetY -25

		#define bms_packvoltage_label_align_offsetX 0
		#define bms_packvoltage_label_align_offsetY 0

		#define bms_current_label_align_offsetX 0
		#define bms_current_label_align_offsetY 25

		#define bms_maxcurrent_label_align_offsetX 0
		#define bms_maxcurrent_label_align_offsetY 25


		//fault text area
		#define fault_area_sizeX 405*1.66	
		#define fault_area_sizeY 60*1.77
		#define fault_area_align_offsetX -10
		#define fault_area_align_offsetY -10	
	#endif
	lv_obj_t* init(DisplayManager::styles_t* styles);
	void update(DataManager::car_data_t data);
}

