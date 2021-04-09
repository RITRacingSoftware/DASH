#ifndef TFT_DISPLAY_ITEM_H_
#define TFT_DISPLAY_ITEM_H_

#include "text-item/tft-text-item.h"
#include "rectangle-item/tft-rectangle-item.h"

class TFT_DISPLAY_ITEM : public DISPLAY_ITEM_INTF
{
private:
    TFT_TEXT_ITEM myText;
    TFT_RECTANGLE_ITEM myRectangle;
    bool changed;

public:
    TFT_DISPLAY_ITEM(const uint8_t fontSize, uint32_t xCoord, uint32_t yCoord, 
        uint32_t textBoxWidth, uint32_t textBoxHeight, uint16_t foreColor, uint16_t bgColor,
        char text[MAX_STRING_SIZE]);

    TFT_DISPLAY_ITEM(TFT_TEXT_ITEM text, TFT_RECTANGLE_ITEM rectangle);
    ~TFT_DISPLAY_ITEM() = default;

    void updateElement(Adafruit_RA8875 *const);

    void updateText(char my_text[MAX_STRING_SIZE]);

    void addText(char my_text[MAX_STRING_SIZE]);

    void updateTextColor(uint16_t color);

    void updateTextLocation(uint32_t xCoord, uint32_t yCoord);

    void updateRectangleSize(uint32_t width, uint32_t height);

    void updateRectangleLocation(uint32_t xCoord, uint32_t yCoord);

    bool getChanged();

    //bool operator==(DISPLAY_ITEM_INTF const *otherItemIntf);
};

#endif /* TFT_DISPLAY_ITEM_H_ */