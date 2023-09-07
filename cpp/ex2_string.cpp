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

     const char* cp = "hello";
    
    const char ca[] = "hello";
    std::cout << "strlen(ca)=" << std::strlen(ca) << ", "<< std::char_traits<char>::length(ca) << std::endl;
    char ca2[10]={'0'};
    memset(ca2, 'a', 9); std::cout << "ca2=" << ca2 << std::endl;
    {
        char *cptr = new char [10];
        std::memset(cptr, 'A', 10); std::cout << "cptr=" << cptr << std::endl;
        delete[] cptr;
    }
    {
        auto cpt2 = std::make_unique<char[]>(10);
        std::memset(cpt2.get(), 'B', 10);std::cout << "cpt2=" << cpt2 << std::endl;
    }

    return 0;
}