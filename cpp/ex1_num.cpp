/*
To build: 
g++ ex1_num.cpp -o ./build/ex1_num -std=c++11 -g 
// -g is need for debug
*/ 
#include <climits> // for CHAR_BIT 
#include <vector>
#include <string> 
#include <algorithm>
#include <cassert>  // for assert
#include <iostream>

bool isDigit(char c) {
    return c <= '9' && c >= '0';
}

// https://leetcode.com/problems/string-to-integer-atoi/ 
int myAtoi(std::string str) {
    int iFirstNonSpace = -1;
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] != ' ') {
            iFirstNonSpace = i; break;
        }
    }
    if (iFirstNonSpace < 0) return 0;
    int bNegative = 1;
    if (str[iFirstNonSpace] == '-') {
        bNegative = -1;
        ++iFirstNonSpace;
    }
    else if (str[iFirstNonSpace] == '+') {
        bNegative = 1;
        ++iFirstNonSpace;
    }
    long long val = 0;
    for (int i = iFirstNonSpace; i < str.length(); ++i) {
        if (str[i] <= '9' && str[i] >= '0') {
            val = val * 10 + str[i] - '0';

            if (val * bNegative < INT_MIN) return INT_MIN;
            else if (val * bNegative > INT_MAX) return INT_MAX;
        }
        else {
            break;
        }
    }
    val = val * bNegative;
    if (val < INT_MIN) return INT_MIN;
    else if (val > INT_MAX) return INT_MAX;
    return val;
}

std::string vector2string(const std::vector<int> &a)
{
    std::string str;
    for (auto e : a)
        str += std::to_string(e);
    return str; // for debug 
}

std::vector<int> getDigits(int num, bool bOrder=true)
{
    std::vector<int> ret;
    if (num == 0)
        ret.push_back(0);
    while (num > 0) {
        ret.push_back(num % 10);
        num /= 10;
    }
    // the above result is in reverse order 
    if (bOrder)
        std::reverse(std::begin(ret), std::end(ret)); // to rever it back 
    return ret;
} 
 

int main()
{
    // number of bits in a byte, <climits> 
    // CHAR_BIT * sizeof(T) = T's occupied bits. 
    std::cout << CHAR_BIT << ", " << CHAR_MIN << ", " << CHAR_MAX << std::endl;
     
    std::cout << vector2string(getDigits(8973240)) << std::endl;
    std::cout << vector2string(getDigits(0)) << std::endl;
    assert( vector2string(getDigits(0)) == "0" );
    assert( vector2string(getDigits(10)) == "10" );
    assert( vector2string(getDigits(190)) == "190" );
    assert( vector2string(getDigits(195)) == "195" );
    assert( vector2string(getDigits(8973240)) == "8973240" );
 
    return 0; // exist status, 0 success. 
}
