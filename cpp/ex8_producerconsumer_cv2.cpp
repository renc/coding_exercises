/*

ex8_producerconsumer_cv1.cpp first, simple  
ex8_producerconsumer_cv2.cpp is a refactor of that version. 

Producer Consumer with std::queue 
https://gist.github.com/iikuy/8115191 

g++ filename.cpp -o exename -std=c++11 -pthread -g
-g for debug

*/ 
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream> 
#include <vector> 

template <typename T>
class QueueCV {
public:
    void push(const T& n)
    {
        {
            std::lock_guard<std::mutex> lk(m_mtx);
            m_queue.push(n);
        } // Important, unlock before notify, otherwise, 
        // the waiting thread wait up before the mutx is unlock.
        m_cv.notify_all();
        // vs notify_one() ? multiply customers.
    }
    // T pop() // i use this function signature before, but change to
    // Using a reference parameter to receive the result is used to transfer ownership
    //  out of the queue in order to avoid the exception safety issues 
    // of returning data by-value: if the copy constructor of a by-value return throws, 
    // then the data has been removed from the queue, but is lost, 
    // whereas with this approach, the potentially problematic copy is performed 
    // prior to modifying the queue (see Herb Sutter's Guru Of The Week #8 
    // for a discussion of the issues). 
    void pop(T &destination)
    {
        std::unique_lock<std::mutex> lk(m_mtx);
        m_cv.wait(lk, [this](){ return m_queue.empty() == false;});
        // same as
        // while ( ! (q.empty() == false) ) 
        //     m_cv.wait(lk);
        //
        // condition variables can be subject to spurious wake-ups, 
        // so it is important to check the actual condition being waited for 
        // when the call to wait returns.
        // 
        // if we are here, q is not empty and mtx is locked.
        
        //T iRet = m_queue.front();
        destination = m_queue.front();
        m_queue.pop();   
        //return iRet;     
        // this could be blocking (not no-blocking)
    }
    T& front() {
        // why return type is reference ? 
        // https://en.cppreference.com/w/cpp/container/queue/front
        std::lock_guard<std::mutex> lk(m_mtx);
        return m_queue.front();
    } 
    const T& front() const {
        std::lock_guard<std::mutex> lk(m_mtx);
        return m_queue.front();
    }
    // renc, why to remove front() and pop() for multiply customers ?
    // tbb concurrent_queue does not have front() and pop() functions.
    // https://www.threadingbuildingblocks.org/tutorial-intel-tbb-concurrent-containers 
    // https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
    // 
    bool try_pop(T &destination) {
        std::lock_guard<std::mutex> lk(m_mtx);
        if (m_queue.empty())
            return false;

        destination = m_queue.front();
        m_queue.pop();  
        return true;
    }
    bool empty() const {
        std::lock_guard<std::mutex> lk(m_mtx);
        return m_queue.empty();
    }

private:
    std::mutex m_mtx;
    std::condition_variable m_cv;

    std::queue<T> m_queue; // un-bound
};

class Producer
{
public:
    Producer(QueueCV<int> &q) : m_queue(q) {}
    void run()
    {
        int iN = 0;
        while(1) {
            m_queue.push(iN++);
        }
    }
private:
    QueueCV<int> &m_queue;
};

class Consumer
{
public:
    Consumer(QueueCV<int> &q, int iId) : m_queue(q), m_iId(iId) {}
    void run()
    {
        while(1) {
            int iN = -1;
            m_queue.pop(iN);
            std::cout << "Consumer id: " << m_iId << ", " << iN << std::endl;
        }
    }
private:
    QueueCV<int> &m_queue;
    int m_iId;
};
int main()
{
    QueueCV<int> queue;
    Producer p(queue);
    Consumer c0(queue, 10);
    Consumer c1(queue, 11);
    Consumer c2(queue, 12);

        std::thread tC0(&Consumer::run, c0);
        std::thread tC1(&Consumer::run, c1);
        std::thread tC2(&Consumer::run, c2);

    std::thread tP(&Producer::run, p);

    std::vector<std::thread> arrC;
    arrC.push_back(    std::move(tC0));
    arrC.push_back(    std::move(tC1));
    arrC.push_back(    std::move(tC2));
    
    tP.join();  
    for (auto &c : arrC)
        if (c.joinable())
            c.join();
    return 1; 
}