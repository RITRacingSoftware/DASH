#include "../include/controller-intf.h"

class TFT_CONTROLLER : public CONTROLLER_INTF
{

public:
    TFT_CONTROLLER() = default;
    ~TFT_CONTROLLER() = default;

    void initialize() override;

    void updateView() override;

    void updateModel() override;
};