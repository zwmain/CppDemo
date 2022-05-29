#include <map>
#include <string>
#include <vector>

using namespace std;

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
