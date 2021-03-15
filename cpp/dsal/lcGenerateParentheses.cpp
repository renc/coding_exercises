#include <iostream>
#include <string>
#include <vector>
#include "common.hpp"

using namespace std;

// https://leetcode.com/problems/generate-parentheses/submissions/
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        std::vector<std::string> result;
        if (n >= 1) {
            std::string s;
            int iCount = 0; // = the num of (  minus the num of ). 

            generateParenthesis_bt(s, iCount, n, result);
        }
        return result;
    }
    void generateParenthesis_bt(std::string s, int iCount, int n, std::vector<std::string>& result) {
        if (iCount == 0 && s.length() == n * 2) {
            std::cout << "s=" << s << "; ";
            result.push_back(s);
            return;
        }
        if (s.length() > n * 2) return;
        if (iCount >= 1) {
            // can add (  or )
            if (iCount < n)
                generateParenthesis_bt(s + '(', iCount + 1, n, result);
            generateParenthesis_bt(s + ')', iCount - 1, n, result);
        }
        else {
            // only add ( 
            generateParenthesis_bt(s + '(', iCount + 1, n, result);
        }
    }
};

int main()
{
    Solution s;
    std::vector<std::string> r = s.generateParenthesis(3);
    util::printStrings(r);
}