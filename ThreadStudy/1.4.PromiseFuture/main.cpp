#include <iostream>
#include <thread>
#include <future>

void task(int a, int b, std::promise<int> &res)
{
    int ra = a * a;
    int rb = b * 2;
    res.set_value(ra + rb);
}

int main()
{
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t1(task, 2, 3, std::ref(p));

    // 注意：get函数只能执行一次
    // 如果执行多次，程序会崩溃
    std::cout << "结果：" << f.get() << std::endl;

    t1.join();

    return 0;
}
