#include <iostream>
#include <string>
#include <memory>

class DrinkMaker {
public:
    void makeDrink(const std::string& name)
    {
        std::cout << name << " is ready!" << std::endl;
    }
};

class Command {
public:
    virtual void execute() = 0;
};

class OrderCommand : public Command {
public:
    OrderCommand(const std::string& drinkName, DrinkMaker* receiver)
        : mName(drinkName)
        , mReceiver(receiver)
    {
    }
    void execute() override
    {
        if (mReceiver) {
            mReceiver->makeDrink(mName);
        }
    }

private:
    std::string mName;
    DrinkMaker* mReceiver = nullptr;
};

class OrderMachine {
public:
    void setCommand(std::shared_ptr<Command> cmd)
    {
        mCmd = cmd;
    }
    void executeOrder()
    {
        if (mCmd) {
            mCmd->execute();
        }
    }

private:
    std::shared_ptr<Command> mCmd;
};

int main()
{
    DrinkMaker mk;
    int i = 0;
    while (i-- > 0) {
        std::string name;
        std::cin >> name;
        std::shared_ptr<Command> cmd = std::make_shared<OrderCommand>(name, &mk);
        OrderMachine mch;
        mch.setCommand(cmd);
        mch.executeOrder();
    }
    return 0;
}
