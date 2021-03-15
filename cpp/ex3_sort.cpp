#include <stdio.h> // for print
#include <stdlib.h> // for srand, rand
#include <time.h> // for time
#include <assert.h> // for assert
#include <vector> // for std::vector
#include <algorithm> // for std::swap
#include <iterator> // for std::back_inserter 

namespace rencSort
{
	// my implement of sorting algorithems.

	// CLRS, chapter 2. 
	void insertSort(std::vector<int> &a)
	{
		for (unsigned int i = 1; i < a.size(); ++i)
		{
			// insert a[i] into before.
			const int key = a[i];
			int j = i - 1;
			for (; j >= 0; --j)
			{
				//[0] [1] [2] [j] [i]
				if (a[j] > key)
				{
					a[j+1] = a[j]; // move a[j] backward.
				} 
				else
					break;
			} 
			// j == -1 or a[j] <= key 
			a[j+1] = key; 
		} 
	} 

	void merge(const std::vector<int> &leftA, const std::vector<int> &rightA, 
		std::vector<int>&a)
	{
		unsigned int lp = 0, rp = 0;  // left pos, right pos.
		for (; lp < leftA.size(); )
		{
			if (leftA[lp] <= rightA[rp])
			{
				a.push_back(leftA[lp]);
				++lp;
			}
			else
			{
				// >
				a.push_back(rightA[rp]);
				++rp; 
				if (rp >= rightA.size())
				{
					for (; lp < leftA.size(); ++lp)
						a.push_back(leftA[lp]);
					break;
				} 
			}
		}
		if (rp < rightA.size())
		{
			for (; rp < rightA.size(); ++rp)
				a.push_back(rightA[rp]);
		}
	} 
	void mergeSort(std::vector<int> &a)
	{
		if (a.size() == 1)
			return; 

		std::vector<int> leftA;
		std::vector<int> rightA;
		unsigned int mid = a.size() / 2;
		for (unsigned int i = 0; i < mid; ++i)
			leftA.push_back(a[i]);
		for (unsigned int i = mid; i < a.size(); ++i)
			rightA.push_back(a[i]);
		mergeSort(leftA);
		mergeSort(rightA);

		a.clear();
		merge(leftA, rightA, a);// implement by my self.	 
	} 

	void mergeSort2(std::vector<int> &a)
	{
		if (a.size() == 1)
			return; 

		unsigned int mid = a.size() / 2;
		/*
		std::vector<int> leftA(mid);
		std::vector<int> rightA(a.size() - mid); // need to set the size ourself
		std::copy(a.begin(), a.begin() + mid, leftA.begin()); 
		std::copy(a.begin() + mid, a.end(), rightA.begin()); 
		*/
		std::vector<int> leftA;
		std::vector<int> rightA;
		std::copy(a.begin(), a.begin() + mid, std::back_inserter(leftA)); 
		std::copy(a.begin() + mid, a.end(), std::back_inserter(rightA)); 
		// which create a std::back_insert_iterator<std::vector<int> >(leftA); 
		// that will invoke push_back() whenever it need to.


		mergeSort2(leftA);
		mergeSort2(rightA);

		a.clear();
		std::merge(leftA.begin(), leftA.end(), rightA.begin(), rightA.end(),std::back_inserter(a));
	} 

	// CLRS, chapter6. heapsort
	//0
	//1    2 
	//3 4  5 6 
	int parentIdx(int idx) { return (int)floor((idx - 1)/2); };
	int leftIdx(int idx) { return idx * 2 + 1; };
	int rightIdx(int idx) { return idx * 2 + 2; }; 
	void maxHeapify(std::vector<int> &a, int heapSize, int idx)
	{
		int lc = leftIdx(idx);
		int rc = rightIdx(idx);
		int largest = idx;

		if (lc < heapSize && a[idx] < a[lc])
			largest = lc;
		else
			largest = idx;

		if (rc < heapSize && a[largest] < a[rc])
			largest = rc;

		if (largest != idx)
		{
			std::swap(a[idx], a[largest]);
			maxHeapify(a, heapSize, largest); 
		} 
	}
	int buildMaxHeap(std::vector<int> &a)
	{
		int heapSize = a.size();
		for (int i = heapSize/2; i >= 0; --i)
			maxHeapify(a, heapSize, i);
		return heapSize; 
	} 
	void heapSort(std::vector<int> &a)
	{
		int heapSize = buildMaxHeap(a); // O(n). 

		for (int i = a.size()-1; i >= 0; --i)
		{
			std::swap(a[0], a[i]);
			--heapSize;
			maxHeapify(a, heapSize, 0); 
		} 
	} 
}; // end of rencSort.

namespace mergeSort3
{
	// renc: this implement is very much similar to CLRS version 2 chapter2.
	// http://www.personal.kent.edu/~rmuhamma/Algorithms/MyAlgorithms/Sorting/mergeSort.htm
	void merge(int numbers[], int temp[], int left, int mid, int right)
	{
		int i, left_end, num_elements, tmp_pos;

		left_end = mid - 1;
		tmp_pos = left;

		num_elements = right - left + 1;

		while ((left <= left_end) && (mid <= right))
		{
			if (numbers[left] <= numbers[mid])
			{
				temp[tmp_pos] = numbers[left];
				tmp_pos = tmp_pos + 1;
				left = left +1;
			}
			else
			{
				temp[tmp_pos] = numbers[mid];
				tmp_pos = tmp_pos + 1;
				mid = mid + 1;
			}
		}

		while (left <= left_end)
		{
			temp[tmp_pos] = numbers[left];
			left = left + 1;
			tmp_pos = tmp_pos + 1;
		}
		while (mid <= right)
		{
			temp[tmp_pos] = numbers[mid];
			mid = mid + 1;
			tmp_pos = tmp_pos + 1;
		}

		for (i = 0; i <= num_elements; i++)
		{
			numbers[right] = temp[right];
			right = right - 1;
		}
	} 
	void m_sort(int numbers[], int temp[], int left, int right)
	{
		int mid;

		if (right > left)
		{
			mid = (right + left) / 2;
			m_sort(numbers, temp, left, mid);
			m_sort(numbers, temp, mid+1, right);

			merge(numbers, temp, left, mid+1, right);
		}
	}
	void mergeSort(int numbers[],  int temp[], int array_size)
	{
		m_sort(numbers, temp, 0, array_size - 1);
	}
} // end of namespace mergeSort3. 


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

bool testSortResult(const std::vector<int> &a, std::vector<int> &oriArr)
{
	std::sort(oriArr.begin(), oriArr.end());// sort the original array using std func. 
	if (a.size() != oriArr.size())
		return false;

	bool bSucceed = true;
	for (int i = 0; i < a.size(); ++i)
	{
		if (a[i] != oriArr[i])
		{
			bSucceed = false;
			break;
		}
	}
	return bSucceed;
}

int main(char **, int)
{
	/*int b[] = {6,6,3,8,7,4,5,9,0,1,6,2 }; 
	std::vector<int> a;
	a.insert(a.end(), &b[0], &b[sizeof(b) / sizeof(int)] );	
	*/
	
	srand( time(0) ); 
	std::vector<int> a(1000);
	for (int i = 0; i < a.size(); ++i)
	{
		a[i] = rand(); 
	} 

	std::vector<int> aBackup = a;
	printf("Init array: \n");
	for (int i = 0; i < a.size(); ++i)
	{
		printf("%d, ", a[i]); 
	}
	printf("\n");
	
	//rencSort::insertSort(a);
	//rencSort::mergeSort(a);
	//rencSort::mergeSort2(a);
	rencSort::heapSort(a); 
	//myqsort(a, 0, a.size()); 
	
	printf("Final array: \n");
	for (int i = 0; i < a.size(); ++i)
	{
		printf("%d, ", a[i]); 
	}
	printf("\n");
	
	// to check if the result is right or wrong
	bool bSucceed = testSortResult(a, aBackup); 
	if (bSucceed)
		printf("sort - Succeed!\n");
	else
		printf("sort - Fail!\n");
	
	scanf("%d");
	system("pause"); // windows
	return 1; 
}