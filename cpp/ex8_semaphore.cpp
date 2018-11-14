
// What is semaphore ?
// https://www.justsoftwaresolutions.co.uk/threading/locks-mutexes-semaphores.html
// 
// How to implement
// https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads
// 

#include <mutex>
#include <condition_variable> 

class semaphore
{
public:
    // let a waiting one to acquire the lock.
    void notify()
    {
        std::lock_guard<std::mutex> lk(m_mtx);
        ++m_iCount;
        m_cv.notify_one();
    }
    // wait for the lock (wait for a lock be released)
    void wait()
    {
        std::unique_lock<std::mutex> lk(m_mtx);
        while (m_iCount == 0) // Handle spurious wake-ups.
            m_cv.wait(lk); //unlock, and wait for waitup

        // when it come down here, lk is locked. 
        // the current thread will consume 1 from m_iCount;
        --m_iCount;
    }
    bool try_wait()
    {
        std::unique_lock<std::mutex> lk(m_mtx);
        if (m_iCount > 0)
        {
            --m_iCount;
            return true;
        }
        return false;
    }
private:
    std::mutex m_mtx;
    std::condition_variable m_cv;
    int m_iCount = 0; // initialized as locked. 
    //int m_iCount = 1; // why this is wrong?
};