#include <iostream>
#include <thread>
#include <future>
#include <functional>

int task(int a, int b)
{
    int ra = a * a;
    int rb = b * 2;
    int res = ra + rb;
    return res;
}

int main()
{
    // std::future<int> res = std::async(task, 2, 3);

    // std::cout << "结果：" << res.get() << std::endl;

    std::packaged_task<int(int, int)> t(task);

    std::future<int> f = t.get_future();

    // 如果packaged_task构造的时候仅仅传入原始函数
    // 那么他可以向普通函数一样被传入thread中
    // 注意这里必须用std::move，否则会报错
    std::thread t1(std::move(t), 2, 3);

    std::cout << "结果" << f.get() << std::endl;

    t1.join();

    return 0;
}
