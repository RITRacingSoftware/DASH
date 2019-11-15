
#include "tft-display.h"

TFT_DISPLAY::TFT_DISPLAY(uint8_t CSPin, uint8_t resetPin)
    : my_display_driver(CSPin, resetPin) {
  this->my_display_driver.begin(RA8875_480x272);

  this->my_display_driver.displayOn(true);
  this->my_display_driver.GPIOX(
      true); // Enable TFT - display enable tied to GPIOX
  this->my_display_driver.PWM1config(
      true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  this->my_display_driver.PWM1out(255);
  this->my_display_driver.fillScreen(RA8875_BLACK);
}

void TFT_DISPLAY::addElement(DISPLAY_ITEM_INTF *const element) {
  this->my_elements.push_back(element);
}

void TFT_DISPLAY::updateScreen() {
  // Blank screen

  // Going in reverse ordeer since the item at the front should be on top
  for (auto element = this->my_elements.rbegin();
       element != this->my_elements.rend(); element++) {
    (*element)->updateElement(&this->my_display_driver);
  }
}

bool TFT_DISPLAY::removeElement(DISPLAY_ITEM_INTF const &element) {
  for (auto i = this->my_elements.begin(); i != this->my_elements.end(); i++) {
    // Double derefernce bc dereferencing the iterator to get the pointer
    if (**i == element) {
      this->my_elements.erase(i);
      return true;
    }
  }
  return false;
}

etl::vector<DISPLAY_ITEM_INTF *const, MAX_ELEMENTS>
TFT_DISPLAY::getElements() const {
  return this->my_elements;
}
