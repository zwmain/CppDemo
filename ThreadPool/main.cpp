#include "ThreadPool.h"
#include <iostream>

int main()
{
    ThreadPool pool(4);
    std::cout << pool.size() << std::endl;
    return 0;
}
