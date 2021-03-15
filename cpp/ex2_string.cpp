#include <cstdlib> 
#include <cstring> 
#include <string> 
#include <iostream>
#include <sstream> // for std::istringstream 
#include <assert.h>
#include <string>

#include "util/string.h"
void readPosition(std::string s, int& r, int& c)
{
    int i = 0; 
    for (; i < s.size(); ++i) {
        if ('A' <= s[i] && s[i] <= 'Z') {
            c = s[i] - 'A' + 1; // 'A' = 1, 'Z' = 26
            r = std::stoi(s.substr(0, i));
        }
    }
}
int main()
{
    {
        std::string cell = "13D";  int r = -1, c = -1;
        readPosition(cell, r, c);
        std::cout << cell << ", r=" << r << ", c=" << c << std::endl;
    }
    {
        std::string cell = "3Z";  int r = -1, c = -1;
        readPosition(cell, r, c);
        std::cout << cell << ", r=" << r << ", c=" << c << std::endl;
    }

    {
        char to[10], from[]=" hello _";
        std::cout << renc::strcpy1(to, from) <<std::endl;
    }
    const char arrC[] = " 123 ";
    char *pS = const_cast<char *>(arrC);
    std::cout << "sizeof:" << sizeof(" 123 ") << ", " << sizeof(arrC) << ", " << sizeof(pS) << std::endl;
 
 /* since C++14
    using namespace std::string_literals;
 
    std::string s1 = "abc\0\0def";
    std::string s2 = "abc\0\0def"s;
    std::cout << "s1: " << s1.size() << " \"" << s1 << "\"\n";
    std::cout << "s2: " << s2.size() << " \"" << s2 << "\"\n";
  */
    return 0;
}