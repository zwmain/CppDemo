# 0876.链表的中间节点

链接：[0876.链表的中间节点](https://leetcode.cn/problems/middle-of-the-linked-list/)

## 解法

快慢指针，快指针每次走两步，慢指针每次走一步

当快指针到结尾时，慢指针正好在中间

## 源码

```c++
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

```
