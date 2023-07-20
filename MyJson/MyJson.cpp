/**
 * @file MyJson.cpp
 * @author zwmain
 * @brief 简单的JSON实现
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "MyJson.h"
#include <utility>

namespace zwn {

MyJson::MyJson()
{
}

MyJson::MyJson(const MyJson& other)
    : _val(other._val)
    , _type(other._type)
{
}

MyJson::MyJson(MyJson&& other)
    : _val(std::move(other._val))
    , _type(other._type)
{
}

MyJson::MyJson(const std::map<std::string, MyJson>& val)
    : _type(JsonType::JSON_OBJECT)
{
    _val = val;
}

MyJson::MyJson(const std::vector<MyJson>& val)
    : _type(JsonType::JSON_ARRAY)
{
    _val = val;
}

MyJson::MyJson(const std::string_view& val)
    : _type(JsonType::JSON_STRING)
{
    _val = (std::string)val;
}

MyJson::MyJson(const char* val)
    : _type(JsonType::JSON_STRING)
{
    _val = std::string(val);
}

MyJson::MyJson(int val)
    : _type(JsonType::JSON_INT)
{
    _val = static_cast<int64_t>(val);
}

MyJson::MyJson(int64_t val)
    : _type(JsonType::JSON_INT)
{
    _val = val;
}

MyJson::MyJson(uint64_t val)
    : _type(JsonType::JSON_INT)
{
    _val = static_cast<int64_t>(val);
}

MyJson::MyJson(double val)
    : _type(JsonType::JSON_FLOAT)
{
    _val = val;
}

MyJson::MyJson(bool val)
    : _type(JsonType::JSON_BOOL)
{
    _val = val;
}

MyJson::~MyJson()
{
}

MyJson& MyJson::operator=(const MyJson& other)
{
    _type = other._type;
    _val = other._val;
    return *this;
}

MyJson& MyJson::operator=(MyJson&& other)
{
    _type = other._type;
    _val = std::move(other._val);
    return *this;
}

MyJson& MyJson::operator=(const std::map<std::string, MyJson>& val)
{
    _type = JsonType::JSON_OBJECT;
    _val = val;
    return *this;
}

MyJson& MyJson::operator=(const std::vector<MyJson>& val)
{
    _type = JsonType::JSON_ARRAY;
    _val = val;
    return *this;
}

MyJson& MyJson::operator=(const std::string_view& val)
{
    _type = JsonType::JSON_STRING;
    _val = (std::string)val;
    return *this;
}

MyJson& MyJson::operator=(const char* val)
{
    _type = JsonType::JSON_STRING;
    _val = std::string(val);
    return *this;
}

MyJson& MyJson::operator=(int val)
{
    _type = JsonType::JSON_INT;
    _val = static_cast<int64_t>(val);
    return *this;
}

MyJson& MyJson::operator=(int64_t val)
{
    _type = JsonType::JSON_INT;
    _val = val;
    return *this;
}

MyJson& MyJson::operator=(uint64_t val)
{
    _type = JsonType::JSON_INT;
    _val = static_cast<int64_t>(val);
    return *this;
}

MyJson& MyJson::operator=(double val)
{
    _type = JsonType::JSON_FLOAT;
    _val = val;
    return *this;
}

MyJson& MyJson::operator=(bool val)
{
    _type = JsonType::JSON_BOOL;
    _val = val;
    return *this;
}

JsonType MyJson::type() const
{
    return _type;
}

} // namespace zwn
