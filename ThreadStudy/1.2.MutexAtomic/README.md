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

## 原子变量
