/* 
	Counting inversions in an array. 

	canjiang ren at 2014/5/22 
*/ 

#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <algorithm> // std::sort

unsigned int merge(std::vector<int> &arrL, std::vector<int> &arrR, std::vector<int> &arr)
{
	int iCount = 0;
	int i = 0, j = 0; 
	for (; i < arrL.size() && j < arrR.size(); )
	{
		if (arrL[i] <= arrR[j])
		{
			arr.push_back( arrL[i] );
			++i;
		} 
		else
		{
			arr.push_back( arrR[j] );
			++j;

			iCount += arrL.size() - i; 
		} 
	}

	if (i == arrL.size())
	{
		// the whole arrL has been added to the arr[] result already, 
		// now we append the remain of arrR to the end of arr[];
		for (int k = j; k < arrR.size(); ++k)
			arr.push_back( arrR[k] );
	} 
	if (j == arrR.size())
	{
		for (int k = i; k < arrL.size(); ++k)
			arr.push_back( arrL[k] );
	}
	return iCount;
}

unsigned int countInversion(const std::vector<int> &arr, unsigned int s, unsigned int e, 
							std::vector<int> &aSorted)
{
	if (s == e) 
	{
		assert(false); // should be e > s, at least s + 1 == e. if u down here, u got a bug.
		return 0;
	}
	if (s + 1 == e) 
	{
		aSorted = std::vector<int>(&arr[s], &arr[s]+1); 
		return 0; 
	}

	// break to two parts
	std::vector<int> aSortedL, aSortedR;
	const unsigned int iMid = (s + e) / 2;
	unsigned int iCountL = countInversion(arr, s, iMid, aSortedL);
	unsigned int iCountR = countInversion(arr, iMid, e, aSortedR);
	
	unsigned int iCountMerge = merge(aSortedL, aSortedR, aSorted);
	return iCountL + iCountR + iCountMerge; 
}

unsigned int countInversionBruteForce(const std::vector<int> &arr)
 {
	 unsigned int iCount = 0;
	 
	 for (int i = 0, n = arr.size(); i < n - 1; ++i)
		 for (int j = i+1; j < n; ++j)
			 if (arr[i] > arr[j])
				 ++iCount;

	 return iCount;
 }

unsigned int test1()
{
	std::vector<int> array;
	int a[] = { 7, 6, 5, 4, 3, 2, 1};
	array = std::vector<int>(a, a + sizeof(a) / sizeof(a[0]));
	
	std::vector<int> aSorted;
	unsigned int iCount = countInversion(array, 0, array.size(), aSorted);
	std::cout << "The result is: " << iCount << std::endl;

	// to check if the array is sorted correctly during the countInversion.
	bool bOK = true;
	std::sort(array.begin(), array.end());
	for (int i = 0; i < array.size(); ++i)
	{
		if (array[i] != aSorted[i])
		{
			bOK = false;
			break;
		}
	} 
	std::cout << "The sorted array from divide-conquer is " << bOK << std::endl;
	
	return iCount;
} 

unsigned int test2()
{
	std::ifstream inFile; 
	inFile.open("IntegerArray.txt"); 
	
	std::vector<int> array;
	int value; 
	while (inFile >> value)
	{
		array.push_back( value );
	} 

	std::vector<int> aSorted;
	unsigned int iCount = countInversion(array, 0, array.size(), aSorted);
	std::cout << "The result is: " << iCount << std::endl;

	
	//unsigned int iCountBruteForce = 0;
	//iCountBruteForce = countInversionBruteForce(array); // 2407905288
	//std::cout << "The result is: " << iCountBruteForce << std::endl;
	
	// to check if the array is sorted correctly during the countInversion.
	bool bOK = true;
	std::sort(array.begin(), array.end());
	for (int i = 0; i < array.size(); ++i)
	{
		if (array[i] != aSorted[i])
		{
			bOK = false;
			break;
		}
	} 
	std::cout << "The sorted array from divide-conquer is " << bOK << std::endl;
	
	return iCount; 
} 

int main()
{
	test1();
	test2(); 
	return 1; 
}