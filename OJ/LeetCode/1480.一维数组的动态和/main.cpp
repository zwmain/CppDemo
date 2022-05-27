#include "main.h"
#include <iostream>

int main()
{
    vector<int> inp1({1, 2, 3, 4});
    Solution slu;
    auto res = slu.runningSum(inp1);
    for (auto &i : res)
    {
        cout << i << ",";
    }
    cout << endl;
    return 0;
}
