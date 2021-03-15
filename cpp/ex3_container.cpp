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
#include <assert.h>
// To test the basic idea of std container 

namespace comp {
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
	void test()
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
	}
} // end of namespace comp 
namespace matchparenthese {
	bool isValid(std::string s)
	{
		int iCountLeftParentthese = 0;
		int iCountLeftSquare = 0;
		char cLast = ' ';
		for (auto e : s) {
			if (e == '(') {
				++ iCountLeftParentthese;
				cLast = e;
			}
			else if (e == '[') {
				++iCountLeftSquare;
				cLast = e;
			} 
			else if (e == ')') {
				if (cLast != '(') return false;
				--iCountLeftParentthese;
				//but how to update this cLast, that is why we need stack. 
			}
		}
	}
	void unittest()
	{
		assert(isValid(""));
		assert(isValid("([])"));
		assert(isValid("([])[") == false);
		assert(isValid("([)") == false);
		assert(isValid("[])") == false);
		assert(isValid("([[][]])"));
	}
}
namespace unique {
	template< typename Iter >
	Iter unique(Iter first, Iter last) {
		if (first == last)
			return last;
		Iter result = first;
		while (++first != last) {
			if (!(*result == *first) && ++result != first) {
				*result = std::move(*first);
			}
		}
		return ++result;
	}
	void test() {
		std::vector<int> a{ 1,2,3,4,4,4,5,6 };
		auto result = unique::unique(a.begin(), a.end());
		int i = 0;
		++i;
	}
}
int main(void)
{
	//matchparenthese::unittest();
	unique::test();
	return 1;
}