#include "tft-display-item.h"

TFT_DISPLAY_ITEM::TFT_DISPLAY_ITEM(TFT_TEXT_ITEM text, TFT_RECTANGLE_ITEM rectangle)
    : myText{text}, myRectangle{rectangle} { this->changed = true; }

TFT_DISPLAY_ITEM::TFT_DISPLAY_ITEM(const uint8_t fontSize, uint32_t xCoord, uint32_t yCoord, 
        uint32_t textBoxWidth, uint32_t textBoxHeight, uint16_t foreColor, uint16_t bgColor,
        char text[MAX_STRING_SIZE]) : myText(fontSize, xCoord, yCoord, foreColor, bgColor, text),
        myRectangle(xCoord, yCoord, textBoxWidth, textBoxHeight, bgColor) {}

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

void TFT_DISPLAY_ITEM::addText(char my_text[MAX_STRING_SIZE])
{
    myText.addText(my_text);
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
    return this->changed;
}

// bool TFT_DISPLAY_ITEM::operator==(DISPLAY_ITEM_INTF const *otherItem){
//     // if (otherItem)
//     // {
//     //     return (this->myText == otherItem->myText &&
//     //         this->myRectangle == otherItem->myReactangle;);
//     // }
//     return false;

// }

void TFT_DISPLAY_ITEM::updateRectangleSize(uint32_t width, uint32_t height)
{
    myRectangle.updateSize(width, height);
    this->changed = true;
}

void TFT_DISPLAY_ITEM::updateRectangleLocation(uint32_t xCoord, uint32_t yCoord)
{
    myRectangle.updateLocation(xCoord, yCoord);
    this->changed = true;
}

void TFT_DISPLAY_ITEM::updateRectangleColor(uint16_t color){
    myRectangle.updateColor(color);
    this->changed = true;
}