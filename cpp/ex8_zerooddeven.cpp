/*
https://leetcode.com/problems/print-zero-even-odd/

*/
#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector> 


#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace fail {
    class ZeroEvenOdd {
    private:
        int n;
        std::atomic<int> iCurNum = 1;
        std::mutex mtx; // using this to lock the iCurNum.

        bool bPrintZero = false;
        std::mutex mtxZero;
        std::condition_variable cvZero;

        int iNext = 1; // 1: odd, 2: even
        std::mutex mtxNext;
        std::condition_variable cvNext;
    public:
        ZeroEvenOdd(int n) {
            this->n = n;
            iCurNum = 1;
        }

        // printNumber(x) outputs "x", where x is an integer.
        void zero(std::function<void(int)> printNumber) {
            while (true) {
                std::unique_lock<std::mutex> lk(mtxZero);
                cvZero.wait(lk, [&]() {return bPrintZero; });

                if (iCurNum > n) {
                    bPrintZero = false;
                    break;
                }
                printNumber(0);
                bPrintZero = false;
                lk.unlock();
                cvZero.notify_all();
            }
        }

        void even(std::function<void(int)> printNumber) {
            while (true) {
                std::unique_lock<std::mutex> lkNext(mtxNext);
                cvNext.wait(lkNext, [&]() { return iNext == 2; });
                //std::cerr << "e" << iCurNum << ",";
                if (iCurNum > n) break;
                if (iCurNum % 2 == 0 && bPrintZero == false) {
                    bPrintZero = true;
                    cvZero.notify_all();

                    std::unique_lock<std::mutex> lk(mtxZero);
                    cvZero.wait(lk, [&]() {return bPrintZero == false; });

                    printNumber(iCurNum);
                    ++iCurNum;
                }
                iNext = 1;
                lkNext.unlock();
                cvNext.notify_all();
            }
        }

        void odd(std::function<void(int)> printNumber) {
            while (true) {
                std::unique_lock<std::mutex> lkNext(mtxNext);
                cvNext.wait(lkNext, [&]() { return iNext == 1; });
                //std::cerr << "o" << iCurNum << ",";
                if (iCurNum > n) break;
                if (iCurNum % 2 == 1 && bPrintZero == false) {
                    bPrintZero = true;
                    cvZero.notify_all();

                    std::unique_lock<std::mutex> lk(mtxZero);
                    cvZero.wait(lk, [&]() {return bPrintZero == false; });

                    printNumber(iCurNum);
                    ++iCurNum;
                }
                iNext = 2;
                lkNext.unlock();
                cvNext.notify_all();
            }
        }
    };
    // fail: sometime success, sometime hang at the begining. 
};
// state machine, only one state, one mtx. 
class ZeroEvenOdd {
private:
    int n;
    std::atomic<int> iCurNum = 1;
     
    int iNext = 0; // 1: odd, 2: even
    std::mutex mtxNext;
    std::condition_variable cvNext;
public:
    ZeroEvenOdd(int n) {
        this->n = n;
        iCurNum = 1;
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(std::function<void(int)> printNumber) {
        while (true) {
            std::unique_lock<std::mutex> lkNext(mtxNext);
            cvNext.wait(lkNext, [&]() { return iNext == 0; });

            if (iCurNum > n) { 
                break;
            }
            printNumber(0);
            if (iCurNum % 2 == 1) iNext = 1;
            if (iCurNum % 2 == 0) iNext = 2;
            lkNext.unlock();
            cvNext.notify_all();
        }
    }

    void even(std::function<void(int)> printNumber) {
        while (true) {
            std::unique_lock<std::mutex> lkNext(mtxNext);
            cvNext.wait(lkNext, [&]() { return iNext == 2; });
            if (iCurNum > n) break;
            
            printNumber(iCurNum);
            ++iCurNum;
             
            iNext = 0;
            lkNext.unlock();
            cvNext.notify_all();
        }
    }

    void odd(std::function<void(int)> printNumber) {
        while (true) {
            std::unique_lock<std::mutex> lkNext(mtxNext);
            cvNext.wait(lkNext, [&]() { return iNext == 1; });
            
            printNumber(iCurNum);
            ++iCurNum; 

            iNext = 0;
            lkNext.unlock();
            cvNext.notify_all();
        }
    }
};
int main()
{
    std::function<void (int) > printNum = [](int n) { std::cout << n;  };

    ZeroEvenOdd zeo(20);
    std::thread t0(&ZeroEvenOdd::zero, &zeo, printNum);
    std::thread t1(&ZeroEvenOdd::even, &zeo, printNum);
    std::thread t2(&ZeroEvenOdd::odd, &zeo, printNum);
    t0.join();
    t1.join();
    t2.join();
    return 1;
}

