#ifndef TFT_TEXT_ITEM_H_
#define TFT_TEXT_ITEM_H_

#include "../include/display-interfaces/display-item-intf.h"
#include <cstdint>
#include "etl/cstring.h"

#define MAX_STRING_SIZE 60

class TFT_TEXT_ITEM : public DISPLAY_ITEM_INTF
{
private:
  const uint8_t my_font_size;
  uint32_t my_x;
  uint32_t my_y;
  uint16_t my_fore_color;
  uint16_t my_bg_color;
  char my_text[MAX_STRING_SIZE];

public:
  TFT_TEXT_ITEM(const uint8_t fontSize, uint32_t xCoord, uint32_t yCoord,
                uint16_t foreColor, uint16_t bgColor,
                char text[MAX_STRING_SIZE]);

  ~TFT_TEXT_ITEM() = default;

  void updateText(char my_text[MAX_STRING_SIZE]);

  void updateColor(uint16_t color);

  void updateLocation(uint32_t xCoord, uint32_t yCoord);

  void updateElement(Adafruit_RA8875 *const displayDriver) override;

  void addText(char my_text[MAX_STRING_SIZE]); //Adds onto the end of the previous string instead of creating new string

  
  bool getChanged() override;

  //bool operator==(DISPLAY_ITEM_INTF const *otherItemIntf) const override;
};

#endif /* TFT_TEXT_ITEM_H_ */
