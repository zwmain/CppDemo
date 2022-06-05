#include "main.h"
#include "Utils.hpp"
#include <iostream>

int main()
{
    std::string fp(INPUT_FILE);
    auto inp = zwn::inputIntArr(fp);
    Solution slu;
    int res = slu.maximumWealth(inp);
    cout << res << endl;
    return 0;
}
