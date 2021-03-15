
#include "common.hpp"
using namespace std;

//https://leetcode.com/problems/minimum-cost-for-tickets/
class Solution {
public:
    int mincostTickets(vector<int>& days, vector<int>& costs) {
        std::cout << "days" << util::toString(days) << std::endl;
        std::cout << "cost" << util::toString(costs) << std::endl;
        int iMin = imp(days, costs);
        return iMin; 
    }
private:
    // private since no need for users to call this. 
    virtual int imp(vector<int>& days, vector<int>& costs) {
        std::cout << "---- new instance ----\n";
        aCache = std::vector<int>(days.size(), INT_MAX);
        aTicket = std::vector<int>(days.size(), 0);
        int iMin = mincostTickets_bt(days, 0, costs);
        std::cout << "days" << util::toString(days) << std::endl;
        std::cout << "cost" << util::toString(costs) << std::endl;
        std::cout << "minC" << util::toString(aCache) << std::endl;
        std::cout << "Tkts" << util::toString(aTicket) << std::endl;
        return iMin;
    }
    std::vector<int> aCache; // aCache[s]: start at s, the min cost to end .
    std::vector<int> aTicket; // aCache[s]; at s, buy which tick, 0 means no need to buy. 
    // https://leetcode.com/discuss/interview-question/algorithms/124720/tickets-needed-to-get-minimum-cost
    // s: start index in this round.
    int mincostTickets_bt(vector<int>& days, int s, vector<int>& costs) {
        if (s >= days.size()) return 0;
        if (aCache[s] != INT_MAX) return aCache[s];
        std::string sPrefix(s, '-');
        std::cout << sPrefix << "s=" << s << std::endl;
        // 1-day, costs[0]
        std::cout << sPrefix << "1:b \n";
        int iChoise1 = costs[0] + mincostTickets_bt(days, s + 1, costs);
        std::cout << sPrefix << "1:c" << iChoise1 << "\n";
        // 7-day, costs[1]
        int i = 0;
        for (i = 0; i <= 6; ++i) { 
            if (s + i < days.size()) {
                if (days[s + i] <= days[s] + 6) {}
                else break;
            }
            else break;
        }
        std::cout << sPrefix << "7:b" << s + i << " \n";
        int iChoise7 = costs[1] + mincostTickets_bt(days, s + i, costs);
        std::cout << sPrefix << "7:c" << iChoise7 << "\n";
        //30-day, costs[2] 
        int j = 0; 
        for (j = 0; j <= 29; ++j) { 
            if (s + j < days.size() && days[s + j] <= days[s] + 29) {}
            else break;
        }
        std::cout << sPrefix << "30:b" << s + j << " \n";
        int iChoise30 = costs[2] + mincostTickets_bt(days, s + j, costs);
        std::cout << sPrefix << "30:c" << iChoise30 << "\n";

        std::cout << sPrefix << "s=" << s << ", d[s]=" << days[s] << ", c[" 
            << iChoise1 << ", " << iChoise7 << ", " << iChoise30<< "].min" 
            << std::min(iChoise1, std::min(iChoise7, iChoise30)) << " \n";
        if (iChoise1 <= iChoise7 && iChoise1 <= iChoise30) {
            aCache[s] = iChoise1; aTicket[s] = 1;
            return iChoise1;
        }
        else if (iChoise7 <= iChoise1 && iChoise7 <= iChoise30) {
            aCache[s] = iChoise7; aTicket[s] = 7; //for (int t = 1; t < i; ++t) aTicket[s + t] = 0;
            return iChoise7;
        }
        aCache[s] = iChoise30; aTicket[s] = 30; //for (int t = 1; t < j; ++t) aTicket[s + t] = 0;
        return iChoise30;
        //return std::min(iChoise1, std::min(iChoise7, iChoise30)); //same/
    }
};

class SolutionDP : public Solution {
public:
    int imp (vector<int>& days, vector<int>& costs) override {
        return mincostTickets_dp(days, costs);
    }
    int mincostTickets_dp(vector<int> & days, vector<int> & costs) {
        std::cout << "mincostTickets_dp() start ...\n";
        if (days.size() == 0) return 0;
        std::vector<int> aCache = std::vector<int>(days.size(), INT_MAX);
        std::vector<int> aTicket = std::vector<int>(days.size(), 0);
        
        // from backword 
        aCache[days.size() - 1] = util::min(costs);// 1 days may not be the cheapest 
        for (int s = days.size() - 2; s >= 0; --s) {
            // 1days
            int c1 = costs[0] + aCache[s + 1];
            // 7days
            int i7 = 0;
            for (; i7 <= 6; ++i7)
            {
                if (s + i7 <= days.size() - 1 && days[s + i7] <= days[s] + 6) {}
                else break;
            }
            int c7 = costs[1];
            if (s + i7 <= days.size() - 1) c7 += aCache[s + i7];
            // 30days
            int i30 = 0;
            for (; i30 <= 29; ++i30) // offset: day0 to day29
            {
                if (s + i30 <= days.size() - 1 && days[s + i30] <= days[s] + 29) {}
                else break;
            }
            int c30 = costs[2];
            if (s + i30 <= days.size() - 1) c30 += aCache[s + i30];
            int iMinCAtS = std::min(c1, std::min(c7, c30));
            aCache[s] = iMinCAtS;
            std::cout << "s" << s << ", " <<  util::toString(aCache) << std::endl;
        }

        std::cout << "days" << util::toString(days) << std::endl;
        std::cout << "cost" << util::toString(costs) << std::endl;
        std::cout << "minC" << util::toString(aCache) << std::endl;
        std::cout << "Tkts" << util::toString(aTicket) << std::endl;
        return aCache.front();
    }
};
 

int main()
{
    if (0) {
        std::vector<int> days{ 1, 4, 6, 7, 8, 20 };
        std::vector<int> costs{ 2, 7, 15 };
        int iExpected = 11;
        Solution s; int iMin = s.mincostTickets(days, costs);  assert(iMin == iExpected);
        {
            std::unique_ptr<Solution> pS = std::make_unique<SolutionDP>();
            int iMin = pS->mincostTickets(days, costs);
            assert(iMin == iExpected);
        }
    }if (1) {
        std::vector<int> days{ 1, 4, 6, 7, 8, 20 };
        std::vector<int> costs{ 7, 2, 15 }; // 7days is way cheapter than 1day 
        int iExpected = 6;
        Solution s; int iMin = s.mincostTickets(days, costs);  assert(iMin == iExpected);
        {
            std::unique_ptr<Solution> pS = std::make_unique<SolutionDP>();
            int iMin = pS->mincostTickets(days, costs);
            assert(iMin == iExpected);
        }
    } 
    if (1) {
        std::vector<int> days{ 1, 4, 6, 9, 10, 11, 12, 13, 14, 15 };
        std::vector<int> costs{ 3, 13, 45 };
        int iExpected = 22;
        Solution s; int iMin = s.mincostTickets(days, costs);  assert(iMin == iExpected);
        {
            std::unique_ptr<Solution> pS = std::make_unique<SolutionDP>();
            int iMin = pS->mincostTickets(days, costs);
            assert(iMin == iExpected);
        }
    }
    if (1) {
        std::vector<int> days{ 1, 4, 6, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 20, 21, 22, 23, 27, 28 };
        std::vector<int> costs{ 3, 13, 45 };
        int iExpected = 44;
        Solution s; int iMin = s.mincostTickets(days, costs);  assert(iMin == iExpected);
        {
            std::unique_ptr<Solution> pS = std::make_unique<SolutionDP>();
            int iMin = pS->mincostTickets(days, costs);
            assert(iMin == iExpected);
        }
    } 
    return 0; 
}