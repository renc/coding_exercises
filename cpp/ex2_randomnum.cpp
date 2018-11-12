#include <stdlib.h> // for NULL
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
int main()
{
	// without setting seed, the num sequence will be same 
	int iA0 = rand(); iA0;
	for (int i = 0; i < 10; ++i)
		std::cout << rand() << std::endl;

	std::random_device rd{};
	std::mt19937 gen{ rd() };

	// values near the mean are the most likely
	// standard deviation affects the dispersion of generated values from the mean
	std::normal_distribution<> d{ 5,2 };
	double n0 = d(gen);

	std::map<int, int> hist{};
	for (int n = 0; n<10000; ++n) {
		++hist[std::round(d(gen))];
	}
	for (auto p : hist) {
		std::cout << std::setw(2)
			<< p.first << ' ' << std::string(p.second / 200, '*') << '\n';
	}
}