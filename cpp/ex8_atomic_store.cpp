/*

custom SpinLock class use the memory order, so get to check what it is.

I could just learn the "Release-Acquire ordering" first. 

"Mutual exclusion locks, such as std::mutex or atomic spinlock, 
are an example of release-acquire synchronization: 
when the lock is released by thread A and acquired by thread B, 
everything that took place in the critical section (before the release) 
in the context of thread A has to be visible to thread B (after the acquire) 
which is executing the same critical section."


example from : 
https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Acquire_ordering

https://en.cppreference.com/w/cpp/atomic/atomic/store 

2018/10/25
*/ 
#include <iostream> 
#include <thread>
#include <atomic>
#include <cassert>
#include <string>

std::atomic<std::string *> ptr;
std::atomic<std::string > str;
int data;

void producer()
{
    std::cout << "Producer start.\n";
    std::string *p = new std::string("Hello");
    data = 42;
    ptr.store(p, std::memory_order_relaxed); // atomically replaces the curr value with desired.
    std::cout << "Producer end.\n";
}

void consumer()
{
    std::cout << "Consumer start.\n";
    std::string *p2;
    while( !(p2 = ptr.load(std::memory_order_acquire)) ) 
    {} // spin

    assert(*p2  == "Hello");
    assert(data == 42); 
    std::cout << "Consumer end.\n";
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
}

