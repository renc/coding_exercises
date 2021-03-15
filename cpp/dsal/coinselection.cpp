#include <vector>
#include <map>
#include <iostream>
#include <cassert>
#include <algorithm>


namespace csunlimit {
	// case the coins can be used unlimit times
	// https://leetcode.com/problems/coin-change

    class Solution {
    public:
        int coinChange(std::vector<int>& coins, int amount) {
            if (amount == 0) return 0;
            if (amount < 0 || coins.size() == 0) return -1;
            return coinChange_dp(coins, amount);

            std::vector<int> cache(amount + 1, -1); // carefully
            for (auto e : coins) if (e <= amount) cache[e] = 1;// how many this save substeps in bt?
            return coinChange_bt_mem(coins, amount, cache);
        }

        // 15 / 182 test cases passed. Time Limit Exceeded
        int coinChange_bt(std::vector<int>& coins, int amount) {
            if (amount == 0) return 0; // very important 
            if (amount < 0) return -1; // very important 
            int iMinCount = INT_MAX;
            for (int i = 0; i < coins.size(); ++i) {
                int temp = coinChange_bt(coins, amount - coins[i]);
                if (temp == -1) {
                    // fail to find combination
                }
                else
                    iMinCount = std::min(iMinCount, 1 + temp);
            }
            if (iMinCount == INT_MAX) return -1;
            return iMinCount;
        }
        // 31 / 182 test cases passed. Time Limit Exceeded
        // do not require the coins array is sorted. 
        int coinChange_bt_mem(std::vector<int>& coins, int amount, std::vector<int>& cache) {
            // backtracking + memorization to speed up 
            if (amount == 0) return 0; // very important 
            if (amount < 0) return -1; // very important
            //std::cout << "amount=" << amount << ", ";
            if (cache[amount] != -1) return cache[amount];

            int iMinCount = INT_MAX;
            for (int i = 0; i < coins.size(); ++i) {
                int temp = coinChange_bt_mem(coins, amount - coins[i], cache);
                if (temp == -1) {
                    // fail to find combination
                }
                else
                    iMinCount = std::min(iMinCount, 1 + temp);
            }
            if (iMinCount == INT_MAX) return -1;
            //std::cout << "amount=" << amount << ", minCount=" << iMinCount << std::endl;
            cache[amount] = iMinCount;
            return iMinCount;
        }
        //                                         
        int coinChange_dp(std::vector<int>& coins, int amount) {
            if (amount == 0) return 0;
            if (amount < 0) return -1;
            std::sort(coins.begin(), coins.end());
            // make sure from small to large coins, it is matter? 
            // yes, example [2,5,10,1], tgt=27 
            std::vector<int> dp(amount + 1, INT_MAX);
            dp[0] = 0; // no coins need for tgt amount=0.
            //for (int i = 0; i < coins.size(); ++i) {
            //    dp[coins[i]] = 1; //  crash when coins[i] > amount 
            //} // also the smallest coin [0], the start 
            for (int i = 1; i <= amount; ++i) {
                // we are going to find out how many coins need for this amount i
                for (int j = 0; j < coins.size(); ++j) {
                    if (coins[j] < i) { // if this coint is < the target money 
                        if (dp[i - coins[j]] != INT_MAX)
                            if (dp[i] > 1 + dp[i - coins[j]]) // wo this will be wrong answer.
                                dp[i] = 1 + dp[i - coins[j]]; // this requires the dp from botton up
                    }
                    else if (coins[j] == i) {
                        dp[i] = 1; // one coin is good enougth 
                    }
                    else if (coins[j] > i) {}
                }
                //std::cout << "i=" << i << ", dp[i]=" << dp[i] << "; ";
            }
            if (dp[amount] == INT_MAX) return -1;

            return dp[amount];
        }
    };
} // namespace csunlimit 

int main()
{
    {
        std::vector<int> coins{ 186, 419, 83, 408 }; int amount = 6249;
        csunlimit::Solution so;
        int iMinCount = so.coinChange(coins, amount);
        std::vector<int> cache(amount + 1, -1);
        int iMinCount2 = so.coinChange_bt_mem(coins, amount, cache); // you cannot image how show it is.
        std::cout << "minCount=" << iMinCount << std::endl;
    }
    return 0; 
}