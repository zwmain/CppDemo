#include "main.h"
#include "Utils.hpp"
#include <iostream>

int main()
{
    int n = 15;
    Solution slu;
    auto res = slu.fizzBuzz(n);
    zwn::outputStrArr(res);
    return 0;
}
