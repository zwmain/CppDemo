#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

int globalVar = 0;

std::mutex mtx;

std::atomic<int> atoVar = 0;

void task1()
{
    for (int i = 0; i < 1000; ++i)
    {
        // 使用lock_guard可以自动加锁
        // 并在离开作用域后自动解锁
        // std::lock_guard<std::mutex> lock(mtx);

        // unique_lock更加灵活
        // 会有更多的成员函数操作锁
        std::unique_lock<std::mutex> lock(mtx);

        ++globalVar;
        ++atoVar;
        // 执行一些逻辑
        std::this_thread::sleep_for(std::chrono::microseconds(50));
        --atoVar;
        --globalVar;
    }
}

int main()
{
    std::thread t1(task1);
    std::thread t2(task1);

    t1.join();
    t2.join();

    std::cout << "最终值：" << globalVar << std::endl;

    return 0;
}
