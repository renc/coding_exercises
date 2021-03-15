#include <iostream>
#include <string>
#include <vector>
#include "common.hpp"

using namespace std;

// https://leetcode.com/problems/subsets/   input without duplicated elements
// https://leetcode.com/problems/subsets-ii/ input with duplicated elements     
//
class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        std::sort(nums.begin(), nums.end()); // sorted. 

    }
     
    vector<vector<int>> subsets(vector<int>& nums) {
        // without duplicate elements in input. do not need to sort 
        
        std::vector<std::vector<int>> result;
        if (1) 
        {
            std::vector<int> sofar; // empty; from left to right, sofar include what elements
            bt(nums, 0, sofar, result);
        }
        if (0) {
            std::vector<int> sofar; 
            bt2(nums, 0, sofar, result);
        }
        return result;
    }
    // this method works ok, for every element, 2 state (with and without). 
    void bt(std::vector<int>& nums, int i, std::vector<int>& sofar, std::vector<std::vector<int>>& result) {
        if (i <= nums.size() - 1) {
            //with this e[i]
            std::vector<int> w = sofar; w.push_back(nums[i]);
            bt(nums, i + 1, w, result);
            // without this e[i]
            bt(nums, i + 1, sofar, result);
        }
        else {
            result.push_back(sofar);
        }
    }

    // this method does not works. 
    void bt2(std::vector<int>& nums, int i, std::vector<int>& sofar, std::vector<std::vector<int>>& result) {
        result.push_back(sofar); 
        for (int k = i; k < nums.size(); ++k) {
            std::vector<int> w = sofar; w.push_back(nums[i]);
            bt2(nums, k + 1, w, result);
        } 
    }
};
int main()
{
    Solution s;
    std::vector<int> nums{ 1, 2, 3 };
    std::vector<std::vector<int> > r = s.subsets(nums);
    std::cout << util::toString(r) << std::endl;
}