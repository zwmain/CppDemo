#include "main.h"
#include <iostream>

int main()
{
    string inp1("aa");
    string inp2("aab");
    Solution slu;
    bool res = slu.canConstruct(inp1, inp2);
    cout << res << endl;
    return 0;
}
