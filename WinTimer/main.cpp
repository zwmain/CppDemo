#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#include <thread>

void cb(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    std::cout << "回调线程号：" << std::this_thread::get_id() << std::endl;
    std::cout << "计时器ID：" << uTimerID << std::endl;
}

int main()
{
    std::cout << "当前线程号：" << std::this_thread::get_id() << std::endl;
    MMRESULT tmId = timeSetEvent(500, 1, cb, NULL, TIME_ONESHOT);
    if (tmId == NULL) {
        std::cout << "定时器创建失败" << std::endl;
        return 0;
    }
    Sleep(1000);
    timeKillEvent(tmId);
    std::cout << "程序结束" << std::endl;
    return 0;
}
