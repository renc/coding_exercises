#include <stdio.h> // for print
#include <stdlib.h> // for srand, rand
#include <time.h> // for time
#include <assert.h> // for assert
#include <vector> // for std::vector

// quick sort
// s                      e 
// 6 6 3 8 7 4 5 9 0 1 6 2 || 6 6 3 8 7 4 5 9 0 1 6 2  || 6 6 3 2 7 4 5 9 0 1 6 8  
// p_i___________________j || p_____i_______________j  || p_______i___________j__
// 6 6 3 2 7 4 5 9 0 1 6 8 || 6 6 3 2 1 4 5 9 0 7 6 8  || 6 6 3 2 1 4 5 9 0 7 6 8     
// p_______i_________j____ || p_________i_____j______  || p_____________i_j______
// 6 6 3 2 1 4 5 0 9 7 6 8 if j < i we stop, some properties: x at the left side of i are <= p;
// p_____________j_i______ y at the right side of j are >= p; 
// since we use a[s] as pivot, after each round, we need to swap p to the middle and do not sort it 
// in the next round of two sides(left, right), x<=p, p, p<=y; but swap p with j or i ? both are ok. 
// 0 6 3 2 1 4 5 6 9 7 6 8  
// ______________j_i______
void myqsort(std::vector<int> &a, int s, int e)
{
	if (s >= e || s < 0 || e > a.size())
		return;

	const int p = a[s]; // use the first one as pivot
	int i = s + 1;
	int j = e - 1; 
	if (i > j)		    // if (i>=j) error, for example {3 2};
		return;

	bool bRun = true;
	while (bRun)
	{
		// 1. i cannot = e. 2. ==p can at the left side.
		while (i < e && a[i] <= p)
			++i; 
		// 1. s is used by pivot, j stop when == s. 2. ==p can at the right side too.
		while (j > s && a[j] >= p)
			--j;

		if (j > i)
		{
			int t = a[i];
			a[i] = a[j];
			a[j] = t;

			++i;
			--j;
		}
		else if (j < i)
		{
			a[s] = a[j];
			a[j] = p;

			bRun = false;
		}
		else if (j == i)
			assert(false); // why not possible in our case (a[s] is pivot and x<=p and y>= p)?
	};

	if (0)
	{
		printf("Afater sort array: (p%d, s%d, e%d) \n", p, s, e);
		for (int ii = 0; ii < a.size(); ++ii)
		{
			printf("%d, ", a[ii]); 
		}
		printf("\n");
	}

	myqsort(a, s, j);
	myqsort(a, i, e);
} 

int main(char **, int)
{
	/*
	int b[] = {6,6,3,8,7,4,5,9,0,1,6,2 }; 
	std::vector<int> a;
	a.insert(a.end(), &b[0], &b[sizeof(b) / sizeof(int)] );	
	*/
	srand( time(0) ); 
	std::vector<int> a(10);
	for (int i = 0; i < a.size(); ++i)
	{
		a[i] = rand(); 
	}
	printf("Init array: \n");
	for (int i = 0; i < a.size(); ++i)
	{
		printf("%d, ", a[i]); 
	}
	printf("\n");
	
	myqsort(a, 0, a.size()); 
				 	
	printf("Final array: \n");
	for (int i = 0; i < a.size(); ++i)
	{
		printf("%d, ", a[i]); 
	}
	printf("\n");
	
	// to check if the result is right or wrong
	bool bSucceed = true;
	for (int i = 0; i < a.size() - 1; ++i)
	{
		if (a[i] > a[i + 1])
		{
			bSucceed = false;
			break;
		}
	}
	if (bSucceed)
		printf("myqsort - Succeed!\n");
	else
		printf("myqsort - Fail!\n");
	scanf("%d");

	return 1; 
}