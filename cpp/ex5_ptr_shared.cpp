// first learn how to implement a simple shared_ptr with reference couting but without considering the deleter
// then there is another implement /home/renc/repo/hands-on-design-patterns-with-cpp/Chapter06/smartptr.C , which use type erase erasing to hind the deleter
//
// after u know about type erase, then go to ex5_function.cpp which use type erase too.


#include <iostream>
#include <string>
#include <atomic>



// custom shared_ptr , this version does not consider the Deleter as template parameter
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

int test ()
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
    if (0) {
        std::cout << "T5: \n";

    }
    return 0; 
}

//-----------------------------------------------------
namespace ns20241116 {
//https://andreasfertig.blog/2024/09/understanding-the-inner-workings-of-cpp-smart-pointers-the-shared_ptr/ 
struct ctrl_blk_base {
    std::atomic_uint64_t shared_ref_count_{1};
    void add_shared() { ++ shared_ref_count_; }
    void dec() { -- shared_ref_count_; }
    virtual void release_shared() = 0;
};

template <typename T>
struct ctrl_blk : ctrl_blk_base {
    T* data_; // payload 

    explicit ctrl_blk(T* data) : crbl_blk_base{}, data_(data) {}
    void release_shared() override {
        if (dec() == 0) {
            delete data_;
            delete this; // self delete, this is new to me
        }
    }
};

template <typename T>
struct ctrl_blk_with_storage : ctrl_blk_base {
    T in_place_;

    template <typename... Args>
    explicit ctrl_blk_with_storage(Args&&... vals) : ctrl_blk_base{}, in_place_{std::forward<Args>(vals)...} {}
    
    T* get() { return &in_place_; }
    void release_shared() override {
        if (dec() == 0) { delete this; } // self delete
    }
}; // used by make_shared<>


template <typename T>
class shared_ptr {
    ctrl_blk_base* ctrl_blk_{};
    T*             t_{};

    shared_ptr(ctrl_blk_with_storage<T>* cb) : shared_ptr(cb, cb->get()) {}
    shared_ptr(ctrl_blk_base* cb, T* t) : ctrl_blk_{cb}, t_{t} {} 

    template <typename T, typename... Args>
    friend shared_ptr<T> make_shared(Args&&... vals);

public:
    shared_ptr() = delete;
    shared_ptr(T* t) : shared_ptr(new ctrl_blk<T>{t}, t) {} // bug ? if t== nullptr
    ~shared_ptr() {
        if (ctrl_blk_) { ctrl_blk_->release_shared(); }
    }

    shared_ptr(const shared_ptr& rhs)
    : ctrl_blk_(rhs.ctrl_blk_), t_(rhs.t_) {
        if (ctrl_blk_) ctrl_blk_->add_shared();
    }
    shared_ptr(shared_ptr&& rhs) : ctrl_blk_{rhs.ctrl_blk_}, t_t(rhs.t_) {
        rhs.ctrl_blk_ = nullptr;
        rhs.t_        = nullptr;
    }

    shared_ptr& operator=(const shared_ptr& rhs) {
        shared_ptr{rhs}.swap(*this); //forward to copy ctor
        return *this;
    } // it is a clever trick, create a new temporary shared_ptr object 
    shared_ptr& operator=(shared_ptr&& rsh) {
        shared_ptr{std::move(rsh)}.swap(*this); // forward to move ctor
        return *this;
    } 

    void swap(shared_ptr& rhs) {
        std::swap(ctrl_blk_, rhs.ctrl_blk_);
        std::swap(t_, rhs.t_);
    }
};

template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&& vals)
{
    return new ctrl_blk_with_storage(std::forward<Args>(vals)...); // this will trigger the shared_ptr ctr
}
}

int main()
{
    test();

    // 

}





/*
~/ws/coding_exercises/cpp$ rm ex5_shptr; clear; g++ -o ex5_shptr ex5_shptr.cpp ; ./ex5_shptr

// ----------
how about the deleter of shared_ptr which use a second func parameter, 
different from deleter of unique_ptr which use a second template parameter

*/
