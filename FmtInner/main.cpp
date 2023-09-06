#include <chrono>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <iostream>
#include <numeric>
#include <string>

int main()
{
    auto curTime = std::chrono::system_clock::now();
    std::string strTime = fmt::format("当前时间为：{:%Y-%m-%d %H:%M:%S} -- {:%S}", curTime, curTime.time_since_epoch());
    std::cout << strTime << std::endl;
    std::cout << "============================================================" << std::endl;
    std::cout << fmt::format("{:%S}", curTime) << std::endl;
    std::cout << fmt::format("{:%S}", curTime.time_since_epoch()) << std::endl;

    std::cout << fmt::format("测试顺序：{:>0{}} -- {}", 3, 4, 5) << std::endl;

    std::cout << std::numeric_limits<uint64_t>::max() << std::endl;

    return 0;
}
