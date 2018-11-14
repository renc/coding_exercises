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

void recursiveFunc(int x)
{   
    x++;
    recursiveFunc(x);
}

int main()
{
    // number of bits in a byte, <climits> 
    // CHAR_BIT * sizeof(T) = T's occupied bits. 
    std::cout << CHAR_BIT << ", " << CHAR_MIN << ", " << CHAR_MAX << std::endl;

    recursiveFunc(0);

    std::cout << vector2string(getDigits(8973240)) << std::endl;
    std::cout << vector2string(getDigits(0)) << std::endl;
    assert( vector2string(getDigits(0)) == "0" );
    assert( vector2string(getDigits(10)) == "10" );
    assert( vector2string(getDigits(190)) == "190" );
    assert( vector2string(getDigits(195)) == "195" );
    assert( vector2string(getDigits(8973240)) == "8973240" );
 
    return 0; // exist status, 0 success. 
}
