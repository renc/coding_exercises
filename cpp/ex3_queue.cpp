#include <stdlib.h> // for NULL
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <map>
#include <random>
#include <cmath>
#include <vector>>
#include <queue>
#include <stack>
#include <algorithm>

std::string removeWhitespace(const std::string &s)
{
	std::string ret;
	for (int i = 0; i < s.length(); ++i)
	{
		if (std::isspace(s[i]) != 0)
		{
			// whitespace
		}
		else
			ret += s[i];

	}
	return ret;
}
class Solution {

public:

	static std::string answer(std::string sInput) 
	{
		std::string sWOWS = removeWhitespace(sInput);
		std::cout << "Input:\n" << sInput << std::endl;
		std::cout << "Remove space:\n" << sWOWS << std::endl;

		std::string ret;

		return ret;
	}
};

int main(void)
{
	std::cout << Solution::answer("a b  c def g  hi j") << std::endl; // expected: 
	std::cout << Solution::answer("  a b  c def g  hi j  ") << std::endl; // expected: 
	std::cout << Solution::answer("a b\tcd\ne") << std::endl;
	return 1;
}