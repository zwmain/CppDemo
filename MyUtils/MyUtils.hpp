#include <string>
#include <vector>

std::vector<std::string> splitString(const std::string &src_str, const std::string &separator);

// ----------------------------------------------------------------------------

std::vector<std::string> splitString(const std::string &src_str, const std::string &separator)
{
    std::vector<std::string> str_arr;
    if (src_str.empty())
    {
        return str_arr;
    }
    size_t last_pos = 0;
    while (true)
    {
        size_t cur_pos = src_str.find(separator, last_pos);
        if (std::string::npos == cur_pos)
        {
            str_arr.push_back(src_str.substr(last_pos));
            break;
        }
        else
        {
            str_arr.push_back(src_str.substr(last_pos, cur_pos - last_pos));
        }
        last_pos = cur_pos + separator.size();
    }
    return str_arr;
}
