#include <iostream> 
#include <vector> 
#include <memory>
#include <algorithm>
#include <assert.h>

using namespace std;
namespace bs {
    namespace cooldown {
        class Solution {
        public:
            int maxProfit(vector<int>& prices) {
                for (const auto& e : prices) {
                    std::cout << e << ", ";
                } std::cout << std::endl;

                // brute force 
                int iBuyIndex = -1;  // not holding position 
                std::vector<int> aProfit(prices.size(), 0);
                for (int i = 0; i < prices.size(); ++i) {
                    int iProfix = 0;
                    iBuyIndex = i; std::cout << "b" << iBuyIndex << ", ";
                    bool bTriggerCooldown = false;

                    for (int j = i + 1; j < prices.size(); ++j) {
                        // buy at i, sell at j, cooldown at j+1
                        if (iBuyIndex > -1 && prices[iBuyIndex] < prices[j]) {
                            bool bToSell = false;
                            if (j == prices.size() - 1) bToSell = true;
                            if (j + 1 < prices.size())
                                if (prices[j] > prices[j + 1])
                                    bToSell = true;

                            if (bToSell) {
                                iProfix += prices[j] - prices[iBuyIndex]; // sell 
                                iBuyIndex = -1;
                                bTriggerCooldown = true; //
                                std::cout << "s" << j << "; ";
                            }
                        }
                        else if (iBuyIndex == -1) {
                            // not holding pos
                            if (bTriggerCooldown) { // not allow to buy
                                bTriggerCooldown = false; // allow to buy
                            }
                            else {
                                iBuyIndex = j;
                                std::cout << "b" << iBuyIndex << ", ";
                            }
                        }
                    }
                    aProfit[i] = iProfix;
                    std::cout << std::endl;
                }

                int iMax = 0;
                for (int i = 0; i < aProfit.size(); ++i) {
                    iMax = std::max(iMax, aProfit[i]);
                }
                return iMax;
            }
        };
    } // end of cooldown 
    namespace mosttwotransactions { 
        class Solution {
        public:
            int maxProfit_1bs(vector<int>& prices, int begin, int end) {
                // [begin, end)
                if (begin >= end || end == begin + 1) return 0;

                int iMin = prices[begin];
                int iMax = 0;
                for (size_t i = begin; i < end; ++i) {
                    iMin = std::min(prices[i], iMin);
                    iMax = std::max(prices[i] - iMin, iMax);
                }
                return iMax;
            }
            int maxProfit_1bs(vector<int>& prices) {
                if (prices.size() == 0) {
                    return 0;
                }
                return maxProfit_1bs(prices, 0, prices.size());
            }
            // Limit Time Error
            int maxProfit_twoDirections(vector<int>& prices)
            {
                if (prices.size() == 0) return 0;
                // there is a date i, from [0, i), [i, end), seperate two subarray.
                // to find the max (max profit of part1, max profit of part2) for i
                // -- error: Time Limit Exceeded 
                //std::vector<int> aProfits(prices.size(), 0);
                int iMax = 0;
                for (size_t i = 0; i < prices.size(); ++i) {
                    //std::cout << "i" << i << ", "; 
                    //aProfits[i] = maxProfit_1bs(prices,0,i ) + maxProfit_1bs(prices, i, prices.size());
                    iMax = std::max(maxProfit_1bs(prices, 0, i) + maxProfit_1bs(prices, i, prices.size()), iMax);
                }

                //std::vector<int>::iterator itMax = std::max_element(aProfits.begin(), aProfits.end());
                //return *itMax;
                return iMax;
            }
            // Accepted, answer copy from others
            int maxProfit_cost(vector<int>& prices) {
                int iFirstBuy = INT_MIN, iFirstSell = 0;
                int iSecondBuy = INT_MIN, iSecondSell = 0;
                for (size_t i = 0; i < prices.size(); ++i)
                {
                    iFirstBuy = std::max(iFirstBuy, -prices[i]); // cost
                    iFirstSell = std::max(iFirstSell, iFirstBuy + prices[i]);
                    iSecondBuy = std::max(iSecondBuy, iFirstSell - prices[i]);
                    iSecondSell = std::max(iSecondSell, iSecondBuy + prices[i]);
                }
                return iSecondSell;
            }
            int maxProfit(vector<int>& prices) {
                if (prices.size() == 0 || prices.size() == 1) return 0;
                if (std::is_sorted(prices.begin(), prices.end(), std::less_equal<int>())) {
                    return prices[prices.size() - 1] - prices[0];
                }
                bool bNonincreasing = true;
                for (int i = 0; i + 1 < prices.size(); ++i) {
                    if (prices[i] < prices[i + 1]) {
                        bNonincreasing = false;
                        //std::cout << prices[i] << ", " << prices[i+1] << std::endl;
                    }
                }
                std::cout << "non-increasing: " << bNonincreasing << std::endl;
                if (std::is_sorted(prices.begin(), prices.end(), std::greater_equal<int>()))
                {
                    return 0;
                }
                return maxProfit_twoDirections(prices);
            }
        };
    } // namespace at most two transactions      
} // end of bs 


int main()
{
    {
        std::vector<int> prices{ 1, 2, 3, 0, 2 };
        bs::cooldown::Solution s;
        int iMax = s.maxProfit(prices);

    }
    {
        std::vector<int> prices{ 7, 6, 4, 3, 1 };
        bs::mosttwotransactions::Solution s; 
        int iMax = s.maxProfit(prices);
        assert(iMax == 0);

        std::vector<int> test1;
        for (int i = 10000; i >= 0; --i) test1.push_back(i);
        
    }
    return 0;
}