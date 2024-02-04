#include <iostream>

class TV {
public:
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual void switchChannel() = 0;
};

class SonyTV : public TV {
public:
    void turnOn() override
    {
        std::cout << "Sony TV is ON" << std::endl;
    }
    void turnOff() override
    {
        std::cout << "Sony TV is OFF" << std::endl;
    }
    void switchChannel()
    {
        std::cout << "Switch Sony TV channel" << std::endl;
    }
};

class TCLTV : public TV {
public:
    void turnOn() override
    {
        std::cout << "TCL TV is ON" << std::endl;
    }
    void turnOff() override
    {
        std::cout << "TCL TV is OFF" << std::endl;
    }
    void switchChannel()
    {
        std::cout << "Switch TCL TV channel" << std::endl;
    }
};

class RemoteControl {
public:
    RemoteControl(TV* tv)
        : mTV(tv)
    {
    }
    virtual void performOperation() = 0;

protected:
    TV* mTV = nullptr;
};

class PowerOperation : public RemoteControl {
public:
    PowerOperation(TV* tv)
        : RemoteControl(tv)
    {
    }
    void performOperation() override
    {
        if (mTV) {
            mTV->turnOn();
        }
    }
};

class OffOperation : public RemoteControl {
public:
    OffOperation(TV* tv)
        : RemoteControl(tv)
    {
    }
    void performOperation() override
    {
        if (mTV) {
            mTV->turnOff();
        }
    }
};

class ChannelSwitchOperation : public RemoteControl {
public:
    ChannelSwitchOperation(TV* tv)
        : RemoteControl(tv)
    {
    }
    void performOperation() override
    {
        if (mTV) {
            mTV->switchChannel();
        }
    }
};

int main()
{
    int brand = 0;
    std::cin >> brand;
    TV* tv = nullptr;
    if (brand == 0) {
        tv = new SonyTV;
    } else {
        tv = new TCLTV;
    }
    int oper = 0;
    std::cin >> oper;
    RemoteControl* ctrl = nullptr;
    if (oper == 0) {
        ctrl = new PowerOperation(tv);
    } else if (oper == 1) {
        ctrl = new OffOperation(tv);
    } else {
        ctrl = new ChannelSwitchOperation(tv);
    }
    ctrl->performOperation();
    delete tv;
    return 0;
}
