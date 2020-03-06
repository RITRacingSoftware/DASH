#ifndef DISPLAY_INTF_H_
#define DISPLAY_INTF_H_

#include "etl/vector.h"
#include <Arduino.h>
#include "display-item-intf.h"
#include "tft-display/tft-display-item.h"

#define MAX_ELEMENTS 50

class DISPLAY_INTF
{
public:
  DISPLAY_INTF() = default;
  virtual ~DISPLAY_INTF() = default;

  virtual void addElement(TFT_DISPLAY_ITEM *element) = 0;

  virtual void updateScreen() = 0;

  virtual bool removeElement(TFT_DISPLAY_ITEM *element) = 0; //removed const

  virtual etl::vector<TFT_DISPLAY_ITEM *, MAX_ELEMENTS>
  getElements() const = 0;
};

#endif /* DISPLAY_INTF_H_ */
