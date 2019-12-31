#include "tft-text-item.h"
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

TFT_TEXT_ITEM::TFT_TEXT_ITEM(const uint8_t fontSize, uint32_t xCoord,
                             uint32_t yCoord, uint16_t foreColor,
                             uint16_t bgColor,
                             etl::string<MAX_STRING_SIZE> text)
    : my_font_size(fontSize), my_x(xCoord), my_y(yCoord),
      my_fore_color(foreColor), my_bg_color(bgColor), my_text(text) {}

void TFT_TEXT_ITEM::updateElement(Adafruit_RA8875 *const displayDriver) {
  // Update the colors
  displayDriver->setCursor(this->my_x, this->my_y);
  displayDriver->textTransparent(this->my_fore_color);

  // Note: .c_str() gets a pointer to the begining of the string, adn .size()
  // gets the size of the actual string, not just the size of the container
  displayDriver->textWrite(this->my_text.c_str(), this->my_text.size());
}

void TFT_TEXT_ITEM::updateText(etl::string<MAX_STRING_SIZE> text) {
  this->my_text = text;
}

void TFT_TEXT_ITEM::updateColor(uint16_t color) { this->my_fore_color = color; }

void TFT_TEXT_ITEM::updateLocation(uint32_t xCoord, uint32_t yCoord) {
  this->my_x = xCoord;
  this->my_y = yCoord;
}
