#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <vector> 
#include <string> 

// http://en.cppreference.com/w/cpp/thread/condition_variable
// �����̶߳���ͬһ��cv������ͬһ��cv���ǲ���ͬһ��mutex.
// �����������˼�ĵط�������threads, main thread, worker thread, 
// �໥����, ��ʾworker thread��main thread, Ȼ��main thread ��worker thread.

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void worker_thread()
{
	// Wait until main() sends data
	std::unique_lock<std::mutex> lk(m);
	cv.wait(lk, [] {return ready; });

	// after the wait, we own the lock.
	std::cout << "Worker thread is processing data\n";
	data += " after processing";

	// Send data back to main()
	processed = true;
	std::cout << "Worker thread signals data processing completed\n";

	// Manual unlocking is done before notifying, to avoid waking up
	// the waiting thread only to block again (see notify_one for details)
	lk.unlock();
	cv.notify_one();
}
// ���ȼ���������lk.lock(); ����ʹ��cv.wait(lk); ��ʾ����ȴ�suspend current thread, �ȴ�������, ע����������Զ�����lk.unlock();
//
// Ȼ�� ���� cv.wait(lk, [] { return ready; }); === while (!ready) { cv.wait(lk); };
// ��Ϊ{}���������һ��, ����Ҳ��block current thread and wait for signal, Ҳ���Զ�����lk.unlock(); �Ƚϸ���һ������, 
// �������ѵ�ʱ��, ����lk.lock()!!! Ȼ�� ��while(!ready),
// ����ready==false, ����cv.wait(lk) ������Զ�lk.unlock();������(...�Ȱ��Ȱ���, �ȵ������ѵ�ʱ��, ���ظ�������һ��). 
// ����ready==true��, �Ͳ��ü������ˣ��������, ע����ʱ��lk still locked !!!  ��Ҳ������Ϊʲô���溯�����cv.notify_one()֮ǰ��Ҫunlock.

int main()
{
	std::thread worker(worker_thread);

	data = "Example data";
	// send data to the worker thread
	{
		std::this_thread::sleep_for(std::chrono::seconds(5)); // renc, ԭ��������û�������Ҽ��˿�cv.wait()��û����cpu
		std::lock_guard<std::mutex> lk(m);
		ready = true;
		std::cout << "main() signals data ready for processing\n";
	}
	cv.notify_one();

	// wait for the worker
	{
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [] {return processed; });
	}
	std::cout << "Back in main(), data = " << data << '\n';

	worker.join();
	getc(stdin);
} 