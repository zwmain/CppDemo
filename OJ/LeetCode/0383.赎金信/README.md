# 0383.赎金信

链接：[0383.赎金信](https://leetcode.cn/problems/ransom-note/)

## 我的解法

使用map统计magzine各个字符的数量

遍历ransomNote字符串，从map中查询每个字符串，如果查询不到，则返回false

如果查询到了，则将统计数目-1，减到0的时候，移除该字符

```c++
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine)
    {
        map<char, int> mag_map;
        for (auto c : magazine) {
            auto it = mag_map.find(c);
            if (it == mag_map.end()) {
                mag_map.emplace(c, 1);
            } else {
                it->second += 1;
            }
        }
        for (auto c : ransomNote) {
            auto it = mag_map.find(c);
            if (it == mag_map.end()) {
                return false;
            } else {
                it->second -= 1;
                if (it->second == 0) {
                    mag_map.erase(it);
                }
            }
        }
        return true;
    }
};

```

## 官方解法

依然使用统计字符数目的方法，但是不用map，而是直接用数组

因为只有26个小写英文字母，数组就可以满足需求，完全没必要使用map

```c++
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine)
    {
        if (ransomNote.size() > magazine.size()) {
            return false;
        }
        vector<int> arr(26);
        for (auto& c : magazine) {
            arr[c - 'a']++;
        }
        for (auto& c : ransomNote) {
            arr[c - 'a']--;
            if (arr[c - 'a'] < 0) {
                return false;
            }
        }
        return true;
    }
};

```
