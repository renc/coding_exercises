#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <queue>
#include <atomic>

// the most simple and error one
std::queue<int> g_q; // queue 
std::atomic_int sumProduct = 0; 
std::atomic_int sumConsume = 0;
void feed(int n)
{
	for (int i = 0; i < n; ++i) 
	{ 
		{ 
			g_q.push(i + 1000);
			sumProduct += i + 1000;
		}  
	}
}

void consume(int n)
{
	for (int i = 0; i < n; ++i)
	{ 
		int a = g_q.front();
		g_q.pop();
		std::cout << a << ", ";
		sumConsume += a;
	}
}

int main()
{
	//for (int i = 0; i < 9999; ++i)  even crash with this.
	{
		std::thread t1(feed, 200);//thread object
		std::thread t2(consume, 100);
		std::thread t3(consume, 100);
		t1.join();
		t2.join(); t3.join();

		if (sumProduct != sumConsume)
			std::cout << "\n" << "sum: prodect " << sumProduct << " ==?== consume " << sumConsume << ".\n";
	}
	getc(stdin);
}