#include <iostream>
#include <thread>
#include <mutex>
#include <deque>

std::mutex mtx;
std::deque<int> q;

// producer
void task1()
{
    int i = 0;
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        q.push_back(i);
        i < 10000 ? ++i : i = 0;
    }
}

// consumer
void task2()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (!q.empty())
        {
            int data = q.front();
            q.pop_front();
            std::cout << "得到数值：" << data << std::endl;
        }
    }
}

int main()
{
    std::thread t1(task1);
    std::thread t2(task2);

    t1.join();
    t2.join();

    return 0;
}
