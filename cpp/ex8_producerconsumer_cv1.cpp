/*

ex8_producerconsumer_cv1.cpp first, simple  
ex8_producerconsumer_cv2.cpp is a refactor of that version. 

Producer Consumer with std::queue 
https://gist.github.com/iikuy/8115191 

g++ filename.cpp -o exename -std=c++11 -pthread 

*/ 

#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <queue>

std::queue<int> g_q; // queue
std::condition_variable cv;
std::mutex cv_m;// to synchronize to std::cout, for the condition variable
// 可以看作是provider
void feed(int n) 
{
	for (int i = 0; i < n; ++i) // 可以看作是一个timer 每2s发一个东西
	{
		std::this_thread::sleep_for(std::chrono::seconds(2)); //停顿可以造成消费者从队列取东西为空
		{
			std::lock_guard<std::mutex> lk(cv_m);
			g_q.push(i + 1000);
		} //这个{}是为了让lock_guard释放lock
		cv.notify_all(); // 通知在wait的threads
	}
}
void consume(int n) 
{
	for (int i = 0; i < n; ++i)
	{
		// wait until the queue is not empty
		std::unique_lock<std::mutex> lk(cv_m); //必须是unique_lock,而且是同一个cv_m; 
		cv.wait(lk, [] { return !g_q.empty(); }); 
		// == while (g_q.empty()) { cv.wait(lk); } // after the wait, we own the lock.
		int a = g_q.front(); 
		g_q.pop();
		std::cout << a << std::endl;
	}
}

int main()
{
	std::thread t1(feed, 99);//thread object
	std::thread t2(consume, 99);
	t1.join();
	t2.join();
	getc(stdin);
}
