#include "../../include/display-interfaces/display-intf.h"

class TFT_DISPLAY : public DISPLAY_INTF
{
private:
    etl::vector<DISPLAY_ITEM_INTF *const, MAX_ELEMENTS> my_elements;

public:
    TFT_DISPLAY();
    ~TFT_DISPLAY();

    void addElement(DISPLAY_ITEM_INTF *const element) override;

    void updateScreen() override;

    bool removeElement(DISPLAY_ITEM_INTF const &element) override;

    etl::vector<DISPLAY_ITEM_INTF *const, MAX_ELEMENTS> getElements() const override;
};