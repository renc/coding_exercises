#include <vector>
#include <algorithm> // std::reverse
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

int solution(vector<int> &A) {
	// write your code in C++14 (g++ 6.2.0)
	if (std::is_sorted(A.begin(), A.end()))
		return A.size();

	int iCount = 0;
	for (int i = 0; i <= A.size() - 2; ++i)
	{
		if (A[i] <= A[i + 1]) {
			// in order
		}
		else {
			// A[i] > A[i+1]
			// 1. try to remove A[i]
			bool bRemoveLeftSideOk = false;
			if (i == 0) {
				// ok, pass 
				bRemoveLeftSideOk = true;

				if (std::is_sorted(A.begin() + 1, A.end()) == false)
					return 0;
			}
			else {
				// i > 0
				if (A[i - 1] <= A[i + 1]) {
					// ok, pass 
					bRemoveLeftSideOk = true;
				}
				else {
					bRemoveLeftSideOk = false;
				}
			}
			// 2. try to remove A[i+1]
			bool bRemoveRightSideOk = false;
			if (i == A.size() - 2) {
				// A[i+1] is the last item, ok to remove
				bRemoveRightSideOk = true;
				 
				if (std::is_sorted(A.begin(), A.end()-1) == false)
					return 0;
			}
			else if (i+2 < A.size()) {
				if (A[i] <= A[i + 2]) {
					bRemoveRightSideOk = true;
				}
				else {
					bRemoveRightSideOk = false;
				}
			}

			if (!bRemoveLeftSideOk && !bRemoveRightSideOk)
				return 0;
			if (bRemoveLeftSideOk)
				++iCount;
			if (bRemoveRightSideOk)
				++iCount;
		}
	}
	return iCount;
}

int main()
{
	std::vector<int> a1 = { 1,2,3,4,5,6,7 };
	int s1 = solution(a1);
	std::vector<int> a2 = { 1,2,2,2 };
	int s2 = solution(a2);
	std::vector<int> a3{};
	int s3 = solution(a3);
	std::vector<int> a4{ 4,5,2,3,4 };
	int s4 = solution(a4); // 0
	std::vector<int> a5{ 3,4,5,4 };
	int s5 = solution(a5); // 2, 
	std::vector<int> a50{ 3,4,5,4, 4 };
	int s50 = solution(a50); // 1, 
	std::vector<int> a51{ 3,4,5,4, 5 };
	int s51 = solution(a51); // 2, 
	std::vector<int> a6{ 4,3 };
	int s6 = solution(a6); // 2
	std::vector<int> a7{ 4,3,2, 1 };
	int s7 = solution(a7); // 0
	std::vector<int> a8{ 4,3,2 };
	int s8 = solution(a8); // 0,  error
	std::vector<int> a9{ 4,3,2,3 };
	int s9 = solution(a9); // 0

	return 0;
}