#include <cstdlib> 
#include <cstring> 
#include <string> 
#include <iostream>
#include <sstream> // for std::istringstream 
#include <assert.h>

char *strcpy1(char *to, const char *from) 
{
	if (from == NULL) return NULL;
	if (to == from) return to;
    std::cout <<  strlen(to) << ", " << strlen(from) << std::endl;
    char *head = to; // how about not enough memory for to ?
    int i = 0;
	for (; from[i] != '\0'; ++i) // how about not nul terminated?
		to[i] = from[i];
    to[i] = '\0'; // forget ths ?
    return head;
}

// function to remove all spaces.
bool myIsSpace(char ch)
{
    return std::isspace(static_cast<unsigned char>(ch));
}
std::string removeSpace(const std::string &str)
{
    std::string ret{""};
    for (auto e: str) {
        if (!myIsSpace(e)) {
            ret += e;
        } 
    }
    return ret;
}
// function to count the number of words
void getWordsFromString(const std::string &str)
{
    std::istringstream input;
    input.str(str);
    int iCount = 0; 
    while (input)
    {
        std::string sWord;
        input >> sWord;
        if (sWord.empty() == false)
            std::cout << iCount++ << ": " << sWord << std::endl;
        // do not know why the last word is empty
    }
}

int main()
{
    {
        char to[10], from[]=" hello _";
        std::cout << strcpy1(to, from) <<std::endl;
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
    assert(removeSpace(" 1 2 3    \t 4 \n 5 ") == std::string("12345"));   

    getWordsFromString(" how    are \t you ? ");
    return 0;
}