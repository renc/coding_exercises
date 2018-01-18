#include <thread>
#include <iostream>
#include <vector> 

// https://baptiste-wicht.com/posts/2012/03/cpp11-concurrency-part1-start-threads.html
void hello() {
	std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}

int main()
{
	std::vector<std::thread> threads;
	
	for (int i = 0; i < 5; ++i) {
		threads.push_back(std::thread(hello));
	}

	for (auto &t : threads) {
		t.join();
	}
	return 0; 
}