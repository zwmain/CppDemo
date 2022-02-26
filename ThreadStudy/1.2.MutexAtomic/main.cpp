#include <iostream>
#include <thread>

int globalVar = 0;

void task1()
{
    for (int i = 0; i < 1000; ++i)
    {
        ++globalVar;
        std::this_thread::sleep_for(std::chrono::microseconds(50));
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
