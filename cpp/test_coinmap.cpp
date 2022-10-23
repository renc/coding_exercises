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


struct Coin {
	int x{ 0 }, y{ 0 };
	Coin(int r, int c) :x(r), y(c) {};
	bool at(int r, int c) const { return (x == r) && (y == c); }
};
bool target(const std::vector<Coin>& coins)
{
	if ((coins[0].at(2, 2) && coins[1].at(4, 2)) || (coins[1].at(2, 2) && coins[0].at(4, 2)))
	{
		if ((coins[2].at(3, 5) && coins[3].at(4, 3)) || (coins[3].at(3, 5) && coins[2].at(4, 3)))
		{
			return true;
		}
	}
	return false;
}
bool solve(std::vector<std::vector<int>> map, std::vector<Coin> coins, int ci)
{
	if (ci >= coins.size()) return false;

	if (target(coins))
	{
		std::cout << "Find solution\n";
		return true;
	}
	else
	{
		// coin i, without move
		solve(map, coins, ci + 1);

		// coin i, with move 1 step
		auto curCoin = coins[ci];
		{
			for (int r = 0; r < map.size(); ++r)
			{
				for (int c = 0; c < map[r].size(); ++c)
				{
					if (map[r][c] == -1)
					{
						if ((fabs(curCoin.x - r) == 2 && fabs(curCoin.y - c) == 1)
							|| (fabs(curCoin.x - r) == 1 && fabs(curCoin.y - c) == 2))
						{
							std::vector<std::vector<int>> newMap = map;
							newMap[r][c] = ci; // place the coin 
							
							std::vector<Coin> newCoins = coins;
							newCoins[ci].x = r; newCoins[ci].y = c;


							solve(newMap, newCoins, ci);// maybe move the step again
							
							solve(newMap, newCoins, ci + 1);
						}
					}
				}
			}
		}
	}
	return ;
}

int main()
{
	std::vector<Coin> coins{ {3, 5}, {4, 3}, {2, 2}, {4, 2} };
	std::vector<std::vector<int>> map(10);
	for (std::size_t i = 0; i < map.size(); ++i)
	{
		map[i] = std::vector<int>(10, -2);
	} // -2, out of map; -1, inside the map, available; 0/1/2/3 coin occuped;
	map[2][2] = map[3][2] = map[4][2] = map[5][2] = -1;
				map[3][3] = map[4][3] = map[5][3] = -1;
				map[3][4] = map[4][4] = -1;
		     	map[3][5] = -1;
	
	for (std::size_t i = 0; i < coins.size(); ++i)
	{
		const auto& coin = coins[i];
		map[coin.x][coin.y] = i;
	}

	solve(map, coins, 0);
	
	return 1;
}