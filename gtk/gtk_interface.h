#include "config.h"

#ifndef GTK_INTERFACE_H
#define GTK_INTERFACE_H

extern uint16_t screen_data[TFT_SCREEN_PIXELS];

unsigned int millis();
void update_screen_data();

#endif
