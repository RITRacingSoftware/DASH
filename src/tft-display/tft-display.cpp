
#include "tft-display.h"

TFT_DISPLAY::TFT_DISPLAY()
{
}

void TFT_DISPLAY::addElement(DISPLAY_ITEM_INTF *const element)
{
    this->my_elements.push_back(element);
}

void TFT_DISPLAY::updateScreen()
{
    // Going in reverse ordeer since the item at the front should be on top
    for (auto element = this->my_elements.rbegin(); element != this->my_elements.rend(); element++)
    {
        (*element)->updateElement();
    }
}

bool TFT_DISPLAY::removeElement(DISPLAY_ITEM_INTF const &element)
{
    for (auto i = this->my_elements.begin(); i != this->my_elements.end(); i++)
    {
        // Double derefernce bc dereferencing the iterator to get the pointer
        if (**i == element)
        {
            this->my_elements.erase(i);
            return true;
        }
    }
    return false;
}

etl::vector<DISPLAY_ITEM_INTF *const, MAX_ELEMENTS> TFT_DISPLAY::getElements() const
{
    return this->my_elements;
}