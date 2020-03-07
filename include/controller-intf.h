class CONTROLLER_INTF
{
public:
    virtual void initialize() = 0;
    virtual void updateView() = 0;
    virtual void updateModel() = 0;
    virtual void registerCallback() = 0; //Might need to change parameters
    virtual void readyToDrive() = 0;
};