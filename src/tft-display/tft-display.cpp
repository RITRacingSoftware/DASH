
#include "tft-display.h"
#include "Arduino.h"     //Temporary
#include "etl/cstring.h" //Temporary
#include "text-item/tft-text-item.h"
//#include "tft-display-driver.h"

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
  this->my_display_driver.textMode();                // Temporary
  Serial.println("starting");
  // this->my_display_driver.setCursor(300, 300);
  // this->my_display_driver.textTransparent(RA8875_RED);
}

void TFT_DISPLAY::addElement(DISPLAY_ITEM_INTF *element) {
  Serial.println("Pushing element");
  Serial.printf("Address of object pushed list=%p\n", element);
  this->my_elements.push_back(element);
}

void TFT_DISPLAY::updateScreen() {

  // Going in reverse order since the item at the front should be on top
  //Check if text item has changed, if it has then update the associated recangle,
  //then update text element
  for (int i = this->my_elements.size()-1; i >= 0; i--) {
    if(i%2==0){
      TFT_TEXT_ITEM* textItem = (TFT_TEXT_ITEM*) this->my_elements[i];
      textItem->updateElement(&this->my_display_driver);
      bool print = false;
      if(textItem->getChanged()){
        print = true;
      }
      if(print){
        this->my_elements[i+1]->updateElement(&this->my_display_driver);
        this->my_elements[i]->updateElement(&this->my_display_driver);
      }
    }
  }

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
