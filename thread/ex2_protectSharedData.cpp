#include <thread>
#include <iostream>
#include <vector> 

// https://baptiste-wicht.com/posts/2012/03/cp11-concurrency-tutorial-part-2-protect-shared-data.html
struct Counter
{
	Counter() : value(0) {}

	void increment() {
		++value;
	}
	int value; 
};


int main()
{
	for (int tI = 0; tI < 100; ++tI) {
		Counter counter;
		std::vector<std::thread> threads;

		for (int i = 0; i < 10; ++i) {
			threads.push_back(std::thread(
				[&counter]() {
				for (int j = 0; j < 100; ++j) {
					counter.increment();
				}
			})
			);
		}

		for (auto &t : threads) {
			t.join();
		}
		std::cout << "Counter value: " << counter.value << std::endl;
	}
	return 0; 
}