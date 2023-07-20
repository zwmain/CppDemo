/**
 * @file MyJson.h
 * @author zwmain
 * @brief 简单的JSON实现
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _MY_JSON_H_
#define _MY_JSON_H_

#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace zwn {

enum class JsonType {
    JSON_NULL,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_INT,
    JSON_FLOAT,
    JSON_BOOL
};

class MyJson {
public:
    MyJson();
    MyJson(const MyJson& other);
    MyJson(MyJson&& other);
    MyJson(const std::map<std::string, MyJson>& val);
    MyJson(const std::initializer_list<std::pair<std::string, MyJson>>& val);
    MyJson(const std::vector<MyJson>& val);
    MyJson(const std::string_view& val);
    MyJson(const char* val);
    MyJson(int val);
    MyJson(int64_t val);
    MyJson(uint64_t val);
    MyJson(double val);
    MyJson(bool val);
    ~MyJson();

    MyJson& operator=(const MyJson& other);
    MyJson& operator=(MyJson&& other);
    MyJson& operator=(const std::map<std::string, MyJson>& val);
    MyJson& operator=(const std::vector<MyJson>& val);
    MyJson& operator=(const std::string_view& val);
    MyJson& operator=(const char* val);
    MyJson& operator=(int val);
    MyJson& operator=(int64_t val);
    MyJson& operator=(uint64_t val);
    MyJson& operator=(double val);
    MyJson& operator=(bool val);

    JsonType type() const;

private:
    typedef std::variant<
        std::monostate,
        std::map<std::string, MyJson>,
        std::vector<MyJson>,
        std::string,
        int64_t,
        double,
        bool>
        ValT;
    ValT _val;
    JsonType _type = JsonType::JSON_NULL;
};

} // namespace zwn

#endif
