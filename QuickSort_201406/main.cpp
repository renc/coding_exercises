
/*
Requirement:

	to count the comparion in quick sort implement.

	The file contains all of the integers between 1 and 10,000 
	(inclusive, with no repeats) in unsorted order. 
	The integer in the i^{th} row of the file gives you the i^{th} entry of an input array.

	Your task is to compute the total number of comparisons used to sort the given input file by QuickSort. 
	As you know, the number of comparisons depends on which elements are chosen as pivots,
	so we'll ask you to explore three different pivoting rules.

	You should not count comparisons one-by-one. Rather, when there is a recursive call on a subarray of length m, 
	you should simply add m-1 to your running total of comparisons. 
	(This is because the pivot element is compared to each of the other m-1 elements in the subarray in this recursive call.)

	WARNING: The Partition subroutine can be implemented in several different ways, 
	and different implementations can give you differing numbers of comparisons.

Implement:
	the partition and quick-sort are implement as descriped at the video lector.
*/ 

#include <vector>
#include <iostream> // for std::cout;
#include <fstream>
#include <assert.h>
#include <algorithm> // for std::sort

// to use the first element as pivot
unsigned int choosePivotFirst(const std::vector<int> &arr, unsigned int s, unsigned int e)
{
	return s;
}

// to use the last element as pivot
unsigned int choosePivotLast(const std::vector<int> &arr, unsigned int s, unsigned int e)
{
	return e - 1;
}

// to use the "median-of-three" pivot rule.
unsigned int choosePivotX(const std::vector<int> &arr, unsigned int s, unsigned int e)
{
	if (arr.size() == 0)
		return 0; 


	int iF = arr[s];
	int iL = arr[e - 1];
	const int iSize = e - s;
	int iMiddleIdx = 0;
	if (iSize % 2)
	{
		// the array has odd length 
		iMiddleIdx = s + iSize / 2;
	}
	else
	{
		iMiddleIdx = s + iSize / 2 - 1;
		assert( iMiddleIdx == s + (iSize - 1) / 2 ); 
	}
	int iM = arr[iMiddleIdx];

	// the first element is the median
	if (iF > iL && iF < iM)
		return s;
	// the last element is the median
	if (iL > iF && iL < iM)
		return e - 1; 

	// the middle element is the median
	return iMiddleIdx;
}

void swapElement(std::vector<int> &arr, unsigned int a, unsigned int b)
{
	int iTemp = arr[a];
	arr[a] = arr[b];
	arr[b] = iTemp;
} 

// to split the array into two parts
// arr[s, ..., iSortedPivotIdx -1; iSortedPivotIdx; iSortedPivotIdx+1, ..., e-1];
unsigned int partition(std::vector<int> &arr, unsigned int s, unsigned int e, unsigned int iPivotIdx)
{
	// swap the element at a[s] and a[iPivotIdx],
	// so we can always assume the first element a[s] is the pivot value.
	swapElement(arr, s, iPivotIdx);

	// arr[p; < p; > p]
	const int iPivot = arr[s];
	int i = s + 1;
	int j = s + 1;
	for (; j < e; ++j)
	{
		if (arr[j] < iPivot)
		{
			swapElement(arr, i, j);
			++i;
		} 
	} 
	swapElement(arr, s, i - 1); 
	return i - 1;
}

// to sort the elements [s, e-1] of array, 
// where e is the index (of the last element) + 1;
// at the first beginning, arr.size() == e - s; 
int quicksort(std::vector<int> &arr, unsigned int s, unsigned int e)
{
	if (arr.size() == 0 || arr.size() == 1) 
		return 0; 
	if (s == e - 1)
		return 0;
	if (s == e) // there was a bug here, arr[5,6];
		return 0; 
	if (s > e)
	{
		assert( false );
		return 0; 
	}

	unsigned int iPivotIdx = s;
	//iPivotIdx = choosePivotFirst(arr, s, e); 
	//iPivotIdx = choosePivotLast(arr, s, e); 
	iPivotIdx = choosePivotX(arr, s, e); 
	assert( iPivotIdx >= s && iPivotIdx < e ); 

	unsigned int iSortedPivotIdx = partition(arr, s, e, iPivotIdx);
	// the element arr[iSortedPivotIdx] is already at the right pos,
	// no need to sort it again;
	int iLeftCompareNum = quicksort(arr, s, iSortedPivotIdx); 
	int iRightCompareNum = quicksort(arr, iSortedPivotIdx + 1, e); 

	// if we only want to do the quicksort and do not need to count the 
	// number of compartion, then we do need th iLeftCompareNum and iRightCompareNum,
	// and do not need the formula below.
	// #total compare = #compare during the partition + #compare of left side + #compare of right side;
	// #compare during the partition = size of the subarray - 1; why -1, since one element is pivot,
	// and we use this one pivot to compare to the others; 
	return (e - s - 1) + iLeftCompareNum + iRightCompareNum;
}

unsigned int test1()
{
	std::vector<int> array;
	int a[] = { 7, 6, 5, 4, 3, 2, 1};
	array = std::vector<int>(a, a + sizeof(a) / sizeof(a[0]));
	
	std::vector<int> aSorted = array; // backup

	unsigned int iCount = 0;
	quicksort(array, 0, array.size());
	//std::cout << "The result is: " << iCount << std::endl;

	// to check if the array is sorted correctly during the countInversion.
	bool bOK = true;
	std::sort(aSorted.begin(), aSorted.end());
	for (int i = 0; i < array.size(); ++i)
	{
		if (array[i] != aSorted[i])
		{
			bOK = false;
			break;
		}
	} 
	std::cout << "The sorted array from my quicksort implement is " << bOK << std::endl;
	
	return iCount;
} 

int test2()
{
	std::ifstream inFile; 
	inFile.open("QuickSort.txt"); 
	
	std::vector<int> array;
	int value; 
	while (inFile >> value)
	{
		array.push_back( value );
	} 

	// to check if we have success reading in the data from file.
	std::cout << "Read in #" << array.size() << " integers, the first one :"
		<< array[0] << ", the last one :" << array[array.size() - 1] << std::endl;
		
	std::vector<int> aSorted = array; // backup

	int iCompareNum = quicksort(array, 0, array.size());
	std::cout << "#compare: " << iCompareNum << std::endl; 
		
	// to check if the array is sorted correctly during the countInversion.
	bool bOK = true;
	std::sort(aSorted.begin(), aSorted.end());
	for (int i = 0; i < array.size(); ++i)
	{
		if (array[i] != aSorted[i])
		{
			bOK = false;
			break;
		}
	} 
	std::cout << "The sorted array from my quicksort implement is " << bOK << std::endl;

	return 1;
}

int main()
{
	test1(); 
	test2();
	return 1; 
}






