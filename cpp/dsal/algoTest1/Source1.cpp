
// 52 cards
// [1,N] giveNumber(int N); generate random number

void shuffle(std::vector<int> &arr)
{
	// swap(i, 51-i) ?
	// how to select the number i?
	// using random number 

	for (int i = 0; i < 26; ++i)
	{
		int iIdx = giveNumber(52) - 1;          // 52 choise 
		std::swap(arr[iIdx], arr[51 - iIdx]);    
	}
	// pow(52, 26)?

	// target is 52!= 52*51*...*2*1. .
	for (int i = 51; i > 0; --i)
	{
		int iIdx = giveNumber(i + 1) - 1;
		std::swap(arr[iIdx], arr[i]);
	}
	
	// linear algo is not good enough
	// looking to O(lg 52) ?
















}