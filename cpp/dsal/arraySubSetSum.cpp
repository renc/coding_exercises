#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;


bool isContains(const std::vector<std::vector<int> >& a, const std::vector<int>& b) {
    // assume: b is sorted. 
    //return false;
    for (int i = 0; i < a.size(); ++i) {
        if (a[i].size() == b.size()) {
            bool bSame = true;
            for (int j = 0; j < a[i].size(); ++j) {
                if (a[i][j] != b[j]) bSame = false;
            }
            if (bSame) return true;
        }
    }
    return false;
}
void print(std::vector<int>& a, int t) {
    std::cout << "[";
    for (auto e : a) std::cout << e << ", ";
    std::cout << "], t=" << t << std::endl;
}
void print(std::vector<int>& a) {
    std::cout << "[";
    for (auto e : a) std::cout << e << ", ";
    std::cout << "], ";
}
void print(std::vector<std::vector<int> >& a) {
    std::cout << "[";
    for (int i = 0; i < a.size(); ++i) {
        print(a[i]);
    }
    std::cout << "].\n";
}

namespace combSum {
// https://leetcode.com/problems/combination-sum/
// unlimit number of times. get combination, not permutation.
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        //print(candidates, target);
        sort(candidates.begin(), candidates.end());



        //std::map<int, std::vector<std::vector<int> > > mapT2Res; // map target value 2 result arrays.
        //return combinationSum_backtracking2(candidates, 0, target, mapT2Res);

        //if (0) 
        {
            std::map<int, std::vector<std::vector<int> > > mapT2Res; // map target value 2 result arrays.
            return combinationSum_backtracking(candidates, target, mapT2Res);
        }
    }

    vector<vector<int>> combinationSum_backtracking2(vector<int>& candidates, int s, int target
        , std::map<int, std::vector<std::vector<int> > >& mapT2Res)
    {
        // 2020-04-30, [s, n-1] can get target ?
        if (mapT2Res.find(target) != mapT2Res.end()) return mapT2Res[target];

        std::vector<std::vector<int> > aResult;
        for (int i = s; i < candidates.size(); ++i) {
            if (target < candidates[i]) {
            }
            else if (target == candidates[i]) {
                std::vector<int> temp = { target };
                if (isContains(aResult, temp) == false) aResult.push_back(temp);
            }
            else {
                // (target > candidates[i] ) 
                // in this case, we can choose with or without [i]
                // first, with 
                //std::cout << "with " << candidates[i] << std::endl;
                std::vector<std::vector<int> > aTemp = combinationSum(candidates, target - candidates[i]);
                for (int t = 0; t < aTemp.size(); ++t) {
                    aTemp[t].push_back(candidates[i]); // add this value back to each combindation.
                    sort(aTemp[t].begin(), aTemp[t].end());
                }
                for (int t = 0; t < aTemp.size(); ++t) {
                    if (isContains(aResult, aTemp[t]) == false) aResult.push_back(aTemp[t]);
                }

                // second, without
                //std::cout << "wout " << candidates[i] << std::endl;
                std::vector<int> aCandidatesWithoutI;
                for (int t = 0; t < candidates.size(); ++t)
                    if (t != i) aCandidatesWithoutI.push_back(candidates[t]);
                aTemp = combinationSum(aCandidatesWithoutI, target);
                for (int t = 0; t < aTemp.size(); ++t) {
                    if (isContains(aResult, aTemp[t]) == false) aResult.push_back(aTemp[t]);
                }
            }
        }
        //print(aResult);
        mapT2Res[target] = aResult;
        return aResult;
    }

    vector<vector<int>> combinationSum_backtracking(vector<int>& candidates, int target
        , std::map<int, std::vector<std::vector<int> > >& mapT2Res)
    {
        // 2020-04-30, + memorization, use mapT2Res to prevent calc of same result. 67 / 168 test cases passed.
        // 2020-04-25, backtracking, 67 / 168 test cases passed.
        //if (mapT2Res.find(target) != mapT2Res.end()) return mapT2Res[target];//wrong
        // this is wrong, because for the same target, there could be many combinations.

        std::vector<std::vector<int> > aResult;
        for (int i = 0; i < candidates.size(); ++i) {
            if (target < candidates[i]) {
            }
            else if (target == candidates[i]) {
                std::vector<int> temp = { target };
                if (isContains(aResult, temp) == false) aResult.push_back(temp);
            }
            else {
                // (target > candidates[i] ) 
                // in this case, we can choose with or without [i]
                // first, with 
                //std::cout << "with " << candidates[i] << std::endl;
                std::vector<std::vector<int> > aTemp = combinationSum_backtracking(candidates, target - candidates[i], mapT2Res);
                for (int t = 0; t < aTemp.size(); ++t) {
                    aTemp[t].push_back(candidates[i]); // add this value back to each combindation.
                    sort(aTemp[t].begin(), aTemp[t].end());
                }
                for (int t = 0; t < aTemp.size(); ++t) {
                    if (isContains(aResult, aTemp[t]) == false) aResult.push_back(aTemp[t]);
                }

                // second, without
                //std::cout << "wout " << candidates[i] << std::endl;
                std::vector<int> aCandidatesWithoutI;
                for (int t = 0; t < candidates.size(); ++t)
                    if (t != i) aCandidatesWithoutI.push_back(candidates[t]);
                aTemp = combinationSum_backtracking(aCandidatesWithoutI, target, mapT2Res);
                for (int t = 0; t < aTemp.size(); ++t) {
                    if (isContains(aResult, aTemp[t]) == false) aResult.push_back(aTemp[t]);
                }
            }
        }
        //print(aResult);
        mapT2Res[target] = aResult;
        return aResult;
    }
    
    
};
} 

namespace combSumIV {
    // https://leetcode.com/problems/combination-sum-iv/ 
    // unlimit number of times. 
    class Solution {
    public:
        int combinationSum4(vector<int>& nums, int target) {
            std::vector<std::vector<int> > aResult;
            std::vector<int> potentialSequence; // this sequency is a potential result.
            combinationSum_backtracking(nums, target, aResult, potentialSequence);
            return aResult.size();
        }
        vector<vector<int>> combinationSum_backtracking(vector<int>& candidates, int target,
            std::vector<std::vector<int> >& aResult,
            std::vector<int>& potentialSequence)
        {
            // 2020-04-30, + memorization, use mapT2Res to prevent calc of same result. 67 / 168 test cases passed.
            // 2020-04-25, backtracking, 67 / 168 test cases passed.
            //if (mapT2Res.find(target) != mapT2Res.end()) return mapT2Res[target];//wrong
            // this is wrong, because for the same target, there could be many combinations.

            for (int i = 0; i < candidates.size(); ++i) {
                if (target < candidates[i]) {
                }
                else if (target == candidates[i]) {
                    std::vector<int> temp = potentialSequence;
                    temp.push_back(target);

                    if (isContains(aResult, temp) == false)
                        aResult.push_back(temp);
                }
                else {
                    // (target > candidates[i] ) 
                    // in this case, we can choose with or without [i]
                    // first, with 
                    //std::cout << "with " << candidates[i] << std::endl;
                    //std::vector<std::vector<int> > aTemp = combinationSum_backtracking(candidates, target - candidates[i]); 
                    //for (int t = 0; t < aTemp.size(); ++t) {
                    //    aTemp[t].push_back(candidates[i]); // add this value back to each combindation. 
                    //}  // renc, later i find this way to return array is not good, since need to insert the current with x element into the front of aTemp[t], otherwise elements in aTemp[t] are inverse order.
                    std::vector<int> potentialSequenceThisTime = potentialSequence;
                    potentialSequenceThisTime.push_back(candidates[i]); // with[i]
                    combinationSum_backtracking(candidates, target - candidates[i],
                        aResult, potentialSequenceThisTime);

                    // second, without
                    //std::cout << "wout " << candidates[i] << std::endl;
                    std::vector<int> aCandidatesWithoutI;
                    for (int t = 0; t < candidates.size(); ++t)
                        if (t != i) aCandidatesWithoutI.push_back(candidates[t]);
                    potentialSequenceThisTime = potentialSequence;
                    combinationSum_backtracking(aCandidatesWithoutI, target,
                        aResult, potentialSequenceThisTime);

                }
            }
            //print(aResult); 
            return aResult;
        }

        bool isContains(const std::vector<std::vector<int> >& a, const std::vector<int>& b) {
            // assume: b is sorted if u want the combination, otherwise it is permuation.. 
            //return false;
            for (int i = 0; i < a.size(); ++i) {
                if (a[i].size() == b.size()) {
                    bool bSame = true;
                    for (int j = 0; j < a[i].size(); ++j) {
                        if (a[i][j] != b[j]) bSame = false;
                    }
                    if (bSame) return true;
                }
            }
            return false;
        }
        void print(std::vector<int>& a) {
            std::cout << "[";
            for (auto e : a) std::cout << e << ", ";
            std::cout << "], ";
        }
        void print(std::vector<std::vector<int> >& a) {
            std::cout << "[";
            for (int i = 0; i < a.size(); ++i) {
                print(a[i]);
            }
            std::cout << "].\n";
        }
    };
}

int main()
{
    {
        std::vector<int> arr{ 3,12,9,11,6,7,8,5,4 };
        arr = std::vector<int>{ 3,5,4 };
        int target = 15;
        combSum::Solution solution;
        std::vector<std::vector<int> > aResult = solution.combinationSum(arr, target);
    }
    {
        std::vector<int> arr{ 3,12,9,11,6,7,8,5,4 };
        arr = std::vector<int>{ 1,2, 3 };
        int target = 4;
        combSumIV::Solution solution;
        int iResult = solution.combinationSum4(arr, target);
    }
    return 0;
}