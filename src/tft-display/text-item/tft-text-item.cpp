#include "tft-text-item.h"
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "stdio.h"

TFT_TEXT_ITEM::TFT_TEXT_ITEM(const uint8_t fontSize, uint32_t xCoord,
                             uint32_t yCoord, uint16_t foreColor,
                             uint16_t bgColor,
                             char text[MAX_STRING_SIZE])
    : my_font_size(fontSize), my_x(xCoord), my_y(yCoord),
      my_fore_color(foreColor), my_bg_color(bgColor)
      {
          strncpy(this->my_text, text, MAX_STRING_SIZE);
          this->changed = true;
      }

void TFT_TEXT_ITEM::updateElement(Adafruit_RA8875 *const displayDriver) {
  // Update the colors
  displayDriver->textSetCursor(this->my_x, this->my_y);
  displayDriver->textEnlarge(this->my_font_size);
  displayDriver->textTransparent(this->my_fore_color);


  // Note: .c_str() gets a pointer to the begining of the string, adn .size()
  // gets the size of the actual string, not just the size of the container
  //Serial.printf("Updating text %s, address=%p, address of me=%p\n", this->my_text, this->my_text, this);
  displayDriver->textWrite(this->my_text, MAX_STRING_SIZE);
  this->changed = false;
}

void TFT_TEXT_ITEM::updateText(char text[MAX_STRING_SIZE]) {
  //etl::string<MAX_STRING_SIZE> *pointer = &this->my_text;
  Serial.println("hereeee");
  if(this->my_text == NULL){
    Serial.println("null");
  }
  else{
    Serial.print("not null");
    Serial.printf("%s\n", text);
    strncpy(this->my_text, text, MAX_STRING_SIZE);
    Serial.printf("new text = %s, address of text %p, address of me=%p\n", this->my_text, this->my_text, this);
  }
  
}

void TFT_TEXT_ITEM::updateColor(uint16_t color) { this->my_fore_color = color; }

void TFT_TEXT_ITEM::updateLocation(uint32_t xCoord, uint32_t yCoord) {
  this->my_x = xCoord;
  this->my_y = yCoord;
}

bool TFT_TEXT_ITEM::operator==(DISPLAY_ITEM_INTF const *otherItemIntf) const{

    TFT_TEXT_ITEM * otherItem = dynamic_cast<TFT_TEXT_ITEM *>(otherItem);

    if (otherItem)
    {
        return (this->my_font_size == otherItem->my_font_size &&
            this->my_x == otherItem->my_x && this->my_y == otherItem->my_y &&
            this->my_text == otherItem->my_text &&
            this->my_bg_color == otherItem->my_bg_color &&
            this->my_fore_color == otherItem->my_fore_color);
    }
    
    return false;
  }
  
bool TFT_TEXT_ITEM::getChanged(){
  return changed;
}