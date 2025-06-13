#include "config.h"

#ifndef GTK_INTERFACE_H
#define GTK_INTERFACE_H

extern uint16_t screen_data[TFT_SCREEN_PIXELS];

uint64_t time_usecs();
void update_screen_data();

#endif
