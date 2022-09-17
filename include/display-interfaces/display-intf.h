#ifndef DISPLAY_INTF_H_
#define DISPLAY_INTF_H_

#include "etl/vector.h"
#include <Arduino.h>
#include "display-item-intf.h"
#include "ui-elements/tft-display-item.h"

#define MAX_ELEMENTS 50

class DISPLAY_INTF
{
public:
  DISPLAY_INTF() = default;
  virtual ~DISPLAY_INTF() = default;

  virtual void addElement(DISPLAY_ITEM_INTF *element) = 0;

  virtual void updateScreen() = 0;

  virtual bool removeElement(DISPLAY_ITEM_INTF *element) = 0; //removed const

  virtual etl::vector<DISPLAY_ITEM_INTF *, MAX_ELEMENTS>
  getElements() const = 0;

  virtual void clearScreen() = 0;
};

#endif /* DISPLAY_INTF_H_ */
