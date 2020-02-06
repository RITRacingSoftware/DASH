
#include "tft-display.h"
#include "Arduino.h"     //Temporary
#include "etl/cstring.h" //Temporary

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
  this->my_display_driver.fillScreen(RA8875_YELLOW); // Temporary
  this->my_display_driver.textMode();                // Temporary
  Serial.println("starting");
  // this->my_display_driver.setCursor(300, 300);
  // this->my_display_driver.textTransparent(RA8875_RED);
  etl::string<12> text = "What";
  this->my_display_driver.textWrite(text.c_str(), text.size());
}

void TFT_DISPLAY::addElement(DISPLAY_ITEM_INTF *element) {
  Serial.println("Pushing element");
  this->my_elements.push_back(element);
}

void TFT_DISPLAY::updateScreen() {
  // Blank screen
  this->my_display_driver.graphicsMode();
  this->my_display_driver.fillScreen(RA8875_BLACK);

  // Going in reverse ordeer since the item at the front should be on top
  Serial.println("updating");
  Serial.print("Begin: ");
  // Serial.println(this->my_elements.rbegin().base);
  Serial.print("End: ");
  // Serial.println(this->my_elements.rend());
  // for (auto element = this->my_elements.rbegin();
  //       element != this->my_elements.rend(); ++element) {
  //          Serial.println("here");
  //     (*element)->updateElement(&this->my_display_driver);
  //  }
  // Serial.print("Size: ");
  // Serial.println(this->my_elements.size());
  //  for (int i = 0; i < this->my_elements.size(); i++ ){
  //    Serial.println("here");
  //    this->my_elements[i]->updateElement(&this->my_display_driver);
  // }
  // this->my_elements[0]->updateElement(&this->my_display_driver);
  etl::string<14> text = "shits fuckeddd";
  this->my_display_driver.textMode();
  // this->my_display_driver.setCursor(300, 300);
  this->my_display_driver.textTransparent(RA8875_RED);
  this->my_display_driver.textWrite("Hello world", 10);
}

bool TFT_DISPLAY::removeElement(DISPLAY_ITEM_INTF const *element) {
  for (auto i = this->my_elements.begin(); i != this->my_elements.end(); i++) {
    // Double derefernce bc dereferencing the iterator to get the pointer
    if (**i == element) {
      // this->my_elements.erase(i);
      return true;
    }
  }
  return false;
}

etl::vector<DISPLAY_ITEM_INTF *, MAX_ELEMENTS>
TFT_DISPLAY::getElements() const {
  return this->my_elements;
}
