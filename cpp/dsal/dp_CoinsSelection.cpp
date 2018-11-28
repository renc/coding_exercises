// dynamic programming 
// given some coins, for example {1, 3, 5}
// given a sum, for example sum =11, 
// to calc the minimun number of coins used.
// Check my local notebook for details. 
// $ g++ ./dsal/dp_CoinsSelection.cpp  -o ./build/dp_CoinsSelection -std=c++11 -g

#include <iostream>
#include <vector>
#include <map>
#include <cmath>

std::string vector2string(const std::vector<int> &a)
{
    std::string str;
    for (auto e: a)
        str += std::to_string(e) + ", ";
    return str;
}
// https://www.topcoder.com/community/competitive-programming/tutorials/dynamic-programming-from-novice-to-advanced/

// this version will enumerate all posible selections
// if no possible combination, no output.
// Greed ? everytime to select the biggest coin to used, that is greedy.
// the input parameter: std::vector<int> coinsUsed is used to print result, not returned, so no reference.
void greed(const std::vector<int> &coins, int iSum, std::vector<int> coinsUsed)
{
    if (iSum <= 0)
    {
        std::cout << "Coin selection, greed: "; 
        for (auto e: coinsUsed)
            std::cout << e << ", ";
        std::cout << std::endl; 
        return; 
    }  
    
    for (int i = coins.size() - 1; i >= 0; --i)
    {
        if (iSum >= coins[i])
        {
            std::vector<int> coinsUsedUpdated = coinsUsed;
            coinsUsedUpdated.push_back(coins[i]);

            greed(coins, iSum - coins[i], coinsUsedUpdated); 
        }
    }
}
// different from the above version, this version try to find the combination
// to get the minimum number of coins used.
bool greedFindOptimal(const std::vector<int> &coins, int iSum, std::vector<int> coinsUsed)
{
    if (iSum <= 0)
    {
        std::cout << "Coin selection, greed: "; 
        for (auto e: coinsUsed)
            std::cout << e << ", ";
        std::cout << std::endl; 
        return true; 
    }  
    
    bool bFindAllready = false;
    for (int i = coins.size() - 1; i >= 0; --i)
    {
        if (iSum >= coins[i])
        {
            std::vector<int> coinsUsedUpdated = coinsUsed;
            coinsUsedUpdated.push_back(coins[i]);

            bFindAllready = greedFindOptimal(coins, iSum - coins[i], coinsUsedUpdated); 
            if (bFindAllready)
                break;
        }
    }
    return bFindAllready;
}
// this is the most basic one, only to find the minumum number of coins combination.
// 
unsigned int dp_mostbasic(const std::vector<int> &coins, int iSum)
{
    if (iSum == 0) { 
        return 0;
    } else if (iSum < 0) {
        return INT32_MAX;
        // Note, tricky, if the return type is int,
        // later 1 + INT32_MAX become (int)-2147483648
    }

    unsigned int iMinimumNum = INT32_MAX;
    
    for (int i = 0; i < coins.size(); ++i) { 

        unsigned int iCoinNum = 1 + dp_mostbasic(coins, iSum - coins[i]); 
        if (iMinimumNum > iCoinNum)
        {
            iMinimumNum = iCoinNum; 
        }
    } 
    return iMinimumNum;
}
unsigned int dp(const std::vector<int> &coins, int iSum, std::vector<int> &coinsUsed)
{
    if (iSum == 0) { 
        return 0;
    } else if (iSum < 0) {
        return INT32_MAX;
        // Note, tricky, if the return type is int,
        // later 1 + INT32_MAX become (int)-2147483648
    }

    unsigned int iMinimumNum = INT32_MAX;
    int iSelectedCoin = -1;
    std::map<int, std::vector<int> > mapCoinToCacheUsed; 
    for (auto e: coins)
        mapCoinToCacheUsed[e] = coinsUsed;

    for (int i = 0; i < coins.size(); ++i) {
        mapCoinToCacheUsed[coins[i]].push_back( coins[i] );

        unsigned int iCoinNum = 1 + dp(coins, iSum - coins[i], mapCoinToCacheUsed[coins[i]]); 
        if (iMinimumNum > iCoinNum)
        {
            iMinimumNum = iCoinNum;
            iSelectedCoin = coins[i];
        }
    }
    
    if (iSelectedCoin >= 0)
    {     
        coinsUsed = mapCoinToCacheUsed[ iSelectedCoin ];
    }
    return iMinimumNum;
}
 
unsigned int dp_cacheSubProblem(const std::vector<int> &coins, int iSum, std::map<int, int> &mapCacheDp)
{
    if (iSum == 0) { 
        return 0;
    } else if (iSum < 0) {
        return INT32_MAX;
        // Note, tricky, if the return type is int,
        // later 1 + INT32_MAX become (int)-2147483648
    }

    if (mapCacheDp.find(iSum) != mapCacheDp.end())
    {
        // sub problem already calc before.
        return mapCacheDp[iSum];
    }

    unsigned int iMinimumNum = INT32_MAX;
    
    for (int i = 0; i < coins.size(); ++i) { 

        unsigned int iCoinNum = 1 + dp_cacheSubProblem(coins, iSum - coins[i], mapCacheDp); 
        if (iMinimumNum > iCoinNum)
        {
            iMinimumNum = iCoinNum; 
        }
    }

    mapCacheDp[iSum] = iMinimumNum; // cache dp(n) value; 
    return iMinimumNum;
}

unsigned int dp_bottomup(const std::vector<int> &coins, int iSum)
{ 
    
    return 0;
} 

unsigned int dp_bottomup_v2(const std::vector<int> &coins, int iSum)
{
    // to use the bottom up approach to calc all the subproblems.
    
    std::map<int, int> dp;
    dp[0] = 0; // introduce error dp[1] = min( 1 + dp[1-1], ... )
    for (size_t i = 0; i < coins.size(); ++i)
    {
        dp[ coins[i] ] = 1; // to get dp[] only use 1 coin
    }

    for (int i = 0; i <= iSum; ++i)
    {
        // to calc dp[i]
        // = min { 1+dp[i - coins[0]],  1+dp[i - coins[1]],  1+dp[i - coins[2]], ... }
        for (size_t j = 0; j < coins.size(); ++j)
        {
            int temp = i - coins[j];
            if ( temp >= 0) {
                if (dp.find(i) != dp.end())
                    dp[i] = std::min(dp[i], 1 + dp[temp]);
                else
                    dp[i] = 1+dp[temp];
            }
        }
    }
    
    if (dp.find(iSum) == dp.end())
        return 0;

    return dp[iSum];
}

int main()
{
    std::vector<int> coins{1, 2, 5}; // can be used replicition
    int iSum = 11; 
    std::vector<int> coinsUsed {};
    greed(coins, iSum, coinsUsed); 

    std::cout << "To test the greed to find optimal: \n"; 
    coinsUsed = {};
    bool bRet = greedFindOptimal(coins, 11, coinsUsed);
    std::cout << "Find combination? : " << bRet << std::endl;
    coins = {2, 5};
    iSum = 3; 
    bRet = greedFindOptimal(coins, iSum, coinsUsed); // find no combination
    std::cout << "Find combination? : " << bRet << std::endl;

    std::cout << "To test the dp to find optimal: \n"; 
    coinsUsed = {};
    coins = {1, 3, 5};
    iSum = 11; 
    int iCoinNum = dp(coins, iSum, coinsUsed);
    std::cout << "For iSum:" << iSum << ", Coin num: " << iCoinNum;  std::cout  << ", used: " << vector2string(coinsUsed) << std::endl; 
    
    coinsUsed = {};
    coins = {1, 3, 5};
    iSum = 4; 
    iCoinNum = dp(coins, iSum, coinsUsed);
    std::cout << "For iSum:" << iSum << ", Coin num: " << iCoinNum;  std::cout  << ", used: " << vector2string(coinsUsed) << std::endl; 
    
    coins = {1, 3};
    coinsUsed = {};
    iSum = 2; 
    iCoinNum = dp(coins, iSum, coinsUsed);
    std::cout << "For iSum:" << iSum << ", Coin num: " << iCoinNum ;  std::cout  << ", used: " << vector2string(coinsUsed) << std::endl; 
    
    coins = {3, 5};
    coinsUsed = {};
    iSum = 2; 
    iCoinNum = dp(coins, iSum, coinsUsed);
    std::cout << "For iSum:" << iSum << ", Coin num: " << iCoinNum ;  std::cout << ", used: " << vector2string(coinsUsed) << std::endl; 
    


    std::cout << "To test the dp(+memorization) to find optimal: \n"; 
    coins = {1, 3, 5};
    iSum = 11; 
    std::map<int, int> mapCacheDp = {};
    iCoinNum = dp_cacheSubProblem(coins, iSum, mapCacheDp);
    std::cout << "For iSum:" << iSum << ", Coin num: " << iCoinNum << std::endl;
    
    coins = {1, 3, 5};
    iSum = 4; 
    mapCacheDp = {};
    iCoinNum = dp_cacheSubProblem(coins, iSum, mapCacheDp);
    std::cout << "For iSum:" << iSum << ", Coin num: " << iCoinNum << std::endl;
    
    coins = {3, 5};
    iSum = 2; 
    mapCacheDp = {};
    iCoinNum = dp_cacheSubProblem(coins, iSum, mapCacheDp);
    std::cout << "For iSum:" << iSum << ", Coin num: " << iCoinNum << std::endl;
    
    std::cout << "To test the dp(+bottom up) to find optimal: \n"; 
    coins = {1, 3, 5};
    iSum = 11;  
    iCoinNum = dp_bottomup_v2(coins, iSum);
    std::cout << "For iSum:" << iSum << ", Coin num: " << iCoinNum << std::endl;
    
    getchar();
    return 0;
}