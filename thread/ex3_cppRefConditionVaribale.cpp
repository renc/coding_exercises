#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <vector> 
#include <string> 

// http://en.cppreference.com/w/cpp/thread/condition_variable
// 两个线程都对同一个cv操作，同一个cv又是操作同一个mutex.
// 这个例子有意思的地方是两个threads, main thread, worker thread, 
// 相互交流, 显示worker thread等main thread, 然后到main thread 等worker thread.

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
// 首先假如上面先lk.lock(); 单单使用cv.wait(lk); 表示进入等待suspend current thread, 等待被唤醒, 注意这里面会自动调用lk.unlock();
//
// 然后 上面 cv.wait(lk, [] { return ready; }); === while (!ready) { cv.wait(lk); };
// 因为{}里面的内容一致, 所以也是block current thread and wait for signal, 也是自动调用lk.unlock(); 比较更进一步的是, 
// 当被唤醒的时候, 会先lk.lock()!!! 然后 查while(!ready),
// 假如ready==false, 继续cv.wait(lk) 里面会自动lk.unlock();继续等(...等啊等啊等, 等到被唤醒的时候, 就重复上面那一步). 
// 假如ready==true了, 就不用继续等了，真的醒了, 注意这时候lk still locked !!!  这也解释了为什么上面函数最后cv.notify_one()之前需要unlock.

int main()
{
	std::thread worker(worker_thread);

	data = "Example data";
	// send data to the worker thread
	{
		std::this_thread::sleep_for(std::chrono::seconds(5)); // renc, 原来例子是没有这句的我加了看cv.wait()有没有烧cpu
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