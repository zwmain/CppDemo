#include <iostream>
#include <string>
#include <stack>
#include <memory>

class Memento {
public:
    Memento(int val)
        : mValue(val)
    {
    }

    int getValue()
    {
        return mValue;
    }

private:
    int mValue = 0;
};
typedef std::shared_ptr<Memento> MementoPtr;

class Counter {
public:
    Counter() = default;
    ~Counter() = default;
    void increment()
    {
        if (!mRedo.empty()) {
            std::stack<MementoPtr> temp;
            mRedo.swap(temp);
        }
        mUndo.push(std::make_shared<Memento>(mValue));
        ++mValue;
    }

    void decrement()
    {
        if (!mRedo.empty()) {
            std::stack<MementoPtr> temp;
            mRedo.swap(temp);
        }
        mUndo.push(std::make_shared<Memento>(mValue));
        --mValue;
    }

    void undo()
    {
        if (!mUndo.empty()) {
            mRedo.push(std::make_shared<Memento>(mValue));
            mValue = mUndo.top()->getValue();
            mUndo.pop();
        }
    }

    void redo()
    {
        if (!mRedo.empty()) {
            mUndo.push(std::make_shared<Memento>(mValue));
            mValue = mRedo.top()->getValue();
            mRedo.pop();
        }
    }

    int getValue()
    {
        return mValue;
    }

private:
    int mValue = 0;
    std::stack<MementoPtr> mUndo;
    std::stack<MementoPtr> mRedo;
};

int main()
{
    return 0;
}
