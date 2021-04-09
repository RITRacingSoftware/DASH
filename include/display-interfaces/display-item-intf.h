#ifndef DISPLAY_ITEM_INTF_H_
#define DISPLAY_ITEM_INTF_H_

class Adafruit_RA8875;

class DISPLAY_ITEM_INTF {
public:
  DISPLAY_ITEM_INTF() = default;

  DISPLAY_ITEM_INTF(const DISPLAY_ITEM_INTF &) = default;

  virtual ~DISPLAY_ITEM_INTF() = default;

  virtual void updateElement(Adafruit_RA8875 *const) = 0;

  virtual bool getChanged() = 0;
  //virtual bool operator==(DISPLAY_ITEM_INTF const *) const = 0;
};

#endif /* DISPLAY_ITEM_INTF_H_ */