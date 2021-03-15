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
 
int main(void)
{
	std::cout << Solution::answer("a b  c def g  hi j") << std::endl; // expected: 
	std::cout << Solution::answer("  a b  c def g  hi j  ") << std::endl; // expected: 
	std::cout << Solution::answer("a b\tcd\ne") << std::endl;
	return 1;
}