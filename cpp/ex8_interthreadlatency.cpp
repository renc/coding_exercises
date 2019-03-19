
// g++ ex8_interthreadlatency.cpp -o ./build/ex8_interthreadlatency -std=c++11 -pthread -g
// 
// https://mechanical-sympathy.blogspot.com/2011/08/inter-thread-latency.html
// Message rates between threads are fundamentally determined by the latency of memory exchange between CPU
// cores. The minimum unit of transfer will be a cache line exchanged via shared caches or socket interconnects.  

#include <thread>
#include <atomic> 
#include <ctime>
#include <cstdio>
#include <iostream>

const uint64_t ITERATIONS = 50* 1000 * 1000;//500 * 1000 * 1000;
std::atomic_uint_fast64_t s1 {0L}; 
std::atomic_uint_fast64_t s2 {0L}; 

void run()
{
    uint64_t value = s2;
    while (true) {
        while (value == s1) {
            // busy spin
            //printf("t2, %lld\n", value);
        }
        value = ++s2;//s2.fetch_add(1); 
    }
}

int main()
{
    std::thread t(run);

    timespec ts_start;
    timespec ts_finish;
    clock_gettime(CLOCK_MONOTONIC, &ts_start);

    uint64_t value = s1;
    while (s1 < ITERATIONS)
    {
        while (s2 != value) {
            // busy spin 
            //printf("t1, %lld\n", value);
        }
        //value = s1.fetch_add(1);
        value = ++s1; 
    }

    clock_gettime(CLOCK_MONOTONIC, &ts_finish);

    uint64_t start = (ts_start.tv_sec * 1000000000LL) + ts_start.tv_nsec;
    uint64_t finish = (ts_finish.tv_sec * 1000000000LL) + ts_finish.tv_nsec;
    uint64_t duration = finish - start;

    printf("duration = %lld\n", duration);
    printf("ns per op = %lld\n", (duration / (ITERATIONS * 2)));
    printf("op/sec = %lld\n",  
        ((ITERATIONS * 2L * 1000L * 1000L * 1000L) / duration));
    std::cout << "s1 = " << s1 << ", s2 = " << s2 << std::endl;;

    return 0;
}



