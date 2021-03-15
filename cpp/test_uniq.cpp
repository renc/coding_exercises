#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <cassert>

using namespace std;

bool isFizz(const pair<uint64_t, uint64_t> fizz_and_buzz, uint64_t x) {
    // const auto [f,b] = fizz_and_buzz;  // enable c++17 
    const uint64_t f = fizz_and_buzz.first;
    const uint64_t b = fizz_and_buzz.second; 
    if (x % f == 0) return true;
    if (x % b == 0) return true;
    uint64_t y = 10;
    for (y = 10; y < f; y *= 10) {} // in case f or b > 9
    if (x % y == f) return true;
    y = 10;
    for (y = 10; y < b; y *= 10) {} // in case f or b > 9
    if (x % y == b) return true;
    return false;
}

pair<uint64_t, uint64_t> findLargestRange(
    const pair<uint64_t, uint64_t> start_range,
    const pair<uint64_t, uint64_t> fizz_and_buzz,
    const uint32_t window, std::string &ss)
{
    if (window == 0) return pair<uint64_t, uint64_t>{0, 0};
    // const auto [n, m] = start_range; //c++17
    const uint64_t n = start_range.first, m = start_range.second;

    pair<uint64_t, uint64_t> maxCount{ 0, 0 }; // result;
    ss = "";
    for (uint64_t i = n; i <= m; ++i) {
        const bool bFizz = isFizz(fizz_and_buzz, i);
        if (bFizz) ss += "F";
        else ss += std::to_string(i);
        if (!bFizz) continue;
         
        int iCount = 0;
        for (uint64_t j = i; j <= i + window - 1 && j <=m; ++j) {
            // [i, j] 
            if (isFizz(fizz_and_buzz, j) == true) {
                ++iCount;
            }
        }
        if (iCount > maxCount.second) {
            maxCount.first = i;
            maxCount.second = iCount; 
        }
    }
    return maxCount; // probably wrong
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    pair<uint64_t, uint64_t> range, fizz_and_buzz;
    uint64_t window_size;
    {
        range.first = 1; range.second = 10;
        fizz_and_buzz = { 3, 5 };
        window_size = 5;
        std::string ss; 
        pair<uint64_t, uint64_t> answer = findLargestRange(range, fizz_and_buzz, window_size, ss);
        assert(answer.first == 3 && answer.second == 3);
        cout << answer.first << " " << answer.second << std::endl;
    } 
    {
        range.first = 1; range.second = 20;
        fizz_and_buzz = { 3, 5 };
        window_size = 5;
        std::string ss;
        pair<uint64_t, uint64_t> answer = findLargestRange(range, fizz_and_buzz, window_size, ss);
        assert(answer.first == 9 && answer.second == 4);
        cout << answer.first << " " << answer.second << std::endl;
    }
    {
        range.first = 1; range.second = 36;
        fizz_and_buzz = { 3, 5 };
        window_size = 5;
        std::string ss;
        pair<uint64_t, uint64_t> answer = findLargestRange(range, fizz_and_buzz, window_size, ss);
        assert(answer.first == 9 && answer.second == 4);
        cout << answer.first << " " << answer.second << std::endl;
    }
    return 1; // ok
}
