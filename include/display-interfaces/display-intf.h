#include "etl/vector.h"
#include <Arduino.h>
#include "display-item-intf.h"

#define MAX_ELEMENTS 256

class DISPLAY_INTF
{
public:
    DISPLAY_INTF() = default;
    virtual ~DISPLAY_INTF() = default;

    virtual void addElement(DISPLAY_ITEM_INTF *const element) = 0;

    virtual void updateScreen() = 0;

    virtual bool removeElement(DISPLAY_ITEM_INTF const &element) = 0;

    virtual etl::vector<DISPLAY_ITEM_INTF *const, MAX_ELEMENTS> getElements() const = 0;
};