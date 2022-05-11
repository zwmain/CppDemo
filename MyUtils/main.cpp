#include "MyUtils.hpp"
#include <iostream>

int main()
{
    std::string txt("工具。学习。Utils。字符串。分割。Split。String。测试");
    auto res = splitString(txt, "。");
    for (const auto &i : res)
    {
        std::cout << i << "  ";
    }
    std::cout << std::endl;
    std::regex sep_reg("。");
    res = splitString(txt, sep_reg);
    for (const auto &i : res)
    {
        std::cout << i << "  ";
    }
    std::cout << std::endl;
    return 0;
}
