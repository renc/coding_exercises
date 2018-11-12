9 11 5 7 16 1 4 2
int maxprofit(const std::vector<int> &price)
{
	// one chance, first buy and then sell
	int iMax = 0; 
	for (int i = 0; i < price.size(); ++i)
	{
		for (int j = 0; j < price.size(); ++j)
		{
			if (price[j] - price[i] > iMax)
				iMax = price[j] - price[i];
		}
	}
	return iMax; 
}

		9 11 5 7 16  1  4  2
maxPft     2 2 2 11 11 11 11 
minIdx  0    2 
int maxprofit(const std::vector<int> &price)
{ // linear 
	// one chance, first buy and then sell
	// cache the maxinum so far, 
	// replace it with new max (current price - lowest price)

	int iMax = 0;
	//int iMinPriceIdx = 0; 
	int iMinPrice = price[0];
	for (int i = 1; i < price.size(); ++i)
	{
		if (price[i] - iMinPrice > iMax)
		{
			iMax = price[i] - iMinPrice;
		}
		if (price[i] < iMinPrice)
			iMinPrice = price[i];
	}
	return iMax;
}
