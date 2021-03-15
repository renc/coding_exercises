#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <algorithm>

// https://leetcode.com/problems/last-substring-in-lexicographical-order/
// 9 / 24 test cases passed.
// renc, most simple /naive way, easy to remember. O(n*n*logn)
// n*logn for sorting, during 1 sort it use O(n) for comparing two string, 
// so totally O(n*logn * n)
std::vector<int> buildSuffixArray(const std::string& s)
{
    std::map<std::string, int> mapSuffix2Idx;
    for (int i = 0; i < s.length(); ++i) {
        mapSuffix2Idx[s.substr(i)] = i;
    }
    std::vector<int> arr;
    for (std::map<std::string, int>::const_iterator it = mapSuffix2Idx.begin(); it != mapSuffix2Idx.end(); ++it) {
        std::cout << it->first << ", " << it->second << std::endl;
        arr.push_back(it->second);
    }
    return arr;
}

// 22 / 24 test cases passed. Status: Time Limit Exceeded
// O(n*logn*logn)
namespace gfg {

	struct Suffix {
		int suffixIndex; // to store the original index, will not change once set
		int tuple[2]; // this rank and next rank, the sort is done for each time.
	};

	bool compare(Suffix a, Suffix b) {
		if (a.tuple[0] < b.tuple[0]) return true;
		else if (a.tuple[0] == b.tuple[0] && a.tuple[1] < b.tuple[1]) return true;
		return false;
	}

	std::vector<int> buildSuffixArray(std::string s) {
        const size_t n = s.length();
		std::vector<Suffix> aSuf(n);
        // step k=1
		for (size_t i = 0; i < n; ++i) {
			aSuf[i].suffixIndex = i; 
			aSuf[i].tuple[0] = s[i] - 'A'; // many article use 'a' here, but i use 'A' so it does not just support lower cases.
			aSuf[i].tuple[1] = (i + 1 < n) ? s[i + 1] - 'A' : -1;
		}
		std::sort(aSuf.begin(), aSuf.end(), compare);
		
		std::vector<int> mapSurffixIdx2SortIndex(n);
		int sortIndex = 0; // ==0,every time after sort,  
		for (size_t i = 1; i < n; ++i) {
			if (aSuf[i].tuple[0] == aSuf[i - 1].tuple[0] &&
				aSuf[i].tuple[1] == aSuf[i - 1].tuple[1]) {}
			else {
				sortIndex = sortIndex + 1; // it is ok to increase, do not matter to increase 1 or 2 or 3
			}
			mapSurffixIdx2SortIndex[aSuf[i].suffixIndex] = sortIndex;
		}
        //   [0][1][2][3][4][5][6][7][8][9]...[n-1]  string
		//   k1    k2    k3          k4
		//   cnt2  cnt4  cnt8        cnt16 
		//const size_t iTotolSteps = 
		for (size_t k = 2; ; ++k) {
			size_t count = (size_t)pow(2, k);
			size_t offset = count / 2; // element index offset from last step(step k-1), 
			assert(offset == (size_t)pow(2, k - 1));
			// update the 2-tuple
			for (size_t i = 0; i < n; ++i) {
				aSuf[i].tuple[0] = mapSurffixIdx2SortIndex[ aSuf[i].suffixIndex ];

				const size_t nextSuffixIndex = aSuf[i].suffixIndex + offset;
				aSuf[i].tuple[1] = nextSuffixIndex < n ? mapSurffixIdx2SortIndex[nextSuffixIndex] : -1;
			} 

			std::sort(aSuf.begin(), aSuf.end(), compare);

			int sortIndex = 0; // ==0,every time after sort,  
			for (size_t i = 1; i < n; ++i) {
				if (aSuf[i].tuple[0] == aSuf[i - 1].tuple[0] &&
					aSuf[i].tuple[1] == aSuf[i - 1].tuple[1]) {
				}
				else {
					sortIndex = sortIndex + 1; // it is ok to increase, do not matter to increase 1 or 2 or 3
				}
				mapSurffixIdx2SortIndex[aSuf[i].suffixIndex] = sortIndex;
			}
			if (sortIndex >= n - 1) break; // finished sorting
			if (count >= n) break;
		}

		std::vector<int> arr(n);
		for (size_t i = 0; i < n; ++i) {
			arr[i] = aSuf[i].suffixIndex;
		}
		return arr;
	}

	int buildSuffixArray_getLastItem(std::string s) {
		const size_t n = s.length();
		std::vector<Suffix> aSuf(n);
		// step k=1
		for (size_t i = 0; i < n; ++i) {
			aSuf[i].suffixIndex = i;
			aSuf[i].tuple[0] = s[i] - 'A';
			aSuf[i].tuple[1] = (i + 1 < n) ? s[i + 1] - 'A' : -1;
		}
		std::sort(aSuf.begin(), aSuf.end(), compare);

		std::vector<int> mapSurffixIdx2SortIndex(n);
		int sortIndex = 0; // ==0,every time after sort,  
		for (size_t i = 1; i < n; ++i) {
			if (aSuf[i].tuple[0] == aSuf[i - 1].tuple[0] &&
				aSuf[i].tuple[1] == aSuf[i - 1].tuple[1]) {
			}
			else {
				sortIndex = sortIndex + 1; // it is ok to increase, do not matter to increase 1 or 2 or 3
			}
			mapSurffixIdx2SortIndex[aSuf[i].suffixIndex] = sortIndex;
		}
		// discard all the other suffix, just leave the suffix with the last sortIdx 

		//   [0][1][2][3][4][5][6][7][8][9]...[n-1]  string
		//   k1    k2    k3          k4
		//   cnt2  cnt4  cnt8        cnt16 
		//const size_t iTotolSteps = 
		for (size_t k = 2; ; ++k) {
			size_t count = (size_t)pow(2, k);
			size_t offset = count / 2; // element index offset from last step(step k-1), 
			assert(offset == (size_t)pow(2, k - 1));
			// update the 2-tuple
			for (size_t i = 0; i < n; ++i) {
				aSuf[i].tuple[0] = mapSurffixIdx2SortIndex[aSuf[i].suffixIndex];

				const size_t nextSuffixIndex = aSuf[i].suffixIndex + offset;
				aSuf[i].tuple[1] = nextSuffixIndex < n ? mapSurffixIdx2SortIndex[nextSuffixIndex] : -1;
			}

			std::sort(aSuf.begin(), aSuf.end(), compare);

			int sortIndex = 0; // ==0,every time after sort,  
			int countLastSortIndex = 0;
			for (size_t i = 1; i < n; ++i) {
				if (aSuf[i].tuple[0] == aSuf[i - 1].tuple[0] &&
					aSuf[i].tuple[1] == aSuf[i - 1].tuple[1]) {
				}
				else {
					sortIndex = sortIndex + 1; // it is ok to increase, do not matter to increase 1 or 2 or 3
					countLastSortIndex = 0; // reset to zero,
				}
				mapSurffixIdx2SortIndex[aSuf[i].suffixIndex] = sortIndex;
				++countLastSortIndex;
			}
			if (sortIndex >= n - 1) break; // finished sorting
			if (count >= n) break;
			if (countLastSortIndex == 1) {
				break;
			}
		}
		return aSuf.back().suffixIndex;
	}
} // namespace gfg
int main()
{
	{
		std::string s = "banana";
		std::vector<int> arr = buildSuffixArray(s);
		std::vector<int> aExpectedResult{ 5, 3, 1, 0, 4, 2 };
		for (int i = 0; i < arr.size(); ++i) {
			assert(arr[i] == aExpectedResult[i]);
		}
	} 
	{
		std::string s = "mississippi";
		std::vector<int> arr = buildSuffixArray(s);
		std::vector<int> aExpectedResult{ 10,7,4,1,0,9,8,6,3,5,2 };
		for (int i = 0; i < arr.size(); ++i) {
			assert(arr[i] == aExpectedResult[i]);
		}
	}
	{
		std::string s = "abab";
		std::vector<int> arr = gfg::buildSuffixArray(s);
		std::vector<int> aExpectedResult{ 2, 0, 3, 1 };
		for (int i = 0; i < arr.size(); ++i) {
			assert(arr[i] == aExpectedResult[i]);
		}
	}
	{
		std::string s = "mississippi";
		std::vector<int> arr = gfg::buildSuffixArray(s);
		std::vector<int> aExpectedResult{ 10,7,4,1,0,9,8,6,3,5,2 };
		for (int i = 0; i < arr.size(); ++i) {
			assert(arr[i] == aExpectedResult[i]);
		}
	}
	return 0; 
}