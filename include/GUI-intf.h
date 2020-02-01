#ifndef GUI_INTF_H_
#define GUI_INTF_H_

class GUI
{
public:
    GUI() = default;
    virtual ~GUI() = default;

    virtual void update() = 0;
};

#endif