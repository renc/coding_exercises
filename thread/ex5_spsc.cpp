#include <iostream>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <queue>

std::queue<int> g_q; // queue
std::condition_variable cv;
std::mutex cv_m;// to synchronize to std::cout, for the condition variable
				// ���Կ�����provider
void feed(int n)
{
	for (int i = 0; i < n; ++i) // ���Կ�����һ��timer ÿ2s��һ������
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));
		{
			std::lock_guard<std::mutex> lk(cv_m);
			g_q.push(i + 1000);
		} //���{}��Ϊ����lock_guard�ͷ�lock
		cv.notify_all(); // ֪ͨ��wait��threads
	}
}
void consume(int n)
{
	for (int i = 0; i < n; ++i)
	{
		// wait until the queue is not empty
		std::unique_lock<std::mutex> lk(cv_m); //������unique_lock,������ͬһ��cv_m; 
		cv.wait(lk, [] { return !g_q.empty(); });// after the wait, we own the lock.
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