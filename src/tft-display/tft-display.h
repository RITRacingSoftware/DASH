#ifndef TFT_DISPLAY_H_
#define TFT_DISPLAY_H_

#include "../../include/display-interfaces/display-intf.h"
#include "Adafruit_RA8875.h"

class TFT_DISPLAY : public DISPLAY_INTF
{
private:
  etl::vector<TFT_DISPLAY_ITEM *, MAX_ELEMENTS> my_elements;
  Adafruit_RA8875 my_display_driver;
  int cycles;

public:
  TFT_DISPLAY(uint8_t CSPin, uint8_t resetPin);
  ~TFT_DISPLAY() = default;

  void addElement(TFT_DISPLAY_ITEM *element) override;

  void updateScreen() override;

  bool removeElement(TFT_DISPLAY_ITEM *element) override; //removed const

  etl::vector<TFT_DISPLAY_ITEM *, MAX_ELEMENTS> getElements() const override;
};

#endif /* TFT_DISPLAY_H_ */
