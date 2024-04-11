/*
Longest Substring with K Distinct Characters (medium)

Problem Statement #
Given a string, find the length of the longest substring in it with no more than K distinct characters.

Example 1:
Input: String="araaci", K=2
Output: 4
Explanation: The longest substring with no more than '2' distinct characters is "araa".
renc: 
brute force first , 
araa,  still good, [0,3] range, len=4, 
araac, there are a/r/c three char > K=2, so left0----right4  this [0,4] range stop hre, no meaning to try [0,5]
 raac , still not good, so [1,4] stop here, no need to try [1,5]
  aac , good now [2,4] works, can try move right to [2,5] 
  aaci, not good > K, so left->
   aci, not good > k, so left-> 
    ci,  good now, check the len (which is 2)


Example 2:

Input: String="araaci", K=1
Output: 2
Explanation: The longest substring with no more than '1' distinct characters is "aa".
Example 3:

Input: String="cbbebi", K=3
Output: 5
Explanation: The longest substrings with no more than '3' distinct characters are "cbbeb" & "bbebi".

// --
sliding window

*/

#include <iostream>
#include <string>
#include <unordered_map>

class LongestSubstringKDistinct {
public:
static int findLength(const std::string &str, int k)
{
    int maxLength = 0;
    int windowStart = 0; 
    std::unordered_map<char, int> mapChar2Count; 
    
    for (int windowEnd = 0; windowEnd < str.length(); ++windowEnd)
    {
        // loop to extend the window, until break > k 
        ++mapChar2Count[ str[windowEnd] ];

        while (mapChar2Count.size() > k)
        {
            // stop extending the right side, 
            // move the left side 
            char leftChar = str[windowStart];
            if (--mapChar2Count[leftChar] == 0) {
                mapChar2Count.erase(leftChar);
            }
            ++windowStart; // this move the left side 
        }

        maxLength = std::max(maxLength, windowEnd - windowStart + 1);
    }
    return maxLength;
}
};