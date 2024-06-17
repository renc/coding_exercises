/**
to measure the latency, need a clock to add timestamp
- here only consider msgs using same clock type (eg. RTC, HRT, ...) at a macine 
- not consider synchonize cocks between machines (exch and our server)

*/

#include <sys/time.h>   // struct timeval, gettimeofday
#include <time.h>       // struct timespec, clock_gettime(clock id, timespec*)
#include <unistd.h>     // sleep
#include <cstdint> 
#include <iostream>
#include <source_location> 

constexpr uint64_t iMillion = 1'000'000;
constexpr uint64_t iBillion = 1'000'000'000;

void test_localtime()
{
    std::printf("%s\n", std::source_location::current().function_name());
    time_t tm_now;
    time(&tm_now); // or tm_now = time(NUL); 

    std::printf("time_t now=%ld seconds since epoc\n", tm_now);

    struct tm *pLocalTM = localtime(&tm_now);
    std::printf("localtime of now: %04d-%02d-%02d.%02d:%02d:%02d\n", pLocalTM->tm_year+1900, pLocalTM->tm_mon+1, pLocalTM->tm_mday,
        pLocalTM->tm_hour, pLocalTM->tm_min, pLocalTM->tm_sec); // up to 1 seconds
}

void test_gettimeofday()
{
    std::printf("%s\n", std::source_location::current().function_name());
    struct  timeval startT, endT;
    gettimeofday(&startT, NULL);
    for (auto i = 0; i < iMillion; ++i) { gettimeofday(&endT, NULL); }
    uint64_t nanosecond = (endT.tv_sec - startT.tv_sec) * iBillion + (endT.tv_usec - startT.tv_usec) * 1000; // 1 usec 
    double seconds = (double)nanosecond / iBillion;
    std::printf("%ld times of gettimeofday takes %f seconds, 1 call takes %ld ns\n", iMillion, seconds, nanosecond/iMillion);

    //struct tm* pLocalTM = localtime(&endT.tv_sec); // simiar to above test_localtime(); // seconds since epoc, 
    // but we also has endT.tv_usec now 

    // find this gettimeofday() does not go into kernel cal, waht is vDso ? how do it work    
}

void test_clock_gettime()
{   
    std::printf("%s\n", std::source_location::current().function_name());
    struct  timespec ts;
    clockid_t ckid = CLOCK_MONOTONIC; // vs CLOCK_REAL_TIME which maybe affected by time adjust (say user change the system time using date command);
    // https://linux.die.net/man/2/clock_gettime
    //                              manually change clock(date cmd);           adjtime;              NTP
    // CLOCK_REAL_TIME        :     affected                                   affected         affected
    // CLOCK_MONOTONIC        :     not affected                               affected         affected
    // CLOCK_MONOTONIC_RAW    :     not affected                               not affected     not affected       <-- raw hardware-based time


    // check resolution
    if (clock_getres(ckid, &ts) == -1) 
        std::cout << "clock_getres() error\n";
    printf("resolution: %10ld.%09ld\n", (long)ts.tv_sec, ts.tv_nsec); // 1ns !!

    struct timespec startT, endT;
    clock_gettime(ckid, &startT);
    for (auto i = 0; i < iMillion; ++i) clock_gettime(ckid, &endT);
    uint64_t nanosecond = (endT.tv_sec - startT.tv_sec) * iBillion + (endT.tv_nsec - startT.tv_nsec);
    double seconds = (double) nanosecond / iBillion;
    std::printf("%ld times of clock_gettime takes %f seconds, 1 call takes %ld ns\n", iMillion, seconds, nanosecond/iMillion);
}

uint64_t get_tsc()
{
    uint64_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return (hi << 32) | lo;
}
uint64_t get_tscp()
{
    uint64_t lo, hi;
    __asm__ __volatile__("rdtscp" : "=a"(lo), "=d"(hi));
    return (hi << 32) | lo;
}
void test_tsc()
{
    // https://en.wikipedia.org/wiki/Time_Stamp_Counter   
    const uint64_t cpuFreq = 2194.917 * 1000 * 1000; // cat /proc/cpuinfo   or lscpu     get the cpu MHz 
    const double   nsPerCycle = (double)1'000'000'000 / cpuFreq; // convert clock cycle to nanosecond , lscpu, cat /proc/cpuinfo

    uint64_t beg_tsc, end_tsc;
    uint64_t loop, sum;
    printf("--rdtsc--");
    beg_tsc = get_tsc();
    for (auto i=0; i < iMillion; ++i) end_tsc = get_tsc();
    sum = end_tsc - beg_tsc; 
    printf("1 call takes %.2f cycle, 1 call takes %.2f ns\n", (double)sum/iMillion,  (double)sum/iMillion * nsPerCycle);
    sleep(1); // 1s
    
    printf("--rdtscp--");
    beg_tsc = get_tsc();
    for (auto i=0; i < iMillion; ++i) end_tsc = get_tsc();
    sum = end_tsc - beg_tsc; 
    printf("1 call takes %.2f cycle, 1 call takes %.2f ns\n", (double)sum/iMillion,  (double)sum/iMillion * nsPerCycle); // rdtscp is a bit slower than rdtsc, why ?    

    // 
    printf("CLOCKS / sec = ", CLOCKS_PER_SEC); // empty value, not sure why
}

void test_hpet()
{
    // https://en.wikipedia.org/wiki/High_Precision_Event_Timer
}
int main()
{
    test_localtime();
    test_gettimeofday();
    test_clock_gettime();
    test_tsc();
    return 0;
}

/*
[renc@renc-laptop cpp]$ g++ -Wall -g --std=c++20 ey2_time_measure.cpp
[renc@renc-laptop cpp]$ time ./a.out 
void test_localtime()
time_t now=1718633930 seconds since epoc
localtime of now: 2024-06-17.22:18:50
void test_gettimeofday()
1000000 times of gettimeofday takes 0.039320 seconds, 1 call takes 39 ns
void test_clock_gettime()
resolution:          0.000000001
1000000 times of clock_gettime takes 0.045255 seconds, 1 call takes 45 ns
--rdtsc--1 call takes 28.21 cycle, 1 call takes 12.85 ns
--rdtscp--1 call takes 20.48 cycle, 1 call takes 9.33 ns
CLOCKS / sec = 
real    0m1.114s
user    0m0.083s
sys     0m0.020s
*/