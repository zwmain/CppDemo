#include "main.h"
#include "Utils.hpp"
#include <iostream>

int main()
{
    std::string fp(INPUT_FILE);
    auto inp = zwn::inputList(fp);
    for (ListNode* head : inp) {
        Solution slu;
        ListNode* res = slu.middleNode(head);
        std::cout << res->val << std::endl;
    }
    zwn::destroyList(inp);
    return 0;
}
