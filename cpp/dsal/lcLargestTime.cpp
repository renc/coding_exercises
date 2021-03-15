#include <algorithm>
#include <string>
#include <iostream> 

namespace largesttimeforgivendigits {
    class Solution {
    public:
        bool isValidTime(const std::vector<int>& a) {
            //for (auto e: a) { std::cout << e << " " ; } std::cout << ".\n"; 
            if (a.size() != 4) return false;
            if (a[0] > 2) return false;
            if (a[0] == 2 && a[1] > 3) return false;
            if (a[2] > 5) return false;
            return true;
        }
        // Accepted !
        // to enumerate all the possible permutation and check whether it is a valid time
        string largestTimeFromDigits(vector<int>& A) {
            //for (auto e: A) { std::cout << e << " " ; } std::cout << ".\n"; 
            std::sort(A.begin(), A.end());
            std::vector<int> aLargest;
            do {
                if (isValidTime(A))
                    aLargest = A;
                if (aLargest.size() && false) {
                    for (auto e : aLargest) { std::cout << e << " "; }
                    std::cout << ". largest.\n";
                }
            } while (std::next_permutation(A.begin(), A.end()));

            if (aLargest.size() == 0) return "";
            std::string result = std::to_string(aLargest[0]) + std::to_string(aLargest[1]) + ":"
                + std::to_string(aLargest[2]) + std::to_string(aLargest[3]);
            //std::cout << "Result=" << result << std::endl; 
            return result;
        }
        // [2,0,6,6] fail
        string largestTimeFromDigits_findDigit(vector<int>& A) {
            for (auto e : A) { std::cout << e << " "; } std::cout << ".\n";
            //std::sort(A.begin(), A.end());
            std::string result;
            std::vector<int> d(4, -1);
            // first char
            int idx0 = -1;
            for (int i = 0; i < A.size(); ++i) {
                if (A[i] == 0) {
                    if (idx0 == -1) idx0 = i;
                } if (A[i] == 1) {
                    if (idx0 == -1) idx0 = i;
                    else {
                        if (A[i] > A[idx0]) { idx0 = i; }
                    }
                }
                if (A[i] == 2) { idx0 = i; }
            }
            if (idx0 != -1) {
                d[0] = A[idx0]; A[idx0] = 10;
            }
            else
                return "";

            // second digit
            int idx1 = -1;
            for (int i = 0; i < A.size(); ++i) {
                if (A[i] > 9) continue;
                if (d[0] == 0 || d[0] == 1) {
                    if (idx1 == -1) { idx1 = i; }
                    else if (idx1 != -1 && A[i] > A[idx1]) {
                        idx1 = i;
                    }
                }
                else if (d[0] == 2) {
                    if (A[i] == 0 || A[i] == 1 || A[i] == 2) {
                        if (idx1 == -1) idx1 = i;
                        else {
                            if (A[i] > A[idx1]) { idx1 = i; }
                        }
                    }
                    if (A[i] == 3) { idx1 = i; }
                }
            }
            if (idx1 != -1) {
                d[1] = A[idx1]; A[idx1] = 10;
            }
            else return "";

            // third digit
            int idx2 = -1;
            for (int i = 0; i < A.size(); ++i) {
                if (A[i] > 9) continue;
                if (A[i] <= 5) {
                    if (idx2 == -1) { idx2 = i; }
                    else {
                        if (A[i] > A[idx2]) {
                            idx2 = i;
                        }
                    }
                }
            }
            if (idx2 != -1) {
                d[2] = A[idx2]; A[idx2] = 10;
            }
            else
                return "";
            // forth digit
            int idx3 = -1;
            for (int i = 0; i < A.size(); ++i) {
                if (A[i] > 9) continue;
                idx3 = i;
            }
            if (idx3 != -1) {
                d[3] = A[idx3]; A[idx3] = 10;
            }
            for (auto e : d) { std::cout << e << " "; } std::cout << ".\n";

            result = std::to_string(d[0]) + std::to_string(d[1]) + ":"
                + std::to_string(d[2]) + std::to_string(d[3]);
            std::cout << "Result=" << result << std::endl;

            return result;
        }
    };
} // namespace largesttimeforgivendigits