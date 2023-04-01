#include <coroutine>
#include <iostream>
#include <optional>

template <typename T>
struct MyCoroGenerator {
    /**
     * @brief C++协程要求Generator必须有promise_type这个类型，名字也必须是promise_type
     * 最方便的方法就是定义在Generator类内部
     *
     */
    struct promise_type {
        /**
         * @brief 存放协程返回值
         * 由Generator从获取并返回
         */
        std::optional<T> opt;

        /**
         * @brief 协程是否创建时就被挂起，函数名字也必须是initial_suspend
         * std::suspend_always、std::suspend_never是标准库里面已经定义好的类型，前者表示总是挂起，后者表示从不挂起
         *
         * @return std::suspend_always 协程创建即挂起
         */
        std::suspend_always initial_suspend() const
        {
            return {};
        }

        /**
         * @brief 协程最后一次执行是否挂起，函数名字也必须是final_suspend
         * 由于final_suspend是收尾阶段的工作，所以必须是noexcept
         *
         * @return std::suspend_always 协程最后一次执行也被挂起
         */
        std::suspend_always final_suspend() const noexcept
        {
            return {};
        }

        /**
         * @brief 处理协程中未捕获异常，函数名字必须是unhandled_exception
         *
         */
        void unhandled_exception()
        {
            std::exit(EXIT_FAILURE);
        }

        /**
         * @brief 获取一个Generator对象，该对象从promise_type构造
         *
         * @return MyCoroGenerator
         */
        MyCoroGenerator get_return_object()
        {
            return MyCoroGenerator { std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        /**
         * @brief 定制yield_value接口，接收co_yield返回的值
         *
         * @tparam Arg 值的类型
         * @param arg co_yield返回值
         * @return std::suspend_always 执行完后继续挂起
         */
        template <typename Arg>
        std::suspend_always yield_value(Arg&& arg)
        {
            opt.emplace(std::forward<Arg>(arg));
            return {};
        }

        /**
         * @brief 当协程结束co_return且没有返回值时，调用该函数
         * 还有一个return_value(expr)函数，负责处理协程结束且有返回值的情况
         */
        void return_void()
        {
        }
    };

    /**
     * @brief 协程句柄，存储了协程上下文，一个非常底层的东西，没有RAII
     * 用MyCoroGenerator包裹
     */
    std::coroutine_handle<promise_type> handle;

    /**
     * @brief 默认构造函数
     *
     */
    MyCoroGenerator() = default;

    /**
     * @brief 通过一个handle构造一个Generator
     *
     * @param h 从promise_type构造出来的协程句柄
     */
    MyCoroGenerator(std::coroutine_handle<promise_type> h)
        : handle(h)
    {
    }

    /**
     * @brief 移动构造函数
     *
     * @param other 其他Generator对象
     */
    MyCoroGenerator(MyCoroGenerator&& other)
    {
        if (handle) {
            handle.destroy();
        }
        handle = other.handle;
        other.handle = nullptr;
    }

    /**
     * @brief 析构函数
     *
     */
    ~MyCoroGenerator()
    {
        if (handle) {
            handle.destroy();
        }
    }

    /**
     * @brief 移动赋值函数
     *
     * @param other 其他Generator对象
     * @return MyCoroGenerator& 当前镀锡
     */
    MyCoroGenerator& operator=(MyCoroGenerator&& other)
    {
        if (handle) {
            handle.destroy();
        }
        handle = other.handle;
        other.handle = nullptr;
        return *this;
    }

    /**
     * @brief 继续执行协程，并返回执行结果
     *
     * @return T& 返回的值
     */
    T& next()
    {
        handle.resume();
        if (handle.done()) {
            // throw geneator_done("Generator done");
            throw "Generator Error";
        }
        return *(handle.promise().opt);
    }

private:
    MyCoroGenerator(const MyCoroGenerator&) = delete;
    MyCoroGenerator& operator=(const MyCoroGenerator&) = delete;
};

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
