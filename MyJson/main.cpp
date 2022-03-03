#include "MyJson.h"
#include <iostream>

int main()
{
    zwn::MyValue val = "这里是JSON";
    zwn::MyValue::ValueType val_type = val.type();
    if (zwn::MyValue::STRING_VALUE == val_type)
    {
        std::cout << val.getString() << std::endl;
    }
    return 0;
}
