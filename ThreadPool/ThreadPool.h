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

    ~ThreadPool();

    size_t size() const;

    void start();

    // addTask函数接受的参数转发一个函数和该函数的参数
    // addTask函数返回一个std::feature，一个将来的值
    // std::feature保存的值的类型为函数的返回值的类型
    template <class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type>
    addTask(F &&func, Args &&...args);

private:
    // 执行队列
    std::vector<std::thread> _workers;
    // 任务队列
    std::queue<std::function<void()>> _tasks;

    const size_t _pool_size = 0;

    // 同步
    std::mutex _mtx_tasks;              // 互斥量
    std::condition_variable _condition; // 条件变量
    bool _is_run = true;
};
