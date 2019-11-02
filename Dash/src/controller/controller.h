#include "controller-intf.h"

class CONTROLLER : public CONTROLLER_INTF
{
private:
    /* data */
public:
    CONTROLLER (){}

    processData(uint8_t[8]) override;

};
