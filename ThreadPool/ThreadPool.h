#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
public:
    explicit ThreadPool(const size_t pool_size = 0);

private:
    // 需要跟踪线程以便加入它们
    std::vector<std::thread> _workers;
    // 任务队列
    std::queue<std::function<void()>> _tasks;

    // 同步
    std::mutex _queue_mutex; // 互斥量
    std::condition_variable _condition; // 条件变量
    bool stop = false;
};
