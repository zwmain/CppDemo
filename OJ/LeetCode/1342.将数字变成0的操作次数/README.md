# 1342.将数字变成0的操作次数

链接：[1342.将数字变成0的操作次数](https://leetcode.cn/problems/number-of-steps-to-reduce-a-number-to-zero/)

## 我的解法

没什么特殊的想法，就是按照题目意思将一个数减为0，统计次数s

## 官方解法

使用位运算

## 源码

```c++
class Solution {
public:
    int numberOfSteps(int num)
    {
        int cnt = 0;
        while (num != 0) {
            if (num % 2 == 0) {
                num /= 2;
            } else {
                num -= 1;
            }
            ++cnt;
        }
        return cnt;
    }
};

```
