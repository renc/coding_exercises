#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <queue>

std::queue<int> g_q; // queue 
std::mutex cv_m;// to synchronize to std::cout
void feed(int n)
{
	for (int i = 0; i < n; ++i) 
	{
		// !!if this sleep is added, than crash, since the consumer get nothing from empty queue.
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));// CRASH
		{ 
			g_q.push(i + 1000);
		}  
	}
}
void consume(int n)
{
	for (int i = 0; i < n; ++i)
	{ 
		std::lock_guard<std::mutex> lk(cv_m);
		int a = g_q.front();
		g_q.pop();
		std::cout << a << ", ";
	}
}

int main()
{
	std::thread t1(feed, 200);//thread object
	std::thread t2(consume, 100);
	std::thread t3(consume, 100);
	t1.join();
	t2.join(); t3.join();
	getc(stdin);

}