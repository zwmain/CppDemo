#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>

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
    std::future<typename std::invoke_result<F, Args...>::type> addTask(F &&func, Args &&...args);

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

// ============================ 实现-Implement ================================

ThreadPool::ThreadPool(const size_t pool_size) : _pool_size(pool_size)
{
}

ThreadPool::~ThreadPool()
{
    // 关闭线程池
    std::unique_lock<std::mutex> lock(_mtx_tasks);
    _is_run = false;
    lock.unlock();
    _condition.notify_all();

    // 等待剩余线程执行完毕
    for (auto &i : _workers)
    {
        i.join();
    }
}

size_t ThreadPool::size() const
{
    return _pool_size;
}

void ThreadPool::start()
{
    for (size_t i = 0; i < _pool_size; ++i)
    {
        _workers.emplace_back(
            [this]()
            {
                while (this->_is_run)
                {
                    // 对任务队列加锁
                    std::unique_lock<std::mutex> lock(this->_mtx_tasks);
                    // 如果任务队列一直为空，则等待
                    // 如果后面函数返回true，则停止等待
                    while (this->_tasks.empty())
                    {
                        this->_condition.wait(lock,
                                              [this]()
                                              {
                                                  return !this->_is_run || !this->_tasks.empty();
                                              });
                        // 如果线程池已经关闭则退出线程
                        if(!this->_is_run)
                        {
                            return;
                        }
                    }

                    // 取出一个任务
                    std::function<void()> oneTask = this->_tasks.front();
                    this->_tasks.pop();
                    lock.unlock();

                    // 执行任务
                    oneTask();
                }
            });
    }
}

// C++17前可以使用注释内的返回值类型
// C++17后result_of被废除
// std::future<typename std::result_of<F(Args...)>::type>
template <class F, class... Args>
std::future<typename std::invoke_result<F, Args...>::type> ThreadPool::addTask(F &&func, Args &&...args)
{
    // 包裹用户传进来的函数
    using RtnType = typename std::invoke_result<F, Args...>::type;

    // std::packaged_task无法被复制，为了方便lambda捕获，所以使用shared_ptr
    std::shared_ptr<std::packaged_task<RtnType()>> usrTask =
        std::make_shared<std::packaged_task<RtnType()>>(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...));

    // 准备包裹返回值
    std::future<RtnType> res = usrTask->get_future();

    // 添加到队列
    std::unique_lock<std::mutex> lock(_mtx_tasks);
    _tasks.emplace(
        [usrTask]()
        {
            (*usrTask)();
        });
    lock.unlock();

    // 通知等待的线程
    _condition.notify_one();

    // 返回包裹返回值的future
    return res;
}
