#include <map>
#include <array>

class DATA_PROCESSOR_INTF
{
private:
    std::map<uint16_t, std::function<void(std::array<unit8_t, 8> const & data)>>
public:
    DATA_PROCESSOR_INTF(){}

    virtual ~DATA_PROCESSOR_INTF(){}

    virtual bool register(uint16_t const & id, std::function<void(std::array<uint8_t,8> const & data)>) = 0;

    virtual void processData() = 0;

};
