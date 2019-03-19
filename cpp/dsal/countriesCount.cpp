#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

void toVisitedNeighbor(std::vector<std::vector<int> > &A, int r, int c,
	std::vector<std::vector<bool> > &visited)
{ 
	if (r < 0 || r >= A.size())
		return;
	if (c < 0 || c >= A[r].size())
		return;

	// A[r][c] is visited, 
	// to check its neightboor
	assert(visited[r][c] == true);
	
	// up
	if (r - 1 >= 0) {
		if (A[r - 1][c] == A[r][c] && visited[r - 1][c] == false)
		{
			visited[r - 1][c] = true;
			toVisitedNeighbor(A, r - 1, c, visited);
		}
	}
	// down
	if (r + 1 < A.size()) {
		if (A[r + 1][c] == A[r][c] && visited[r + 1][c] == false)
		{
			visited[r + 1][c] = true;
			toVisitedNeighbor(A, r + 1, c, visited);
		}
	}
	// left 
	if (c - 1 >= 0) {
		if (A[r][c - 1] == A[r][c] && visited[r][c - 1] == false)
		{
			visited[r][c - 1] = true;
			toVisitedNeighbor(A, r, c - 1, visited);
		}
	}
	// right 
	if (c + 1 < A[r].size()) {
		if (A[r][c + 1] == A[r][c] && visited[r][c + 1] == false)
		{
			visited[r][c + 1] = true;
			toVisitedNeighbor(A, r, c + 1, visited);
		}
	}
}

int solution(vector< vector<int> > &A) {
	// write your code in C++14 (g++ 6.2.0)

	std::vector< std::vector<bool> > visited(A.size());
	for (size_t i = 0; i < A.size(); ++i)
	{
		visited[i] = std::vector<bool>(A[i].size(), false);
	}

	int iCountryCount = 0;
	for (size_t r = 0; r < A.size(); ++r)
	{
		for (size_t c = 0; c < A[r].size(); ++c)
		{
			if (visited[r][c] == false)
			{
				++iCountryCount;
				visited[r][c] = true;

				toVisitedNeighbor(A, r, c, visited);
			}
		}
	}
	return iCountryCount;
}

int main_()
{
	std::vector< std::vector<int> > a1 = { 
		{5, 4, 4},
		{4, 3, 4},
		{3, 2, 4},
		{2, 2, 2},
		{3, 3, 4},
		{1, 4, 4},
		{4, 1, 1}
	
	};
	int s1 = solution(a1);//11

	std::vector< std::vector<int> > a2 = {

	};
	int s2 = solution(a2);
	std::vector< std::vector<int> > a3 = {
		{1}
	};
	int s3 = solution(a3);
	return 0;
} 