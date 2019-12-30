#ifndef CAN_PROCESSOR_H_
#define CAN_PROCESSOR_H_

#include "can-processor-intf.h"
#include "etl/queue.h"
//#include "FlexCan_T4.h"

#define MAX_QUEUE_SIZE 20

class CAN_PROCESSOR : public CAN_PROCESSOR_INTF {
private:
  // FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_16> FD;

public:
  CAN_PROCESSOR(){/*FD.begin();*/};
  ~CAN_PROCESSOR(){};

  bool readCAN(CAN_MESSAGE &msg) override;
};

#endif /* CAN_PROCESSOR_H_ */