/*
Maximum Sum Subarray of Size K (easy)

Problem Statement #
Given an array of positive numbers and a positive number ‘k’, find the maximum sum of any contiguous subarray of size ‘k’.

Example 1:

Input: [2, 1, 5, 1, 3, 2], k=3 
Output: 9
Explanation: Subarray with maximum sum is [5, 1, 3].
Example 2:

Input: [2, 3, 4, 1, 5], k=2 
Output: 7
Explanation: Subarray with maximum sum is [3, 4].

// --- 
brute force: O(N*k);

// --- 
sliding window 
Time Complexity, The time complexity of the above algorithm will be O(N).
Space Complexity, The algorithm runs in constant space O(1).
*/

#include <iostream>
#include <vector>

class MaxSumSubArrayOfSizeK {
public:

static int findMaxSumSubArray(int k, const std::vector<int>& arr)
{
    int windowSum = 0; 
    int maxSum = 0; 
    int windowStart = 0; 

    for (int windowEnd = 0; windowEnd < arr.size(); ++windowEnd) {
        windowSum += arr[windowEnd];

        if (windowEnd >= k-1) {
            maxSum = std::max(maxSum, windowSum);
            
            windowSum -= arr[windowStart];
            ++windowStart;
        }
    }
    return maxSum;
}

};