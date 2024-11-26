// question: what is the different of std::auto_ptr vs std::unique_ptr, why deprecate it ?
#include <iostream>
namespace ns20241126 {

template <typename T>
class auto_ptr 
{
private: 
    T* m_ptr; 
public:
    auto_ptr(T* ptr = nullptr) : m_ptr(ptr) { std::cout << "ctor\n"; }

    auto_ptr(/*const*/ auto_ptr& rhs) : m_ptr(rhs.release()) { std::cout <<"copy ctor\n"; }
    auto_ptr& operator=(/*const*/ auto_ptr& rhs) { std::cout <<"op==\n"; 
        reset( rhs.release() );
        return *this;
    } 
    ~auto_ptr() { delete m_ptr; std::cout <<"~auto_ptr\n"; }

    T* release() {std::cout << "release()\n";
        T* tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }
    void reset(T* ptr = nullptr) { std::cout << "reset()\n";
        if (m_ptr != ptr) {
            delete m_ptr;
            m_ptr = ptr;
        }
    }
    T& operator*() const { return *m_ptr; }
}; // edn of auto_ptr 

void doSomething(auto_ptr<int> p) {
    // p is got deleted when go out of the scope 
}
void test()
{
    auto_ptr<int> p1(new int(10));
    doSomething(p1);
    std::cout << *p1 << '\n';
}
} // eod of ns 


int main()
{
    ns20241126::test();
    /*[renc@renc-laptop cpp]$ ./a.out 
    ctor
    release()
    copy ctor
    ~auto_ptr
    Segmentation fault */ 
}
/*
// the problem is at copy ctr and op== , it change the previously owned ptr to null;
// which is not as usual for other copy ctr and op==, it will mark the rhs as const
*/