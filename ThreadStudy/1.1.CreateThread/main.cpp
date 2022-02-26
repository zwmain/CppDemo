#include <iostream>
#include <thread>

void func(int a)
{
    std::cout << "第" << a++ << "输出" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

int main()
{
    int a = 0;
    std::thread t1(func, a);

    std::cout << "这里是主线程" << std::endl;
    // 返回实现支持的并发线程数
    std::cout << "支持的线程线程并发数量：" << t1.hardware_concurrency() << std::endl;

    t1.join();
    // t1.detach();

    std::cout << "主线程即将结束" << std::endl;

    return 0;
}
