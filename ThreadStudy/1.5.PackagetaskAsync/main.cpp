#include <iostream>
#include <thread>
#include <future>

int task(int a, int b)
{
    int ra = a * a;
    int rb = b * 2;
    int res = ra + rb;
    return res;
}

int main()
{
    std::future<int> res = std::async(task, 2, 3);

    std::cout << "结果：" << res.get() << std::endl;

    return 0;
}
