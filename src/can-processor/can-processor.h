#ifndef CAN_PROCESSOR_H_
#define CAN_PROCESSOR_H_

#include "can-processor-intf.h"

class CAN_PROCESSOR : public CAN_PROCESSOR_INTF {
public:
  CAN_PROCESSOR();
  ~CAN_PROCESSOR(){};

  bool readCAN(CAN_MESSAGE &msg) override;

};

#endif /* CAN_PROCESSOR_H_ */