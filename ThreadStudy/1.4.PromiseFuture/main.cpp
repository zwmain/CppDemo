#include <iostream>
#include <thread>
#include <condition_variable>

std::mutex mtx;

void task(int a, int b, int &res)
{
    int ra = a * a;
    int rb = b * 2;
    std::unique_lock<std::mutex> lock(mtx);
    res = ra + rb;
}

int main()
{
    int res = 0;
    std::thread t1(task, 2, 3, std::ref(res));

    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "join之前：" << res << std::endl;
    lock.unlock();

    t1.join();

    lock.lock();
    std::cout << "join之后：" << res << std::endl;
    lock.unlock();

    return 0;
}
