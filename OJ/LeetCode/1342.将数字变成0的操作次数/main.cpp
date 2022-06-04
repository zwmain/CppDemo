#include "main.h"
#include "Utils.hpp"
#include <iostream>

int main()
{
    std::string fp(INPUT_FILE);
    auto inp_arr = zwn::inputInt(fp);
    for (int i : inp_arr) {
        Solution slu;
        int res = slu.numberOfSteps(i);
        std::cout << res << std::endl;
    }
    return 0;
}
