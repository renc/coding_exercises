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

// from SG online test. 
void spread_bt(std::vector<std::vector<int> >& pile, int x, int y) {
    if (x < 0 || y < 0 || x >= pile.size() || y >= pile.size()) {
        return;
    }
    if (pile[x][y] >= 4) {
        pile[x][y] = 0;

        if (x - 1 >= 0) {
            // up
            pile[x - 1][y] += 1;
            spread_bt(pile, x - 1, y);
        }
        if (y - 1 >= 0) {
            // left
            pile[x][y - 1] += 1;
            spread_bt(pile, x, y - 1);
        }
        if (x + 1 < pile.size()) {
            // down
            pile[x + 1][y] += 1;
            spread_bt(pile, x + 1, y);
        }
        if (y + 1 < pile.size()) {
            // right 
            pile[x][y + 1] += 1;
            spread_bt(pile, x, y + 1);
        }
    }
}
// not sure if this is correct 
std::vector<std::vector<int>> sandpile(std::vector<std::vector<int> > pile, int n) {

    const int psize = pile.size();
    int centerx = psize / 2;
    for (int i = 0; i < n; ++i) {
        pile[centerx][centerx] += 1;
        spread_bt(pile, centerx, centerx);
    }
    return pile;
}
void print(const std::vector<std::vector<int>>& a)
{
    for (auto& e : a) {
        for (auto& ee : e) {
            std::cout << ee << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl; 
}
int main() 
{
    {     
        std::vector<std::vector<int> > pile{
            {1, 2, 1},
            {1, 3, 1},
            {1, 2, 1}
        };
        std::vector<std::vector<int>> result = sandpile(pile, 1);
        print(result); 
    }
    {
        std::vector<std::vector<int> > pile{
            {1, 3, 1},
            {3, 3, 3},
            {1, 3, 1}
        };
        std::vector<std::vector<int>> result = sandpile(pile, 1);
        print(result);
    }
    {
        std::vector<std::vector<int> > pile{
            {3, 3, 3},
            {3, 3, 3},
            {3, 3, 3}
        };
        std::vector<std::vector<int>> result = sandpile(pile, 1);
        print(result);
    }
    {
        std::vector<std::vector<int> > pile{
            {0, 0, 3, 0, 0},
            {0, 0, 3, 0, 0},
            {3, 3, 3, 3, 3},
            {0, 0, 3, 0, 0},
            {0, 0, 3, 0, 0}
        };
        std::vector<std::vector<int>> result = sandpile(pile, 1);
        print(result);
    }
    {
        std::vector<std::vector<int> > pile{
            {0, 0, 3, 0, 0},
            {0, 0, 3, 0, 0},
            {3, 3, 3, 3, 3},
            {0, 0, 3, 0, 0},
            {0, 0, 3, 0, 0}
        };
        std::vector<std::vector<int>> result = sandpile(pile, 4);
        print(result);
    }
    {
        std::vector<std::vector<int> > pile{
            {0, 0, 3, 0, 0},
            {0, 0, 3, 0, 0},
            {3, 3, 3, 3, 3},
            {0, 0, 3, 0, 0},
            {0, 0, 3, 0, 0}
        };
        std::vector<std::vector<int>> result = sandpile(pile, 5);
        print(result);
    }
    {
        std::vector<std::vector<int> > pile{
            {0, 0, 3, 0, 0},
            {0, 0, 3, 0, 0},
            {3, 3, 3, 3, 3},
            {0, 0, 3, 0, 0},
            {0, 0, 3, 0, 0}
        };
        std::vector<std::vector<int>> result = sandpile(pile, 9);
        print(result);
    }
    getchar();
    return 1; // ok
}
