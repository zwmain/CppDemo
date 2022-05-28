// 刷题辅助工具库
#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include "../../../MyUtils/MyUtils.hpp"

namespace zwn {

/**
 * @brief 从文件中读取数组
 *
 * @param fp 文件路径
 * @return 整数数组
 */
std::vector<std::vector<int>> inputIntArr(const std::string& fp);

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

} // namespace zwn

#endif
