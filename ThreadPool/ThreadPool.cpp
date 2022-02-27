#include "ThreadPool.h"

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
                    while (this->_is_run && this->_tasks.empty())
                    {
                        this->_condition.wait(lock,
                                              [this]()
                                              {
                                                  return !this->_is_run;
                                              });
                    }

                    // 如果离开循环且队列为空
                    // 则说明_is_run为false
                    // 返回，结束线程
                    if (this->_tasks.empty())
                    {
                        return;
                    }

                    // 取出一个任务
                    std::function<void()> oneTask = this->_tasks.front();
                    this->_tasks.pop();
                    // 执行任务
                    oneTask();
                }
            });
    }
}

template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type>
ThreadPool::addTask(F &&func, Args &&...args)
{
    // 包裹用户传进来的函数
    using RtnType = typename std::result_of<F(Args...)>::type;
    std::packaged_task<RtnType()> usrTask(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    // 准备包裹返回值
    std::future<RtnType> res = usrTask.get_future();

    // 添加到队列
    std::unique_lock<std::mutex> lock(_mtx_tasks);
    _tasks.emplace(
        [task = std::move(usrTask)]()
        {
            task();
        });
    lock.unlock();

    // 通知等待的线程
    _condition.notify_one();

    // 返回包裹返回值的future
    return res;
}
