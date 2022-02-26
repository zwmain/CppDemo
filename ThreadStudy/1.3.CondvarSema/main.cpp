#include <iostream>
#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <semaphore>

std::mutex mtx;
std::deque<int> q;
std::condition_variable cv;

std::counting_semaphore<4> csma;
std::binary_semaphore bsma;

// producer
void task1()
{
    int i = 0;
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        q.push_back(i);
        // 当队列有元素后通知正在等待的线程
        cv.notify_one();
        i < 10000 ? ++i : i = 0;
    }
}

// consumer
void task2()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);

        while (q.empty())
        {
            // 让线程处于休眠状态
            // 此时会解锁
            cv.wait(lock);
        }

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
    csma.acquire();
    bsma.acquire();
    std::thread t1(task1);
    std::thread t2(task2);
    bsma.release();
    csma.release();

    t1.join();
    t2.join();

    return 0;
}
