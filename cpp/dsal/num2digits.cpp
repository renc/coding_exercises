#include <vector>
#include <algorithm> // std::reverse
#include <string>
#include <iostream>
#include <cassert>

std::vector<int> getDigits(int num, bool bOrder = true)
{
	std::vector<int> ret;
	if (num == 0)
		ret.push_back(0);
	while (num > 0) {
		ret.push_back(num % 10);
		num /= 10;
	}
	// the above result is in reverse order 
	if (bOrder)
		std::reverse(std::begin(ret), std::end(ret)); // to rever it back 
	return ret;
}
int valueFromDigits(const std::vector<int> &digits)
{
	int iRet = 0;
	for (size_t i = 0; i < digits.size(); ++i)
	{
		iRet = iRet * 10 + digits[i];
	}
	return iRet;
}

// Given number A, 

int solution(int A)
{
	std::vector<int> digits = getDigits(A);
	int iMax = INT32_MAX;
	iMax;

	int iSide = 0; // 0: left; 1: right
	int iL = 0, iR = digits.size() - 1;
	std::vector<int> aRetDigits;
	while (iL <= iR)
	{
		if (iSide == 0) {
			aRetDigits.push_back(digits[iL++]);
			iSide = 1;
		}
		else if (iSide == 1) {
			aRetDigits.push_back(digits[iR--]);
			iSide = 0;
		}
		else {
			assert(false);
		}
	}

	int iResultValue = valueFromDigits(aRetDigits);
	return iResultValue;
}

/*
int main()
{
	int s1 = solution(123456);
	assert(s1 == 162534);
	int s2 = solution(12345);
	assert(s2 == 15243);
	int s3 = solution(0);
	assert(s3 == 0);
	int s4 = solution(100);
	assert(s4 == 100);
	int s5 = solution(1000);
	assert(s5 == 1000);
	getchar();
	return 0;
}
*/