#include <FastLED.h>

#define numled 20
#define LED_datapin 69

namespace ledControl {
	void init();
    void SOCsetColor(uint8_t soc, uint8_t limp);
}
