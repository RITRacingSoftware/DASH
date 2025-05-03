#pragma once

#include "data-manager.h"
#include "display-manager.h"
#include "lvgl.h"

LV_IMG_DECLARE(DebugScreen);
LV_IMG_DECLARE(wheelStatusNominal);
LV_IMG_DECLARE(wheelStatusError);
LV_IMG_DECLARE(carStatusBooting);
LV_IMG_DECLARE(carStatusNominal);
LV_IMG_DECLARE(carStatusFaulted);
LV_IMG_DECLARE(rtdStartPrecharge);
LV_IMG_DECLARE(rtdHVEnabled);
LV_IMG_DECLARE(rtdEndPrecharge);
LV_IMG_DECLARE(rtdReadyToDrive);

lv_obj_t** createImgObjArray(lv_obj_t** imgArray, lv_img_dsc_t* src);

namespace ScreenDebug {
	lv_obj_t* init(DisplayManager::styles_t* styles);
	void update(DataManager::car_data_t data);
}
