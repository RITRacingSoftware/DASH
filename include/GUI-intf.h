class GUI
{
public:
    GUI() = default;
    virtual ~GUI() = default;

    virtual void update() = 0;
};