#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <algorithm> // std::max, std::min
#include <memory> // std::unique_ptr
namespace util {
	inline std::string toString(const std::vector<int>& a)
	{
		std::string s = "[";
		bool bFirst = true;
		for (auto e : a) {
			if (bFirst) { bFirst = false; } else s += ", "; 
			s += std::to_string(e);
		}
		s += "]";
		return s;
	}
	inline std::string toString(const std::vector<std::vector<int> >& a)
	{
		std::string s = "{";
		for (auto e : a) {
			s += toString(e) + ", ";
		}
		s += "}";
		return s;
	}
	inline bool isSame(const std::vector<int>& a, const std::vector<int>& b) {
		if (a.size() != b.size()) return false;
		for (int i = 0; i < a.size() && b.size(); ++i) if (a[i] != b[i]) return false;
		return true;
	}
	inline int min(const std::vector<int>& a) {
		std::vector<int>::const_iterator it = std::min_element(a.begin(), a.end());
		return *it;
	}
	inline void printStrings(const std::vector<std::string>& a) {
		for (auto e : a) {
			std::cout << e << "\n";
		}
	}
}