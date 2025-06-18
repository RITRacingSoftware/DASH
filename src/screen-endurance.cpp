#include "screen-drive.h"

#ifdef DASH_TESTING
#include "testing_arduino.h"
#else
#include <Arduino.h>
#endif

#include <math.h>
#include "data-manager.h"
#include "display-manager.h"

#include "lvgl.h"

#define UPDATE_FLOAT(PARAM, ELEMENT, FMT) if((data.PARAM != lastdata.PARAM) && (!isnan(data.PARAM))) lv_label_set_text_fmt(elements.ELEMENT, FMT, data.PARAM)

#define ENDURANCE_BAR_POS 200
#define ENDURANCE_LABEL_YPOS 35
#define ENDURANCE_VALUE_YPOS 60
#define ENDURANCE_UNITS_YPOS 105

#define ENDURANCE_ENERGY_MAX 6200
#define ENDURANCE_VOLTAGE_MIN 3000
#define ENDURANCE_VOLTAGE_MAX 4350
#define ENDURANCE_TEMP_MIN 20
#define ENDURANCE_TEMP_MAX 60

namespace ScreenEndurance {
    DataManager::car_data_t lastdata;
	DisplayManager::endurance_styles_t* styles;


	lv_obj_t* screen;

	// Display elements to keep
	struct elements_s {
        lv_obj_t* background_image;

        lv_obj_t *energy_label;
        lv_obj_t *energy_bar;
        lv_obj_t *mincell_label;
        lv_obj_t *mincell_bar;
        lv_obj_t *maxtemp_label;
        lv_obj_t *maxtemp_bar;

	} elements;

	lv_style_t temp_cold_style;
	lv_style_t temp_optimal_style;
	lv_style_t temp_hot_style;
	lv_style_t text_style;

    bool fault_color = false; // Used to toggle the fault color on the status bar

	lv_obj_t* init(DisplayManager::endurance_styles_t* en) {
		Serial.printf("Initializing Endurance Screen\n");
        styles = en;
        //Styles
        lv_style_init(&temp_cold_style);
        lv_style_set_bg_color(&temp_cold_style, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_init(&temp_optimal_style);
        lv_style_set_bg_color(&temp_optimal_style, lv_palette_main(LV_PALETTE_GREEN));
        lv_style_init(&temp_hot_style);
        lv_style_set_bg_color(&temp_hot_style, lv_palette_main(LV_PALETTE_RED));


        screen = lv_obj_create(NULL);
        lv_obj_add_style(screen, &(en->text), LV_PART_MAIN);

        
		elements.energy_label = lv_label_create(screen);
        lv_obj_align(elements.energy_label, LV_ALIGN_TOP_LEFT, 10, ENDURANCE_VALUE_YPOS);
        lv_label_set_text(elements.energy_label, "?.??");
		lv_obj_add_style(elements.energy_label, &(en->text), LV_PART_MAIN);

        elements.energy_bar = lv_bar_create(screen);
        lv_obj_set_pos(elements.energy_bar, ENDURANCE_BAR_POS, 10);
        lv_obj_set_size(elements.energy_bar, 790 - ENDURANCE_BAR_POS, 140);
        lv_bar_set_range(elements.energy_bar, 0, ENDURANCE_ENERGY_MAX);
        lv_bar_set_value(elements.energy_bar, 0.0f, LV_ANIM_OFF);
        lv_obj_add_style(elements.energy_bar, &(en->bar), LV_PART_MAIN);
        lv_obj_add_style(elements.energy_bar, &(en->bar), LV_PART_INDICATOR);

		elements.mincell_label = lv_label_create(screen);
        lv_obj_align(elements.mincell_label, LV_ALIGN_TOP_LEFT, 10, 160+ENDURANCE_VALUE_YPOS);
        lv_label_set_text(elements.mincell_label, "?.??");
		lv_obj_add_style(elements.mincell_label, &(en->text), LV_PART_MAIN);
        
        elements.mincell_bar = lv_bar_create(screen);
        lv_obj_set_pos(elements.mincell_bar, ENDURANCE_BAR_POS, 170);
        lv_obj_set_size(elements.mincell_bar, 790 - ENDURANCE_BAR_POS, 140);
        lv_bar_set_range(elements.mincell_bar, ENDURANCE_VOLTAGE_MIN, ENDURANCE_VOLTAGE_MAX);
        lv_bar_set_value(elements.mincell_bar, ENDURANCE_VOLTAGE_MIN, LV_ANIM_OFF);
        lv_obj_add_style(elements.mincell_bar, &(en->bar), LV_PART_MAIN);
        lv_obj_add_style(elements.mincell_bar, &(en->bar), LV_PART_INDICATOR);

		elements.maxtemp_label = lv_label_create(screen);
        lv_obj_align(elements.maxtemp_label, LV_ALIGN_TOP_LEFT, 10, 320+ENDURANCE_VALUE_YPOS);
        lv_label_set_text(elements.maxtemp_label, "?.??");
		lv_obj_add_style(elements.maxtemp_label, &(en->text), LV_PART_MAIN);
        
        elements.maxtemp_bar = lv_bar_create(screen);
        lv_obj_set_pos(elements.maxtemp_bar, ENDURANCE_BAR_POS, 330);
        lv_obj_set_size(elements.maxtemp_bar, 790 - ENDURANCE_BAR_POS, 140);
        lv_bar_set_range(elements.maxtemp_bar, ENDURANCE_TEMP_MIN, ENDURANCE_TEMP_MAX);
        lv_bar_set_value(elements.maxtemp_bar, ENDURANCE_TEMP_MIN, LV_ANIM_OFF);
        lv_obj_add_style(elements.maxtemp_bar, &(en->bar), LV_PART_MAIN);
        lv_obj_add_style(elements.maxtemp_bar, &(en->bar), LV_PART_INDICATOR);

        lv_obj_t *temp;
        temp = lv_label_create(screen);
        lv_obj_align(temp, LV_ALIGN_TOP_LEFT, 10, ENDURANCE_LABEL_YPOS);
        lv_label_set_text(temp, "Energy");
        lv_obj_add_style(temp, &(en->small), LV_PART_MAIN);
        
        temp = lv_label_create(screen);
        lv_obj_align(temp, LV_ALIGN_TOP_LEFT, 10, ENDURANCE_UNITS_YPOS);
        lv_label_set_text(temp, "kWh");
        lv_obj_add_style(temp, &(en->small), LV_PART_MAIN);
        
        temp = lv_label_create(screen);
        lv_obj_align(temp, LV_ALIGN_TOP_LEFT, 10, 160+ENDURANCE_LABEL_YPOS);
        lv_label_set_text(temp, "Min cell voltage");
        lv_obj_add_style(temp, &(en->small), LV_PART_MAIN);
        
        temp = lv_label_create(screen);
        lv_obj_align(temp, LV_ALIGN_TOP_LEFT, 10, 160+ENDURANCE_UNITS_YPOS);
        lv_label_set_text(temp, "V");
        lv_obj_add_style(temp, &(en->small), LV_PART_MAIN);
        
        temp = lv_label_create(screen);
        lv_obj_align(temp, LV_ALIGN_TOP_LEFT, 10, 320+ENDURANCE_LABEL_YPOS);
        lv_label_set_text(temp, "Max cell temp");
        lv_obj_add_style(temp, &(en->small), LV_PART_MAIN);
        
        temp = lv_label_create(screen);
        lv_obj_align(temp, LV_ALIGN_TOP_LEFT, 10, 320+ENDURANCE_UNITS_YPOS);
        lv_label_set_text(temp, "degC");
        lv_obj_add_style(temp, &(en->small), LV_PART_MAIN);

		
		memset(&lastdata, 0xff, sizeof(lastdata));
        
		Serial.printf("Initialized Drive Screen\n");
		return screen;
	}

    bool vc_fault = false;
    bool bms_fault = false;
	
	void update(DataManager::car_data_t data) {
        if ((data.total_energy != lastdata.total_energy) && (!isnan(data.total_energy))) {
            lv_label_set_text_fmt(elements.energy_label, "%1.2f", data.total_energy);
            lv_bar_set_value(elements.energy_bar, data.total_energy*1000, LV_ANIM_OFF);
        }
        if ((data.bms_cellvoltages_min != lastdata.bms_cellvoltages_min) && (!isnan(data.bms_cellvoltages_min))) {
            lv_label_set_text_fmt(elements.mincell_label, "%1.2f", data.bms_cellvoltages_min);
            lv_bar_set_value(elements.mincell_bar, data.bms_cellvoltages_min*1000, LV_ANIM_OFF);
        }
        if ((data.bms_maxtemp != lastdata.bms_maxtemp) && (!isnan(data.bms_maxtemp))) {
            lv_label_set_text_fmt(elements.maxtemp_label, "%2.0f", (data.bms_maxtemp));
            lv_bar_set_value(elements.maxtemp_bar, data.bms_maxtemp, LV_ANIM_OFF);
        }
		
		lastdata = data;
	}
}
