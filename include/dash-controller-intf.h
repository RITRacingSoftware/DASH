#ifndef DASH_CONTROLLER_INTF_H_
#define DASH_CONTROLLER_INTF_H_

#include "etl/delegate.h"
#include "etl/array.h"

class DASH_CONTROLLER_INTF
{
public:

    DASH_CONTROLLER_INTF() {}
    virtual ~DASH_CONTROLLER_INTF() {}

    virtual void initialize() = 0;
    virtual void updateView() = 0;
    virtual void updateModel() = 0;

    virtual bool registerCallback() = 0;
};

#endif