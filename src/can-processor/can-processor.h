#ifndef CAN_PROCESSOR_H_
#define CAN_PROCESSOR_H_

#include "can-processor-intf.h"
#include "etl/array.h"

class CAN_PROCESSOR : public CAN_PROCESSOR_INTF {
  private:
  //  etl::map<const uint16_t, uint32_t,
  //          12> temp;
  //etl::array<uint16_t, 20> ids;
  etl::array<uint32_t, 20> numMessages;
  
public:
  CAN_PROCESSOR();
  ~CAN_PROCESSOR(){};

  bool readCAN(CAN_MESSAGE &msg) override;

};

#endif /* CAN_PROCESSOR_H_ */