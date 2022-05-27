# 1480.一维数组的动态和

链接：[1480.一维数组的动态和](https://leetcode.cn/problems/running-sum-of-1d-array/)

由题目可以得知，第一个数就是它自己本身，从第二个数开始，每个数都是前面的结果加上自身。

如果把第一个数之前的结果设置为0，那么就有：

cur=cur+last_sum;

last_sum=cur;
