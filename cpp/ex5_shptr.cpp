#include <iostream>
#include <string>

template <typename T>
class shptr
{
public:
    shptr() { std::cout << "shptr.ctr, rc=null \n"; }
    shptr(T* obj) : m_obj(obj), m_rc(new uint32_t(1)) { std::cout << "shptr.ctr, rc=" << *m_rc << "\n"; }
    shptr(const shptr& rhs) 
    {
        m_obj = rhs.m_obj;
        m_rc  = rhs.m_rc;
        ++(*m_rc);
        std::cout << "shptr.ctr, rc=" << *m_rc << "\n";
    }
    shptr(shptr&& rhs) 
    { 
        m_obj = rhs.m_obj;  rhs.m_obj = nullptr;
        m_rc  = rhs.m_rc;   rhs.m_rc = nullptr;
        std::cout << "shptr.mtr, rc=" << *m_rc << "\n";
    }
    shptr& operator=(const shptr& rhs)
    {
        cleanup(); // clean the current one which could point to a valid obj
        m_obj = rhs.m_obj; 
        m_rc = rhs.m_rc;
        ++(*m_rc);
        std::cout << "shptr.c=, rc=" << *m_rc << "\n";
        return *this;
    }
    shptr& operator=(shptr&& rhs) 
    { 
        cleanup(); 
        m_obj = rhs.m_obj;  rhs.m_obj = nullptr;
        m_rc  = rhs.m_rc;   rhs.m_rc = nullptr;
        std::cout << "shptr.m=, rc=" << *m_rc << "\n";
        return *this;
    }
    ~shptr() { std::cout << "shptr.dctr\n"; cleanup(); }

    T* operator->() { return m_obj; }
    T& operator* () { return *m_obj;}
    T* get() { return m_obj; }
    uint32_t use_count() const { return *m_rc; }

private:
    void cleanup()
    {
        if (m_rc == nullptr)
            return; // nothing to do
        if (*m_rc > 0)
            --(*m_rc);
        if (*m_rc == 0)
        {
            if (m_obj != nullptr)
                delete m_obj; 
            m_obj = nullptr;
        }
    }
    T* m_obj {nullptr};
    uint32_t* m_rc{nullptr};
};

struct  A
{
    A() { std::cout << "A.ctr()\n"; }
    A(const A& a) { std::cout << "A.ctr\n"; }
    A(A&& a) { std::cout << "A.mtr\n"; }
    A& operator=(const A&a) { std::cout << "A.c="; return *this; }
    A& operator=(A&&a) { std::cout << "A.m="; return *this;}
    virtual ~A() { std::cout << "A.dctr\n"; }
    std::string m_name {"A0"};
};
struct B: public A
{
    B() { std::cout << "B.ctr()\n"; }
    B(const B& a) { std::cout << "B.ctr\n"; }
    B(B&& a) { std::cout << "B.mtr\n"; }
    B& operator=(const B&a) { std::cout << "B.c="; return *this; }
    B& operator=(B&&a) { std::cout << "B.m="; return *this;}
    ~B() { std::cout << "B.dctr\n"; }
};

int main ()
{
    if (0) {
        std::cout << "T1: \n";
        A* pA0 = new A();
        shptr<A> pS0(pA0);
        shptr<A> pS1 = pS0; 
    }
    if (0) {
        std::cout << "T2: \n";
        A* pA0 = new A();
        shptr<A> pS0(pA0);
        shptr<A> pS1;
        pS1 = std::move(pS0); 
    }
    if (1) {
        std::cout << "T3: \n";
        A* pA0 = new B();
        shptr<A> pS0(pA0);
        shptr<A> pS1 = pS0; 
    }
    if (1) {
        std::cout << "T4: \n";
        A* pA0 = new B();
        shptr<A> pS0(pA0);
        shptr<A> pS1;
        pS1 = std::move(pS0); 
        std::cout << "name=" << pS1->m_name << std::endl;
        std::cout << "name=" << (*pS1).m_name << std::endl;
    }
    return 0; 
}

/*
~/ws/coding_exercises/cpp$ rm ex5_shptr; clear; g++ -o ex5_shptr ex5_shptr.cpp ; ./ex5_shptr
*/