#include <vector>
#include <map> 
#include <unordered_map> 
#include <iostream>
#include <cmath>


unsigned int lisRecursive(const std::vector<int> &a, int curIdx)
{
    int iLIS = 1;

    int iLISofRightSide = 0;
    for (size_t j = curIdx + 1; j < a.size(); ++j)
    {
        if (a[curIdx] > a[j]) {
            // ignore
        } else {
            // to calc lis[j]
            int iTempRightSize = lisRecursive(a, j);
            iLISofRightSide = std::max(iLISofRightSide, iTempRightSize);
        }
    }
    iLIS += iLISofRightSide;
    return iLIS;
}
unsigned int lisRecursive(const std::vector<int> &a)
{
    std::map<int, unsigned int> lis; // count the number of longest increasing sequence

    for (size_t i = 0; i < a.size(); ++i)
    {
        lis[ a[i] ] = lisRecursive(a, i);
    }

    unsigned int iMax = 0;
    for (auto e: lis)
    {
        iMax = std::max(e.second, iMax);
    }

    return iMax;
}


int main()
{
    std::vector<int> a{5,3,4,8,6,7};
    int s1 = lisRecursive(a);
    std::cout << s1 << std::endl;

    return 0;
}