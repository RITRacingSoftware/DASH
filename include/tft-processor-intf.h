

class TFT_PROCESSOR_INTF
{
    public:
    TFT_PROCESSOR_INTF() {}

    virtual ~TFT_PROCESSOR_INTF() {}

    virtual void updateScreen() = 0;

    virtual void initializeCallbacks() = 0;
};