#include "Utils.hpp"
#include <vector>

using namespace std;

#define INPUT_FILE "../input.txt"

class Solution {
public:
    ListNode* middleNode(ListNode* head)
    {
        ListNode* p_slow = head;
        ListNode* p_quick = head;
        while (p_quick && p_quick->next) {
            p_slow = p_slow->next;
            p_quick = p_quick->next;
            if (p_quick) {
                p_quick = p_quick->next;
            }
        }
        return p_slow;
    }
};
