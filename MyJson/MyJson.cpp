#include "MyJson.h"

using namespace zwn;

MyJson::MyJson()
{
}

MyJson::~MyJson()
{
}

MyValue::MyValue() : _data(nullptr)
{
}

MyValue::MyValue(bool val) : _data(val), _val_type(BOOL_VALUE)
{
}

MyValue::MyValue(double val) : _data(val), _val_type(NUMBER_VALUE)
{
}

MyValue::MyValue(const std::string &val) : _data(val), _val_type(STRING_VALUE)
{
}

MyValue::MyValue(std::string &&val) : _data(val), _val_type(STRING_VALUE)
{
}

MyValue::MyValue(const std::vector<MyValue> &val) : _data(val), _val_type(ARRAY_VALUE)
{
}

MyValue::MyValue(std::vector<MyValue> &&val) : _data(val), _val_type(ARRAY_VALUE)
{
}

MyValue::MyValue(const MyJson &val) : _data(val), _val_type(OBJECT_VALUE)
{
}

MyValue::MyValue(MyJson &&val) : _data(val), _val_type(OBJECT_VALUE)
{
}

MyValue::~MyValue()
{
}

MyValue::ValueType MyValue::type() const
{
    return _val_type;
}

std::string &MyValue::getString()
{
    return std::get<std::string>(_data);
}
