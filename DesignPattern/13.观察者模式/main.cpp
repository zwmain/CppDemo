#include <iostream>
#include <memory>
#include <set>
#include <string>

class Observer {
public:
    Observer() = default;
    virtual ~Observer() = default;
    virtual void update(int hour) = 0;
};

class Subject {
public:
    virtual void registerObserver(std::shared_ptr<Observer> observer) = 0;
    virtual void removeObserver(std::shared_ptr<Observer> observer) = 0;
    virtual void notifyObserver() = 0;
};

class Clock : public Subject {
public:
    void registerObserver(std::shared_ptr<Observer> observer) override
    {
        mObservers.insert(observer);
    }
    void removeObserver(std::shared_ptr<Observer> observer) override
    {
        mObservers.erase(observer);
    }
    void notifyObserver() override
    {
        for (auto& o : mObservers) {
            o->update(mHour);
        }
    }
    void tick()
    {
        mHour = (mHour + 1) % 24;
    }

private:
    int mHour = 0;
    std::set<std::shared_ptr<Observer>> mObservers;
};

class Student : public Observer {
public:
    Student(const std::string& name)
        : mName(name)
    {
    }
    void update(int hour) override
    {
        std::cout << mName << "\t" << hour << std::endl;
    }

private:
    std::string mName;
};

int main()
{
    int stuNum = 0;
    std::cin >> stuNum;
    Clock clock;
    for (int i = 0; i < stuNum; ++stuNum) {
        std::string name;
        std::cin >> name;
        clock.registerObserver(std::make_shared<Student>(name));
    }
    int cnt = 0;
    std::cin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        clock.tick();
    }
    return 0;
}
