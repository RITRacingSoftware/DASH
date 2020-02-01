#ifndef TFT_TEXT_ITEM_H_
#define TFT_TEXT_ITEM_H_

#include "../../../include/display-interfaces/display-item-intf.h"
#include <cstdint>
#include "etl/cstring.h"

#define MAX_STRING_SIZE 50

class TFT_TEXT_ITEM : public DISPLAY_ITEM_INTF {
private:
  const uint8_t my_font_size;
  uint32_t my_x;
  uint32_t my_y;
  uint16_t my_fore_color;
  uint16_t my_bg_color;
  etl::string<MAX_STRING_SIZE> my_text;

public:
  TFT_TEXT_ITEM(const uint8_t fontSize, uint32_t xCoord, uint32_t yCoord,
                uint16_t foreColor, uint16_t bgColor,
                etl::string<MAX_STRING_SIZE> text);

  ~TFT_TEXT_ITEM() = default;

  void updateText(etl::string<MAX_STRING_SIZE> my_text);

  void updateColor(uint16_t color);

  void updateLocation(uint32_t xCoord, uint32_t yCoord);

  void updateElement(Adafruit_RA8875 *const displayDriver) override;

  bool operator==(DISPLAY_ITEM_INTF const *otherItemIntf) const override;
};

#endif /* TFT_TEXT_ITEM_H_ */