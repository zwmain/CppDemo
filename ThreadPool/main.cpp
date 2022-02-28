#include "ThreadPool.h"
#include <iostream>

int test(int a)
{
    return a * a;
}

int main()
{
    ThreadPool pool(4);
    std::cout << pool.size() << std::endl;

    pool.start();

    std::vector<std::future<int>> resVec;
    for (int i = 0; i < 16; ++i)
    {
        auto res = pool.addTask(test, i);
        resVec.push_back(std::move(res));
    }

    for (auto &i : resVec)
    {
        std::cout << "结果：" << i.get() << std::endl;
    }

    return 0;
}
