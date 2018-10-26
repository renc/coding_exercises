/*
To build: 
g++ ex1_num.cpp -o ex1_num -std=c++11 
*/ 
#include <vector>
#include <string> 
#include <algorithm>
#include <cassert>  // for assert
#include <iostream>

std::string vector2string(const std::vector<int> &a)
{
    std::string str;
    for (auto e : a)
        str += std::to_string(e);
    return str;
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
    std::cout << vector2string(getDigits(8973240)) << std::endl;
    assert( vector2string(getDigits(0)) == "0" );
    assert( vector2string(getDigits(10)) == "10" );
    assert( vector2string(getDigits(190)) == "190" );
    assert( vector2string(getDigits(195)) == "195" );
    assert( vector2string(getDigits(8973240)) == "8973240" );
    return 1;
}
