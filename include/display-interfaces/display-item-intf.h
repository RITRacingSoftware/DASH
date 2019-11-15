class Adafruit_RA8875;

class DISPLAY_ITEM_INTF
{
public:
    DISPLAY_ITEM_INTF() = default;

    virtual ~DISPLAY_ITEM_INTF() = default;

    virtual void updateElement(Adafruit_RA8875 * const) = 0;

    virtual bool operator==(DISPLAY_ITEM_INTF const &) const = 0;
};