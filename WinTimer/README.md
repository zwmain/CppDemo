# Windows timeSetEvent定时器

参考[微软文档](https://learn.microsoft.com/en-us/previous-versions/dd757634(v=vs.85))

参考[博客](https://blog.csdn.net/a1173356881/article/details/79800686)

## 函数描述

```c++
MMRESULT timeSetEvent(
   UINT uDelay，
   UINT uResolution，
   LPTIMECALLBACK lpTimeProc，
   DWORD_PTR dwUser，
   UINT fuEvent
);
```

- `udelay`事件延迟，以毫秒为单位。如果该值不在定时器支持的最小和最大事件延迟的范围内，则该函数返回错误。最大值是1000秒。
- `uResolution`定时器事件的解析度，以毫秒为单位。分辨率随着较小的值而增加; 分辨率为0表示定期事件应该尽可能准确地发生。但是，为了减少系统开销，您应该使用适合您的应用程序的最大值。
- `lpTimeProc`指向一个回调函数的指针，该函数在单个事件到期时调用一次，或者在周期性事件到期时定期调用。如果`fuEvent`指定了`TIME_CALLBACK_EVENT_SET`或`TIME_CALLBACK_EVENT_PULSE`标志，则`lpTimeProc`参数将被解释为事件对象的句柄。该事件将在单个事件完成后定期执行，或定期事件完成后进行。对于`fuEvent`的任何其他值，`lpTimeProc`参数是指向`LPTIMECALLBACK`类型的回调函数的指针。
- `dwUser`用户提供的回调数据。
- `fuEvent`定时器事件类型。该参数可能包含以下值之一。
  - `TIME_ONESHOT`在uDelay毫秒后发生一次事件。
  - `TIME_PERIODIC`事件每uDelay毫秒发生一次。


## 回调函数

参考：[微软文档](https://learn.microsoft.com/en-us/previous-versions/ff728861(v=vs.85))

```c++
typedef void (CALLBACK TIMECALLBACK)(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
typedef TIMECALLBACK FAR *LPTIMECALLBACK;
```

- `uTimerID`，定时器ID
- `uMsg`，保留
- `dwUser`，用户自定义数据
- `dw1`，保留
- `dw2`，保留

## 头文件及库依赖

```c++
#include <Windows.h>
#include <mmsystem.h>
```

静态库`Winmm.lib`，动态库`Winmm.dll`

## 示例

```c++
#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#include <thread>

void cb(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    std::cout << "回调线程号：" << std::this_thread::get_id() << std::endl;
    std::cout << "计时器ID：" << uTimerID << std::endl;
}

int main()
{
    std::cout << "当前线程号：" << std::this_thread::get_id() << std::endl;
    MMRESULT tmId = timeSetEvent(500, 1, cb, NULL, TIME_ONESHOT);
    if (tmId == NULL) {
        std::cout << "定时器创建失败" << std::endl;
        return 0;
    }
    Sleep(1000);
    timeKillEvent(tmId);
    std::cout << "程序结束" << std::endl;
    return 0;
}

```


```cmake
cmake_minimum_required(VERSION 3.18)
project("Timer" LANGUAGES CXX)

if(MSVC)
    add_compile_options("/source-charset:utf-8")
endif()

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC "Winmm.lib")

```
