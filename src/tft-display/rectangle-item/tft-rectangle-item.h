#ifndef TFT_RECTANGLE_ITEM_H_
#define TFT_RECTANGLE_ITEM_H_

#include "../../../include/display-interfaces/display-item-intf.h"
#include <cstdint>



class TFT_RECTANGLE_ITEM : public DISPLAY_ITEM_INTF {
private:
  uint32_t my_x;
  uint32_t my_y;
  uint32_t my_width;
  uint32_t my_height;
  uint16_t my_color;



public:
  TFT_RECTANGLE_ITEM(uint32_t xCoord, uint32_t yCoord, uint32_t width, uint32_t height,
                uint16_t color);

  ~TFT_RECTANGLE_ITEM() = default;

  void updateColor(uint16_t color);

  void updateLocation(uint32_t xCoord, uint32_t yCoord);

  void updateSize(uint32_t width, uint32_t height);

  void updateElement(Adafruit_RA8875 *const displayDriver) override;

  bool operator==(TFT_RECTANGLE_ITEM const &otherItem) {
    return (this->my_x == otherItem.my_x && this->my_y == otherItem.my_y &&
            this->my_width == otherItem.my_width &&
            this->my_height == otherItem.my_height &&
            this->my_color == otherItem.my_color);
  }
};

#endif /* TFT_RECTANGLE_ITEM_H_ */