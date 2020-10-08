#ifndef TFT_DISPLAY_ITEM_H_
#define TFT_DISPLAY_ITEM_H_

#include "text-item/tft-text-item.h"
#include "rectangle-item/tft-rectangle-item.h"

class TFT_DISPLAY_ITEM
{
private:
    TFT_TEXT_ITEM myText;
    TFT_RECTANGLE_ITEM myRectangle;
    bool changed;

public:
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

    //bool operator==(TFT_DISPLAY_ITEM const *otherItemIntf);
};

#endif /* TFT_DISPLAY_ITEM_H_ */