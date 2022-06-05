# 1672.最富有客户的资产总量

链接：[1672.最富有客户的资产总量](https://leetcode.cn/problems/richest-customer-wealth/)

## 解法

设置一个max初始值为0

遍历二维数组，将每个数组求和，并与max比较，更新max值

最后结果就是最大值

## 源码

```c++
class Solution {
public:
    int maximumWealth(vector<vector<int>>& accounts)
    {
        int max = 0;
        for (auto& arr : accounts) {
            int sum = accumulate(arr.begin(), arr.end(), 0);
            max = max > sum ? max : sum;
        }
        return max;
    }
};

```
