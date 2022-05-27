#include <vector>

using namespace std;

class Solution
{
public:
    vector<int> runningSum(vector<int> &nums)
    {
        int last_sum = 0;
        for (int &i : nums)
        {
            i = i + last_sum;
            last_sum = i;
        }
        return nums;
    }
};
