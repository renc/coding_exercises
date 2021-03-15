#include <stdlib.h> // for NULL
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <set>
#include <map>
#include <random>
#include <cmath>
#include <vector>>
#include <queue>
#include <stack>
#include <algorithm>

// To test the basic idea of std container 

struct Element
{
	int iVal = 0; 
	std::string sVal = "";
};  
struct ElementComp
{
	bool operator() (const Element& a, const Element& b) const {
		if (a.sVal < b.sVal) return true;
		return false;
	}
};


int main(void)
{
	{
		std::set<Element, ElementComp> s;
		s.insert(Element{ 9, "a" });
		s.insert(Element{ 7, "c" });
		s.insert(Element{ 4, "f" });
		s.insert(Element{ 6, "d" });
		s.insert(Element{ 5, "e" });
		s.insert(Element{ 8, "b" });
		for (auto& e : s) {
			std::cout << e.iVal << ", " << e.sVal << std::endl;
		}
	} 
	{
		auto elementComp = [](const Element& a, const Element& b) {
			return a.sVal < b.sVal;
		};
		std::set<Element, decltype(elementComp) > s(elementComp);
		s.insert(Element{ 9, "a" });
		s.insert(Element{ 7, "c" });
		s.insert(Element{ 4, "f" });
		s.insert(Element{ 6, "d" });
		s.insert(Element{ 5, "e" });
		s.insert(Element{ 8, "b" });
		for (auto& e : s) {
			std::cout << e.iVal << ", " << e.sVal << std::endl;
		}
	}
	return 1;
}