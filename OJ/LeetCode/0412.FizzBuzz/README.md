# 0412.FizzBuzz

链接：[0412.FizzBuzz](https://leetcode.cn/problems/fizz-buzz/)

## 解法

判断倍数使用求模运算符

```c++
class Solution {
public:
    vector<string> fizzBuzz(int n)
    {
        vector<string> res;
        for (int i = 1; i <= n; ++i) {
            if (i % 3 == 0 && i % 5 == 0) {
                res.emplace_back("FizzBuzz");
            } else if (i % 3 == 0) {
                res.emplace_back("Fizz");
            } else if (i % 5 == 0) {
                res.emplace_back("Buzz");
            } else {
                res.push_back(to_string(i));
            }
        }
        return res;
    }
};

```
