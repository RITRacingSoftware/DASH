#include "tft-display-item.h"

TFT_DISPLAY_ITEM::TFT_DISPLAY_ITEM(TFT_TEXT_ITEM text, TFT_RECTANGLE_ITEM rectangle)
    : myText{text}, myRectangle{rectangle} { this->changed = true; }

void TFT_DISPLAY_ITEM::updateElement(Adafruit_RA8875 *const driver)
{
    myRectangle.updateElement(driver);
    myText.updateElement(driver);
    this->changed = false;
}

void TFT_DISPLAY_ITEM::updateText(char my_text[MAX_STRING_SIZE])
{
    myText.updateText(my_text);
    this->changed = true;
}

void TFT_DISPLAY_ITEM::updateTextColor(uint16_t color)
{
    myText.updateColor(color);
    this->changed = true;
}

void TFT_DISPLAY_ITEM::updateTextLocation(uint32_t xCoord, uint32_t yCoord)
{
    myText.updateLocation(xCoord, yCoord);
    this->changed = true;
}

bool TFT_DISPLAY_ITEM::getChanged()
{
    return this->changed == true;
}

// bool operator==(TFT_DISPLAY_ITEM const *otherItem){
//     if (otherItem)
//     {
//         return (this->myText == otherItem->myText &&
//             this->myRectangle == otherItem->myReactangle;);
//     }
//     return false;

// }
