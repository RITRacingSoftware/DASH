#include "tft-rectangle-item.h"
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

TFT_RECTANGLE_ITEM::TFT_RECTANGLE_ITEM(uint32_t xCoord,
                             uint32_t yCoord,  uint32_t width, uint32_t height,
                             uint16_t color)
    : my_x(xCoord), my_y(yCoord), my_width(width), my_height(height),
      my_color(color) {}

void TFT_RECTANGLE_ITEM::updateElement(Adafruit_RA8875 *const displayDriver) {
  // Update the rectangle
  displayDriver->fillRect(this->my_x, this->my_y, this->my_width, this->my_height, this->my_color);
}

void TFT_RECTANGLE_ITEM::updateColor(uint16_t color) { this->my_color = color; }

void TFT_RECTANGLE_ITEM::updateLocation(uint32_t xCoord, uint32_t yCoord) {
  this->my_x = xCoord;
  this->my_y = yCoord;
}

void TFT_RECTANGLE_ITEM::updateSize(uint32_t width, uint32_t height) {
  this->my_width = width;
  this->my_height = height;
}


