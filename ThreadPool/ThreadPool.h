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

    ~ThreadPool() = default;

    size_t size() const;

    // enqueue函数接受的参数转发一个函数和该函数的参数
    // enqueue函数返回一个std::feature，一个将来的值
    // std::feature保存的值的类型为函数的返回值的类型
    template <class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type>
    enqueue(F &&func, Args &&...args);

private:
    // 需要跟踪线程以便加入它们
    std::vector<std::thread> _workers;
    // 任务队列
    std::queue<std::function<void()>> _tasks;

    const size_t _pool_size = 0;

    // 同步
    std::mutex _queue_mutex;            // 互斥量
    std::condition_variable _condition; // 条件变量
    bool _stop = false;
};
