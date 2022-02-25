#include "ThreadPool.h"

ThreadPool::ThreadPool(const size_t pool_size) : _pool_size(pool_size)
{
    // todo
}

size_t ThreadPool::size() const
{
    return _pool_size;
}

template <class F, class... Args>
std::future<typename std::result_of<F(Args...)>::type>
    ThreadPool::enqueue(F &&func, Args &&...args)
{
    // todo
}
