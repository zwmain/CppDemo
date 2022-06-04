/**
 * @file Utils.hpp
 * @author zwmain
 * @brief 刷题辅助工具库
 * @version 1.0
 * @date 2022-06-04
 *
 * @copyright Copyright (c) 2022 zwmain
 *
 */
#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include "../../../MyUtils/MyUtils.hpp"
#include <iostream>

/**
 * @brief 链表节点结构体
 *
 */
struct ListNode {
    int val;
    ListNode* next;
    ListNode()
        : val(0)
        , next(nullptr)
    {
    }
    ListNode(int x)
        : val(x)
        , next(nullptr)
    {
    }
    ListNode(int x, ListNode* next)
        : val(x)
        , next(next)
    {
    }
};

namespace zwn {

/**
 * @brief 从文件中读取数组
 *
 * @param fp 文件路径
 * @return 整数数组
 */
std::vector<std::vector<int>> inputIntArr(const std::string& fp);

/**
 * @brief 输出数组
 *
 * @param str_arr 字符串数组
 */
void outputStrArr(const std::vector<std::string>& str_arr);

// ----------------------------------------------------------------------------

std::vector<std::vector<int>> inputIntArr(const std::string& fp)
{
    std::vector<std::vector<int>> res;
    auto lines = readFileAsLine(fp);
    for (auto& line : lines) {
        auto arr = splitString(line, ",");
        std::vector<int> num_arr;
        for (auto& num : arr) {
            int val = std::stoi(num);
            num_arr.push_back(val);
        }
        res.push_back(std::move(num_arr));
    }
    return res;
}

void outputStrArr(const std::vector<std::string>& str_arr)
{
    bool is_first = true;
    std::cout << '[';
    for (auto& s : str_arr) {
        if (is_first) {
            std::cout << '"';
            std::cout << s;
            is_first = false;
        } else {
            std::cout << ",\"";
            std::cout << s;
        }
        std::cout << '"';
    }
    std::cout << ']' << std::endl;
}

} // namespace zwn

#endif
