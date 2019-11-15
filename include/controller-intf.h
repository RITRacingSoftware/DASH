class CONTROLLER_INTF
{
public:
    virtual void initialize() = 0;
    virtual void updateView() = 0;
    virtual void updateModel() = 0;
};