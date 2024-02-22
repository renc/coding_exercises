// 2024/01/30, v good for interview question, memeory ownership, -> custom deleter -> type erase -> customer allocator 

#include <iostream>
#include <atomic> // std::atomic 
namespace n20240130 {

template <typename T>
class SmartPtr {
public:
    SmartPtr(T* ptr) : m_ptr(ptr) { std::cout << "SmartPtr(T*), T=" << typeid(T).name() << "\n"; }
    T& operator*() { return *m_ptr; }
    T* operator->(){ return m_ptr; }

    template<typename U>
    SmartPtr(const SmartPtr<U>& rhs) {
        std::cout << "SmartPtr cctr\n";
    } // why this one is not called, even wo SmartPtr(const SmartPtr<T>&){};
    SmartPtr(const SmartPtr<T>& rhs) {
        std::cout << "SmartPtr cctr\n"; m_ptr = rhs.m_ptr;
    }
    ~SmartPtr() { std::cout << "SmartPtr dtr\n"; 
    //delete m_ptr;  // free(): double free detected in tcache 2 . crash 
    } // 
private:
    T* m_ptr;
};
struct Obj {
    Obj(int id = 0) :m_id(id) {} ;
    Obj(const Obj&) = default;
    Obj(Obj&& ) = default;
    Obj& operator=(const Obj&) = default;
    Obj& operator=(Obj&& ) = default;
    int m_id {0};
}; // i use this as test obj 
std::ostream& operator<<(std::ostream& os, const Obj& obj) 
{ os << "Obj id=" << obj.m_id << "; "; return os;}
void test()
{   std::cout << __FUNCTION__ << "\n";
    {
    SmartPtr<int> p0(new int{10});
    std::cout << "p0 val=" << *p0 << "\n";
    SmartPtr<int> p1(p0);
    std::cout << "p1 val=" << *p1 << "\n";
    } // dtr func is called twice, which is wrong 
    {
    SmartPtr<Obj> p0(new Obj(11));
    SmartPtr<Obj> p1 = p0; // ?? which ctr is called 
    std::cout << "p0 " << *p0 << ", p1 "  << *p1<< ";\n";
    } // dtr func is called twice, which is wrong
}
// there is issue above on how to handle the copy SmartPtr and destructor
// let's look at reference count for sharing ownership. (unique_ptr is another way)
template <typename T>
struct CtrlBlock {
    CtrlBlock(T*ptr = nullptr) : m_ptr(ptr), m_count(0) { if (m_ptr) m_count.store(1); };
    T* m_ptr = nullptr; // why hold a ptr, to delete
    std::atomic<int> m_count = 0;
    void increase() { m_count.fetch_add(1); }
    void decrease() { 
        if (m_count.load() > 0) {
            m_count.fetch_sub(1); 
            if (m_count.load() == 0) {
                delete m_ptr; m_ptr = nullptr; std::cout << "CtrlBlock dtr\n";
            }
        }
    } 
};
template <typename T>
struct SmartPtrRC {
    SmartPtrRC(T* ptr) : m_ptr(ptr), m_pRC(new CtrlBlock<T>(ptr)) {}
    T* m_ptr = nullptr;
    CtrlBlock<T>* m_pRC; 
    ~SmartPtrRC() {
        if (m_pRC != nullptr) {
            m_pRC->decrease(); // an issue here, when it == 0, should delete m_pRC also ?
        }
    }
    SmartPtrRC(const SmartPtrRC<T>& rhs) {
        if (this == &rhs) return;
        m_ptr = rhs.m_ptr;
        m_pRC = rhs.m_pRC;
        if (m_ptr != nullptr) m_pRC->increase();
    }
    SmartPtrRC& operator=(const SmartPtrRC<T>& rhs) {
        if (this == &rhs) return *this;
        if (m_ptr != nullptr) m_pRC->decrease();
        m_ptr = rhs.m_ptr;
        m_pRC = rhs.m_pRC;
        if (m_ptr != nullptr) m_pRC->increase();
        return *this;
    }
    T& operator*() { return *m_ptr; }
    T* operator->() { return m_ptr; }
}; 
void testSmartPtrRC()
{   std::cout << __FUNCTION__ << "\n";
    {
    SmartPtrRC<int> p0(new int{10});
    std::cout << "p0 val=" << *p0 << "\n";
    SmartPtrRC<int> p1(p0);
    std::cout << "p1 val=" << *p1 << "\n";
    } // dtr func is called twice, which is wrong 
    {
    SmartPtrRC<Obj> p0(new Obj(11));
    SmartPtrRC<Obj> p1 = p0; // ?? which ctr is called 
    std::cout << "p0 " << *p0 << ", p1 "  << *p1<< " count=" << p1.m_pRC->m_count << ";\n";
    } 
}
// how to move ctr, move assignment operator= 
// how to implement make_shared;
// how to handle custom deleter 
} // end namespace n20240130

int main()
{
    n20240130::test();
    n20240130::testSmartPtrRC();
    return 0;
}