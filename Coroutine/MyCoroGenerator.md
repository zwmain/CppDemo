# C++20初探

## 认识协程

在C++中，协程就是一个可以暂停和恢复的函数。

包含`co_wait`、`co_yield`、`co_return`关键字的都可以叫协程。

看一个例子：

```c++
MyCoroGenerator<int> testFunc(int n)
{
    std::cout << "Begin testFunc" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << "TestFunc before yield " << i << std::endl;
        co_yield i;
        std::cout << "TestFunc after yield " << i << std::endl;
    }
    std::cout << "End testFunc" << std::endl;
}

int main()
{
    int inp = 10;
    std::cout << "Before testFunc" << std::endl;
    MyCoroGenerator<int> gen = testFunc(inp);
    std::cout << "After testFunc" << std::endl;
    for (int i = 0; i < inp; ++i) {
        std::cout << "Cur input: " << i << std::endl;
        std::cout << "Output value: " << gen.next() << std::endl;
        std::cout << "After input: " << i << std::endl;
    }
}

```

上面这段代码的执行结果是：

```txt
Before testFunc
After testFunc
Cur input: 0
Output value: Begin testFunc
TestFunc before yield 0
0
After input: 0
Cur input: 1
Output value: TestFunc after yield 0
TestFunc before yield 1
1
After input: 1
Cur input: 2
Output value: TestFunc after yield 1
TestFunc before yield 2
2
After input: 2
Cur input: 3
Output value: TestFunc after yield 2
TestFunc before yield 3
3
After input: 3
Cur input: 4
Output value: TestFunc after yield 3
TestFunc before yield 4
4
After input: 4
Cur input: 5
Output value: TestFunc after yield 4
TestFunc before yield 5
5
After input: 5

...

```

调用时发现，函数并没有一开始就执行，而是等到`next`时才开始执行。执行到`co_yield`就会自动暂停，下次`next`才会继续执行

另外，函数本身并没有返回什么，但是这里可以使用`MyCoroGenerator<int>`接收。函数的控制权也转移到了`MyCoroGenerator<int>`，需要执行时就调用`next`。

函数在暂停后执行，依然可以继续上次的状态，这是因为，协程在挂起期间，其上下文全部都被保留，下次执行时恢复。

## 协程句柄

`std::coroutine_handle`类模板是实现协程的最底层的工具，负责存储协程的句柄。他可以特化为`std::coroutine_handle<Promise>`或者`std::coroutine_handle<void>`。

这里面的`Promise`是实现协程必要的`Promise`类，而且其名字必须是`promise_type`。

协程会暂停，其上下文也会保留，`std::coroutine_handle`就是保存和管理协程的地方。

`std::coroutine_handle`中方法不多，但是每个都很重要，其中：

- `done`方法，可以查询协程是否已经结束
- `resume`，恢复一个协程的执行
- `destroy`，销毁一个协程

`std::coroutine_handle`是一个很底层的东西，没有RAII包裹，就像一个裸指针那样，需要手动创建、手动销毁。

所以需要一个包裹类，负责处理协程句柄的初始化和销毁，也就是Generator

## Generator

C++的协程要求Generator必须有`promise_type`这个类，名字也必须这个，不能是其他的名字，直接定义在Generator中最方便。

```c++
template <typename T>
struct MyCoroGenerator {
    struct promise_type {
        // todo
    };
};

```

`promise_type`中有一些固定接口，这些接口如果不实现，那么协程是不完整的。

- `initial_suspend`，协程是否在初始化结束后挂起，返回std::suspend_always、std::suspend_never，这是标准库里面已经定义好的类型，前者表示总是挂起，后者表示从不挂起
- `final_suspend`，协程最后一次执行是否挂起，返回值和上面函数相同。由于final_suspend是收尾阶段的工作，所以必须是noexcept
- `unhandled_exception`，处理协程中未被处理的异常
- `get_return_object`，返回一个Generator对象
- `yield_value`，处理协程返回值，就是`co_yield`传递过来的值
- `return_void`，处理协程结束后的返回值，和`return_value`同时只能存在一个，如果没有返回值就用`return_void`
- `return_value`，处理协程结束后返回值，和`return_void`同时只能存在一个，如果有返回值就用`return_value`

解释一下`yield_value`函数，`co_yield`实际是一个语法糖，相当于`co_wait promise.yield_value(i)`，只有实现了该函数，Genreator才能接收`co_yield`的返回值

函数的返回值，需要回答协程要不要挂起，也就是`std::suspend_always`或者`std::suspend_never`，因为需要`yield`后挂起，所以返回`std::suspend_always`。

为了接收返回值，需要用转发和`std::optional`，接收并存储返回值。

`get_return_object`负责创建一个Generator，一般来说，使用`std::coroutine_handle<promise_type>::from_promise`接口从一个Promise创建句柄，然后使用这个句柄创建Generator。

为了配合创建函数，MyGenerator需要实现一个接收句柄的构造函数。

参考：[C++ coroutine generator 实现笔记](https://zhuanlan.zhihu.com/p/590892907)
