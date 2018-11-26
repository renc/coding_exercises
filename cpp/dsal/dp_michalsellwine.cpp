// dynamic programming 
// g++ ./dsal/dp_michalsellwine.cpp  -o ./build/dp_michalsellwine -std=c++11 -g
 
#include <iostream>
#include <vector>
#include <cmath>

// https://www.hackerearth.com/zh/practice/algorithms/dynamic-programming/introduction-to-dynamic-programming-1/tutorial/
// Michal sell wine: 
int greedy(const std::vector<int> &aInitPrice, int iL, int iR, int iYear)
{
    if (iL >= iR)
    {
        return aInitPrice[iL] * iYear;
    }

    if (aInitPrice[iL] < aInitPrice[iR])
    {
        // to sell the small price first
        return aInitPrice[iL] * iYear + greedy(aInitPrice, iL+1, iR, iYear+1);
    }

    return aInitPrice[iR] * iYear + greedy(aInitPrice, iL, iR-1, iYear+1);
}
int backtrack(const std::vector<int> &aInitPrice, int iL, int iR, int iYear)
{
    if (iL >= iR)
    {
        return aInitPrice[iL] * iYear;
    }

    int iLeft = aInitPrice[iL] * iYear + backtrack(aInitPrice, iL+1, iR, iYear+1);
    int iRight = aInitPrice[iR] * iYear + backtrack(aInitPrice, iL, iR-1, iYear+1);

    if (iLeft > iRight)
    {
        // choose the one on the left size 
    } else {
        // choose the one on the right size
    }
    return std::max(iLeft, iRight);
}
int main()
{
    std::vector<int> aInitPrice{2,3,5,1,4};
    int iTotallyProfitGreedy = greedy(aInitPrice, 0, aInitPrice.size()-1, 1);
    std::cout << "Michal sell wine, greedy l or r: " << iTotallyProfitGreedy << std::endl;
    
    int iTotallyProfitBacktrack = backtrack(aInitPrice, 0, aInitPrice.size()-1, 1);
    std::cout << "Michal sell wine, backtrack l or r: " << iTotallyProfitBacktrack << std::endl;
    
    return 0;
}