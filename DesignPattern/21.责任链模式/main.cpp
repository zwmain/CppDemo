#include <iostream>
#include <string>
#include <memory>

constexpr int MAX_DAYS_SUPERVISOR_CAN_APPROVE = 30;
constexpr int MAX_DAYS_MANAGER_CAN_APPROVE = 15;

class LeaveRequest {
public:
    LeaveRequest(const std::string& name, int days)
        : mName(name)
        , mDays(days)
    {
    }
    std::string& getName()
    {
        return mName;
    }
    int getDays()
    {
        return mDays;
    }

private:
    std::string mName;
    int mDays = 0;
};
typedef std::shared_ptr<LeaveRequest> LeaveReqPtr;

class LeaveHandler {
public:
    virtual void handleRequest(LeaveReqPtr req) = 0;
};
typedef std::shared_ptr<LeaveHandler> LeaveHdlPtr;

class Supervisor : public LeaveHandler {
public:
    Supervisor(LeaveHdlPtr nextHdl)
        : mNext(nextHdl)
    {
    }
    void handleRequest(LeaveReqPtr req) override
    {
        if (!req) {
            return;
        }
        if (req->getDays() <= MAX_DAYS_SUPERVISOR_CAN_APPROVE) {
            std::cout << req->getName() << " Approved by Supervisor." << std::endl;
        } else if (!mNext) {
            mNext->handleRequest(req);
        } else {
            std::cout << req->getName() << " Denied by Supervisor." << std::endl;
        }
    }

private:
    LeaveHdlPtr mNext;
};

class Manager : public LeaveHandler {
public:
    Manager(LeaveHdlPtr next)
        : mNext(next)
    {
    }
    void handleRequest(LeaveReqPtr req) override
    {
        if (!req) {
            return;
        }
        if (req->getDays() <= MAX_DAYS_MANAGER_CAN_APPROVE) {
            std::cout << req->getName() << " Approved by Manager." << std::endl;
        } else if (!mNext) {
            mNext->handleRequest(req);
        } else {
            std::cout << req->getName() << " Denied by Manager." << std::endl;
        }
    }

private:
    LeaveHdlPtr mNext;
};
