#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <sstream>
#include <functional>

/**
 * @brief 分割字符串
 *
 * @param src_str 待分割的字符串
 * @param separator 分隔字符串
 * @return 字符串数组
 */
std::vector<std::string> splitString(const std::string &src_str, const std::string &separator);

/**
 * @brief 使用正则表达式分割字符串
 *
 * @param src_str 待分割待字符串
 * @param sep_reg 分隔符正则表达式
 * @return 字符串数组
 */
std::vector<std::string> splitString(const std::string &src_str, const std::regex &sep_reg);

/**
 * @brief 使用换行符分割字符串
 *
 * @param src_str 待分割的字符串
 * @return 字符串数组
 */
std::vector<std::string> splitStringWithLineBreak(const std::string &src_str);

/**
 * @brief 以文本形式读取整个文件
 *
 * @param fp 文件路径
 * @return 文本内容字符串
 */
std::string readFileAsTxt(const std::string &fp);

/**
 * @brief 按行遍历文件
 *
 * @param fp 文件路径
 * @param func 回调函数，接收字符串作为参数，返回布尔表示是否继续遍历
 */
void traverseFileByLine(const std::string &fp, std::function<bool(const std::string &)> func);

// ----------------------------------------------------------------------------

std::vector<std::string> splitString(const std::string &src_str, const std::string &separator)
{
    std::vector<std::string> str_arr;
    // 如果传入字符串为空，直接返回空数组
    if (src_str.empty())
    {
        return str_arr;
    }
    // 记录上一次搜索的位置，最初为0
    size_t last_pos = 0;
    while (true)
    {
        // 在字符串中从last_pos位置开始查找分隔符的位置
        size_t cur_pos = src_str.find(separator, last_pos);
        if (std::string::npos == cur_pos)
        {
            // 如果返回npos说明已经查找不到分隔字符串了
            str_arr.push_back(src_str.substr(last_pos));
            break;
        }
        else
        {
            // 从上次查找的位置开始，截取字符串
            str_arr.push_back(src_str.substr(last_pos, cur_pos - last_pos));
        }
        // 跳过分隔字符串长度
        last_pos = cur_pos + separator.size();
    }
    return str_arr;
}

std::vector<std::string> splitString(const std::string &src_str, const std::regex &sep_reg)
{
    std::sregex_token_iterator beg(src_str.begin(), src_str.end(), sep_reg, -1);
    std::sregex_token_iterator end;
    return std::vector<std::string>(beg, end);
}

std::vector<std::string> splitStringWithLineBreak(const std::string &src_str)
{
    // 这种方式效率极低，而且还无法正常运行
    // std::regex reg("\\r\\n|\\n|\\r");
    // return splitString(src_str, reg);

    const std::string crlf("\r\n");
    const std::string lf("\n");
    const std::string cr("\r");
    if (std::string::npos != src_str.find(crlf))
    {
        return splitString(src_str, crlf);
    }
    else if (std::string::npos != src_str.find(lf))
    {
        return splitString(src_str, lf);
    }
    else if (std::string::npos != src_str.find(cr))
    {
        return splitString(src_str, cr);
    }
    else
    {
        return std::vector<std::string>();
    }
}

std::string readFileAsTxt(const std::string &fp)
{
    std::ifstream fi(fp);
    if (!fi.is_open())
    {
        return std::string();
    }
    std::ostringstream oss;
    oss << fi.rdbuf();
    return oss.str();
}

void traverseFileByLine(const std::string &fp, std::function<bool(const std::string &)> func)
{
    std::ifstream fi(fp);
    if (!func || !fi.is_open())
    {
        return;
    }
    while (fi.peek() != EOF)
    {
        std::string line_str;
        std::getline(fi, line_str);
        bool is_continue = func(line_str);
        if (!is_continue)
        {
            break;
        }
    }
}
