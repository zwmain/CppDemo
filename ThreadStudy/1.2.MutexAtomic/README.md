# 1.2.互斥量、原子变量

## 互斥量

`std::mutex`为标准库提供的互斥量类，包含`lock`和`unlock`成员函数

```c++
int globalVar = 0;

std::mutex mtx;

void task1()
{
    for (int i = 0; i < 1000; ++i)
    {
        mtx.lock(); // 加锁
        ++globalVar;
        // 执行一些逻辑
        std::this_thread::sleep_for(std::chrono::microseconds(50));
        --globalVar;
        mtx.unlock(); // 解锁
    }
}

```

上面这段代码就是使用`std::mutex`类的成员变量加锁和解锁

如果在加锁后的逻辑里遇到某些问题，程序返回了，那么资源就被锁住，没有释放

使用`std::lock_guard<std::mutex>`可以避免这种情况，他在构造的时候加锁，在析构的时候解锁

```c++
int globalVar = 0;

std::mutex mtx;

void task1()
{
    for (int i = 0; i < 1000; ++i)
    {
        // 使用lock_guard可以自动加锁
        // 并在离开作用域后自动解锁
        std::lock_guard<std::mutex> lock(mtx);
        ++globalVar;
        // 执行一些逻辑
        std::this_thread::sleep_for(std::chrono::microseconds(50));
        --globalVar;
    }
}

```

相比于`std::lock_guard<std::mutex>`，`std::unique_lock<std::mutex>`要更加灵活

后者有更多的成员变量，可以在需要的时候加锁和解锁，而不是只能在构造和析构的时候做

## 原子变量
