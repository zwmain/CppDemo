#include "Utils.hpp"
#include <numeric>

using namespace std;

#define INPUT_FILE "../input.txt"

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
