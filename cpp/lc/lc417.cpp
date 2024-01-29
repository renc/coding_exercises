// https://leetcode.com/problems/pacific-atlantic-water-flow/description/

#include <iostream>
#include <cstdint>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;


class Solution {
public:
    void visit(int r, int c, const vector<vector<int>>& heights, vector<vector<int>>& cache)
    {
        if (cache[r][c] > 0)
        {
            if (r - 1 >= 0) { // up at the same column
                if (heights[r][c] <= heights[r-1][c]    // can move one to new location
                    && cache[r][c] != cache[r-1][c]) {  // the visited value is not the same (or dead loop)
                    int oldv = cache[r-1][c];
                    cache[r-1][c] += cache[r][c];  cache[r-1][c] = std::clamp(cache[r-1][c], 0, 3);
                    std::cout << "[" <<r << "," << c << "]: " << cache[r][c] << " up " << oldv << "->" << cache[r-1][c] << "; " ; 
                    visit(r-1, c, heights, cache);
                }
            }
            if (c - 1 >= 0) { // left at the same row
                if (heights[r][c] <= heights[r][c-1] 
                    && cache[r][c] != cache[r][c-1]) {
                        int oldv = cache[r][c-1];
                    cache[r][c-1] += cache[r][c];  cache[r][c-1] = std::clamp(cache[r][c-1], 0, 3);
                    std::cout << "[" <<r << "," << c << "]: " << cache[r][c] << " lf " << oldv << "->" << cache[r][c-1] << "; ";
                    visit(r, c-1, heights, cache);
                }
            }
            if (c + 1 < heights[r].size()) { // right at the same row
                if (heights[r][c] <= heights[r][c+1]
                    && cache[r][c] != cache[r][c+1]) {
                        int oldv = cache[r][c+1];
                    cache[r][c+1] += cache[r][c];  cache[r][c+1] = std::clamp(cache[r][c+1], 0, 3);
                    std::cout << "[" <<r << "," << c << "]: " << cache[r][c] << " rt " << oldv << "->" << cache[r][c+1] << "; ";
                    visit(r, c+1, heights, cache);
                }
            }
            if (r + 1 < heights.size()) { // down at the same column
                if (heights[r][c] <= heights[r+1][c]
                    && cache[r][c] != cache[r+1][c]) {
                        int oldv = cache[r+1][c];
                    cache[r+1][c] += cache[r][c];  cache[r+1][c] = std::clamp(cache[r+1][c], 0, 3);
                        std::cout << "[" <<r << "," << c << "]: " << cache[r][c] <<  " dw " << oldv << "->" << cache[r+1][c] <<"; ";
                    visit(r+1, c, heights, cache);
                }
            }
        } else {
            // == 0, no one visited yet 
        }
    }
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        // travel from ocean to higher places, 
        // from pacific ocean, the value is 1;
        // from atlantic ocean, the value is 2; 
        // from both oceans, the value is 3.
        std::vector<std::vector<int> > cache;
        cache.resize(heights.size());
        for (size_t r = 0; r < heights.size(); ++r)
        {
            cache[r].resize(heights[r].size());
            for (size_t c = 0; c < heights[r].size(); ++c)
                cache[r][c] = 0;
        }
        for (size_t r = 0; r < heights.size(); ++r)
        {
            for (size_t c = 0; c < heights[r].size(); ++c)
            {
                if (r == 0 || c == 0)
                {
                    if (cache[r][c] == 0 || cache[r][c] == 2)
                        cache[r][c] += 1; // from pacific ocean 
                }
                else if (r == heights.size()-1 || c == heights[r].size()-1)
                {
                    if (cache[r][c] == 0 || cache[r][c] == 1)
                        cache[r][c] += 2; // from atlantic ocean
                }
                visit(r, c, heights, cache);
            }
        }

        std::vector<std::vector<int> > result;
        for (size_t r = 0; r < heights.size(); ++r)
        {
            for (size_t c = 0; c < heights[r].size(); ++c)
            {
                if (cache[r][c] >= 3)
                {
                    std::vector<int> loc; loc.resize(2); loc[0] = r; loc[1] = c;
                    result.push_back(loc);
                }
            }
        }
        return result;
    }
};

int main(int argc, char* argv[])
{
    {
    std::vector<std::vector<int> > heights;
    // [[1,2,2,3,5],[3,2,3,4,4],[2,4,5,3,1],[6,7,1,4,5],[5,1,1,2,4]]
    heights.push_back({1,2,2,3,5});
    heights.push_back({3,2,3,4,4});
    heights.push_back({2,4,5,3,1});
    heights.push_back({6,7,1,4,5});
    heights.push_back({5,1,1,2,4});
    
    Solution solution;
    std::vector<std::vector<int> > result = solution.pacificAtlantic(heights);
    for (auto &v : result)
    {
        std::cout << "(" << v[0] << ", " << v[1] << ") ";
    } std::cout << "\n";
    return 0;
}
