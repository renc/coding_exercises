/*

ex8_producerconsumer_cv1.cpp first, simple  
ex8_producerconsumer_cv2.cpp is a refactor of that version. 

Producer Consumer with std::queue 
https://gist.github.com/iikuy/8115191 

g++ filename.cpp -o exename -std=c++11 -pthread 

*/ 
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream> 
#include <vector> 

class QueueCV {
public:
    void push(int n)
    {
        {
            std::lock_guard<std::mutex> lk(m_mtx);
            m_queue.push(n);
        } // Important, unlock before notify, otherwise, the later 
        m_cv.notify_all();
    }
    int pop() 
    {
        std::unique_lock<std::mutex> lk(m_mtx);
        m_cv.wait(lk, [this](){ return m_queue.empty() == false;});
        // same as
        // while ( ! (q.empty() == false) ) 
        //     m_cv.wait(lk);
        int iRet = m_queue.front();
        m_queue.pop();
        return iRet;
        // this could be blocking (not no-blocking)
    }
private:
    std::mutex m_mtx;
    std::condition_variable m_cv;

    std::queue<int> m_queue; // un-bound
};

class Producer
{
public:
    Producer(QueueCV &q) : m_queue(q) {}
    void run()
    {
        int iN = 0;
        while(1) {
            m_queue.push(iN++);
        }
    }
private:
    QueueCV &m_queue;
};

class Consumer
{
public:
    Consumer(QueueCV &q, int iId) : m_queue(q), m_iId(iId) {}
    void run()
    {
        while(1) {
            int iN = m_queue.pop();
            std::cout << "Consumer id: " << m_iId << ", " << iN << std::endl;
        }
    }
private:
    QueueCV &m_queue;
    int m_iId;
};
int main()
{
    QueueCV queue;
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