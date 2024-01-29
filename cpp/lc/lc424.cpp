// https://leetcode.com/problems/longest-repeating-character-replacement/description/

#include <iostream>
#include <cstdint>
#include <string>
#include <set>
#include <map>
#include <vector>

using namespace std;

class Solution {
    
public:
    struct CharNum { char c; uint32_t n=0; };
    struct CharNumComp {
        bool operator() (const CharNum& a, const CharNum& b) const 
        {
            if (a.n == b.n ) return a.c > b.c;
            return a.n > b.n;
        }
        // from cppref: 
        //Everywhere the standard library uses the Compare requirements, uniqueness is determined by using the equivalence relation. 
        // In imprecise terms, two objects a and b are considered equivalent if neither compares less than the other: !comp(a, b) && !comp(b, a).
        // say CharNum(A,1), CharNum(B,1)
    };
    void printSet(auto& s)
    {
        for (auto e: s)
        {
            std::cout << "(" << e.c <<"," << e.n << ")";
        } std::cout << ";\n";
    }
    int characterReplacement(string s, int k) {
        // https://www.youtube.com/watch?v=gqXU1UyA8pk 
        // sliding window[left, right], window len=right-left+1, result is the max window len;
        // if window len - max(show up char) = how many other chars   <= k, move forward right pointer;
        // other move forward left pointer

        uint32_t left = 0, right = 0;
        uint32_t maxLen = 0; 
        std::map<char, uint32_t> c2n; // char to count
        c2n[s[left]] = 1;  
        std::set<CharNum, CharNumComp> sortedChars;
        sortedChars.insert( CharNum{s[left], 1} );

        for (; right < s.size();)
        {
            uint32_t winLen = right - left +1;            
            uint32_t maxCountOfSortedChar = c2n[sortedChars.begin()->c];
            std::cout <<"[" << left << "," << right << "],wl=" << winLen << ",mc=" << sortedChars.begin()->c <<" " << maxCountOfSortedChar << "; ";
            if (winLen - maxCountOfSortedChar <= k)
            {
                maxLen = std::max(maxLen, winLen);
                ++ right; // the sliding window extends
                if (right >= s.size())
                    break;
                if (c2n[s[right]]>0) { // char inside the window
                    CharNum newCN; newCN.c = s[right]; newCN.n = c2n[s[right]];
                    sortedChars.erase(newCN);
                    ++newCN.n;
                    c2n[s[right]] = c2n[s[right]] + 1;
                    sortedChars.insert(newCN);
                } else { // char first time added into the window
                    c2n[s[right]] = 1;
                    CharNum newCN; newCN.c = s[right]; newCN.n = c2n[s[right]];
                    sortedChars.insert(newCN);
                }
            } else
            {
                // sliding windows shrink from left; 
                CharNum oldCN; oldCN.c = s[left]; oldCN.n = c2n[s[left]];
                sortedChars.erase(oldCN);

                c2n[s[left]] = c2n[s[left]] - 1;
                --oldCN.n; 
                if (oldCN.n > 0)
                    sortedChars.insert(oldCN);
                ++left; 
            }
            printSet(sortedChars);
        }
        return maxLen;
    }
};
int main(int argc, char* argv[])
{
    std::vector<char> c2n;
    c2n.resize(256);
    c2n[(int)'A'] = 1;
    Solution solution;
    solution.characterReplacement("BAAA", 0);
    return 0;
}