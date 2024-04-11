/*
Smallest subarray with a given sum (easy)
Problem Statement #
Given an array of positive numbers and a positive number ‘S’, find the length of the smallest contiguous subarray whose sum is greater than or equal to ‘S’. Return 0, if no such subarray exists.

Example 1:

Input: [2, 1, 5, 2, 3, 2], S=7 
Output: 2
Explanation: The smallest subarray with a sum great than or equal to '7' is [5, 2].
Example 2:

Input: [2, 1, 5, 2, 8], S=7 
Output: 1
Explanation: The smallest subarray with a sum greater than or equal to '7' is [8].
Example 3:

Input: [3, 4, 1, 1, 6], S=8 
Output: 3
Explanation: Smallest subarrays with a sum greater than or equal to '8' are [3, 4, 1] or [1, 1, 6].


//---
sliding window, 
time complex O(N+N) which is O(N)
*/

#include <iostream>
#include <limits>
#include <vector>

class MinSizeSubArraySum {
public:
static int findMinSubArray(int S, const std::vector<int>& arr) {
    int windowStart = 0, windowSum = 0; 
    int minLen = std::numeric_limits<int>::max();

    for (int windowEnd = 0; windowEnd < arr.size(); ++windowEnd) {
        // keep sum up until >= S
        windowSum += arr[windowEnd];

        while (windowSum >= S) { // meet the >= S requirement,
            // then need to move sliding windows, how ? move left side, or move right side ?
            // if move rigt side, sum increase, len become longer
            // so moveleft, until < S, which will trigger move right 
            minLen = std::min(minLen, windowEnd - windowStart + 1);

            windowSum -= arr[windowStart];
            ++windowStart;
        }
    }

    return (minLen == std::numeric_limits<int>::max()) ? 0 : minLen;
     
}

};
