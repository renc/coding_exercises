/*

to build a spin lock. 
https://en.cppreference.com/w/cpp/atomic/atomic_flag 

g++ ex8_threadlock.cpp  -o ex8_threadlock -std=c++11 -pthread 
or 
g++ ex8_threadlock.cpp  -o ex8_threadlock -std=c++11 -lpthread

https://stackoverflow.com/questions/19618926/thread-doesnt-work-with-an-error-enable-multithreading-to-use-stdthread-ope
Q. Why is -pthread necessary? Is this also needed in C++11 code?
A. Not required by stardard but implementation specific. 

[oracle@localhost cpp]$ ./ex8_threadlock
Output from thread: 2
Output from thread: 3
Output from thread: 4
Output from thread: 5
Output from thread: 6
Output from thread: 7
Output from thread: 8
Output from thread: 9
Output from thread: 0
Output from thread: 1
[oracle@localhost cpp]$ ./ex8_threadlock
Output from thread: 0
Output from thread: 1
Output from thread: 2
Output from thread: 3
Output from thread: 4
Output from thread: 5
Output from thread: 6
Output from thread: 7
Output from thread: 8
Output from thread: 9

*/
#include <iostream>
#include <atomic>
#include <thread>
#include <vector> 

// to define a SpinLock 
class SpinLock {
    private:
    std::atomic_flag m_lock = ATOMIC_FLAG_INIT;

    public:
    void lock() {
        while (m_lock.test_and_set(std::memory_order_acquire)) {
            //spin 
        }
        // test_and_set, atomically sets the flag to true and obtains its previous value 
        // if value is false, set to true, return false, no spin, acquire lock.
        // if value is true, return true, spin... 
    }
    void unlock() {
        m_lock.clear(std::memory_order_release); // release lock
    }

};

SpinLock g_Lock;
void func(int n)
{
    g_Lock.lock();
    std::cout << "Output from thread: " << n << std::endl;
    g_Lock.unlock();
}
int main()
{
    std::vector<std::thread> v;
    for (int n = 0; n < 10; ++n) {
        v.emplace_back(func, n);
    }
    for (auto &t: v) {
        t.join();
    }
    return 1;
}

