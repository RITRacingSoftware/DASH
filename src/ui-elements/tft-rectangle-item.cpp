#include "tft-rectangle-item.h"
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

TFT_RECTANGLE_ITEM::TFT_RECTANGLE_ITEM(uint32_t xCoord,
                             uint32_t yCoord,  uint32_t width, uint32_t height,
                             uint16_t color)
    : my_x(xCoord), my_y(yCoord), my_width(width), my_height(height),
      my_color(color), my_prev_height(height), my_prev_width(width) {}

void TFT_RECTANGLE_ITEM::updateElement(Adafruit_RA8875 *const displayDriver) {
  // Update the rectangle
  displayDriver->fillRect(this->my_x, this->my_y, this->my_prev_width, this->my_prev_height, RA8875_BLACK);
  displayDriver->fillRect(this->my_x, this->my_y, this->my_width, this->my_height, this->my_color);
}

void TFT_RECTANGLE_ITEM::updateColor(uint16_t color) { this->my_color = color; }

void TFT_RECTANGLE_ITEM::updateLocation(uint32_t xCoord, uint32_t yCoord) {
  this->my_x = xCoord;
  this->my_y = yCoord;
}

void TFT_RECTANGLE_ITEM::updateSize(uint32_t width, uint32_t height) {
  this->my_prev_width = this->my_width;
  this->my_prev_height = this->my_height;
  this->my_width = width;
  this->my_height = height;
}

// bool TFT_RECTANGLE_ITEM::operator==(DISPLAY_ITEM_INTF const *otherItemIntf) const{

//     TFT_RECTANGLE_ITEM * otherItem = dynamic_cast<TFT_RECTANGLE_ITEM *>(otherItem);

//     if (otherItem)
//     {
//         return (
//             this->my_x == otherItem->my_x && this->my_y == otherItem->my_y &&
//             this->my_width == otherItem->my_width &&
//             this->my_height == otherItem->my_height &&
//             this->my_color == otherItem->my_color);
//     }
    
//     return false;
//   }

  bool TFT_RECTANGLE_ITEM::getChanged()
  {
    return true;
  }
