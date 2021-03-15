#include <algorithm> 
#include <cmath> 

namespace containsduplicateiii {
    class Solution {
    public:
        // pass 41/41, but time limit exceeded. 
        bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {
            if (nums.size() == 1) return false; // i, j need to diff
            if (k == 0 || t < 0) return false;
            std::cout << "arr.size=" << nums.size() << ", k" << k << ",t" << t << std::endl;
            if (k >= nums.size()) {
                std::sort(nums.begin(), nums.end());
                for (int i = 0; i <= nums.size() - 2; ++i) {
                    if (std::abs(nums[i] - nums[i + 1]) <= t) return true;
                }
                return false;
            }
            int left = 0;
            std::vector<int> arr;
            for (int i = k; i < nums.size(); ++i, ++left) {
                assert(i - left == k);
                if (i == k) {  // sort the first time
                    arr = std::vector(nums.begin() + left, nums.begin() + i + 1);
                    std::sort(arr.begin(), arr.end());
                    //std::cout << "ik" << i <<"; \n"; 
                    for (int j = 0; j <= k - 1; ++j) {
                        if (((long)arr[j + 1] - (long)arr[j]) <= (long)t) return true;
                    }
                }
                else {
                    // need to swap nums[left-1] with the nums[i]

                    if (nums[left - 1] == nums[i]) {}
                    else {
                        int tgt = t; // since the t is missued as index later :-( 
                        int t = 0;
                        for (; t < arr.size(); ++t) {
                            if (arr[t] == nums[left - 1]) { break; }
                        } assert(t < arr.size()); // must find!
                        arr[t] = nums[i];  // the new item added.
                        int iNewPos = t;
                        if (t + 1 < arr.size()) {
                            if (arr[t] > arr[t + 1]) {
                                for (int u = t; u <= arr.size() - 2; ++u) {
                                    if (arr[u] > arr[u + 1]) std::swap(arr[u], arr[u + 1]);
                                    else {
                                        iNewPos = u;
                                        break;
                                    }
                                }
                            }
                        }
                        if (0 <= t - 1) {
                            if (arr[t - 1] > arr[t]) {
                                for (int u = t - 1; u >= 0; --u) {
                                    if (arr[u] > arr[u + 1]) std::swap(arr[u], arr[u + 1]);
                                    else {
                                        iNewPos = u;
                                        break;
                                    }
                                }
                            }
                        }
                        //std::for_each(arr.begin(), arr.end(), [](int x) {std::cout << x << ", ";}); std::cout << ".\n";
                        //std::cout << "i" << i << "; ";
                        //std::cout << "newPos" << iNewPos << "; ";
                        if (0 <= iNewPos - 1) {
                            if ((long)arr[iNewPos] - (long)arr[iNewPos - 1] <= (long)tgt) return true;
                        }
                        if (iNewPos + 1 < arr.size()) {
                            long diff = (long)arr[iNewPos + 1] - (long)arr[iNewPos];
                            //std::cout << "diff" << diff << ";";
                            if (diff <= (long)tgt) return true;
                            //else { std::cout << "Here?";}
                        }
                    }



                    //for (int j = 0; j <= k-1; ++j) {
                    //    if ( ((long)arr[j+1] - (long)arr[j]) <= (long)t) return true;
                    //}
                }
            }
            return false;
        }
    };
}