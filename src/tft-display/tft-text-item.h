#include "../../include/display-interfaces/display-item-intf.h"
#include <cstdint>
#include "etl/cstring.h"

#define MAX_STRING_SIZE 50

class TFT_TEXT_ITEM : public DISPLAY_ITEM_INTF
{
private:
    const uint8_t my_font_size;
    uint32_t my_x;
    uint32_t my_y;
    uint16_t my_fore_color;
    uint16_t my_bg_color;
    etl::string<MAX_STRING_SIZE> my_text;

public:
    TFT_TEXT_ITEM(const uint8_t fontSize,
                  uint32_t xCoord,
                  uint32_t yCoord,
                  uint16_t foreColor,
                  uint16_t bgColor,
                  etl::string<MAX_STRING_SIZE> text);

    ~TFT_TEXT_ITEM() = default;

    void updateElement() override;

    bool operator==(TFT_TEXT_ITEM const &otherItem)
    {
        return (this->my_font_size == otherItem.my_font_size &&
                this->my_x == otherItem.my_x &&
                this->my_y == otherItem.my_y &&
                this->my_text == otherItem.my_text &&
                this->my_bg_color == otherItem.my_bg_color &&
                this->my_fore_color == otherItem.my_fore_color);
    }
};