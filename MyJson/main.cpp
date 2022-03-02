#include "MyJson.h"
#include <iostream>

int main()
{
    zwn::MyValue val = "这里是JSON";
    if (zwn::MyValue::STRING_VALUE == val.type())
    {
        std::cout << val.getString() << std::endl;
    }
    return 0;
}
