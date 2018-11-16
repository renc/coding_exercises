
1 to N,
N could be odd and even ??

1 2 3 4 5 = 15
sum = (1 + N) * N/ 2;                  
missingNum = sum - sumOf(input array); 


int is 32 bits, sum >> INT_MAX

can I put all these i into arr[i] = 1

int findMissingNum(const std::vector<int> &arr)
{
	std::vector<int> tempArray(arr.size(), 0); // memory ...
	for (int i = 0; i < N; ++i)
	{
		tempArr[arr[i]-1] = 1;
	}

	for (int i = 0; i < N; ++i)
	{
		if (tempArr[i] == 0)
			return i+1; 
	}
} XOR ???


