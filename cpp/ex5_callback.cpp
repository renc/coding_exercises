#include <queue>
#include <memory>
#include <iostream>
#include <string>
#include <string_view>
#include <array>
#include <functional>

class Handler1 {
public:
    ~Handler1() { std::cout << "~Handler1\n"; }
    int doSth(int a, std::string_view sv) { std::cout << "H1::doSth " << a << ", " << sv << ".\n"; return 1; }
};
int doFunc(int a, std::string_view sv) { std::cout << "doFunc " << a << ", " << sv << ".\n"; return 1; }
using FuncType = int (*)(int, std::string_view) ; // old school stype

class Caller
{
public:
    void regCb0(FuncType inFun)
    {
        m_f0 = inFun;
    }
    template <typename T>
    void regCb1(T *obj, int (T::*funcPtr)(int, std::string_view))
    {
        m_f1 = [=](int a, std::string_view sv) { return (obj->*funcPtr)(a, sv); } // [&] crashes
    }
    template <typename T, int (T::*method)(int, std::string_view)>
    void regCb2(T *obj)
    {
        m_f2 = [=](int a, std::string_view sv) { return (obj->*method)(a, sv); } 
    }
    
    void run() {
        std::cout << "run start\n";
        std::string hw = "hello world!";
        m_f0(0, hw);
        m_f1(1, hw);
        m_f2(2, hw);
    }

    FuncType m_f0{nullptr};
    std::function<int (int, std::string_view)> m_f1;
    std::function<int (int, std::string_view)> m_f2{}; 
};
void testCb()
{
    Caller caller;
    caller.regCb0(doFunc);

    Handler1 h1;
    caller.regCb1<Handler1>(&h1, &Handler1::doSth);
    caller.regCb2<Handler1, &Handler1::doSth>(&h1);
    caller.run();
}
