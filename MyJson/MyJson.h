#pragma once
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace zwn
{
    class MyJson;
    class MyValue;

    class MyJson
    {
    public:
        MyJson();
        ~MyJson();

    private:
        std::map<std::string, MyValue> _json_data;
    };

    class MyValue
    {
    public:
        enum ValueType
        {
            NULL_VALUE,
            BOOL_VALUE,
            NUMBER_VALUE,
            STRING_VALUE,
            ARRAY_VALUE,
            OBJECT_VALUE
        };

        MyValue(std::nullptr_t val = nullptr);
        ~MyValue();

    private:
        std::variant<
            std::nullptr_t,
            bool,
            double,
            std::string,
            std::vector<MyValue>,
            MyJson>
            _data;
        ValueType _val_type = NULL_VALUE;
    };

} // namespace zwn
