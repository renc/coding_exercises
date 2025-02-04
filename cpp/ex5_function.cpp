#include <memory> 
#include <iostream> 
// 2024/01 before reading this implement, please check type erase first (in hands-on-design-patterns-with-cpp/Chapter06/smartptr.C)
// ex5_ptr_unique.cpp, also show the size different between functor == lambda < func ptr < std::function 
// 2023/11 ref: 2022 C++ Software Design Principles and Patterns , Guildline 32, std::function, std::any, std::shared_ptr  

namespace cw {
// C++ Weekly - Ep 333 - A Simplified std::function Implementation
// basic version 1: support function pointer;
// version 2: support lambda 
template <typename T>
struct function;

template <typename Ret, typename ... Param> 
struct function<Ret (Param ... )>
{
    function(Ret (*f)(Param ...))
        : m_actual_callable { std::make_unique<callable_impl<Ret (*)(Param ...)>>(f)}
    {} // need sth to store the func , small func, big fuc, type erasure, type erase 
    Ret operator() (Param ... param) { return m_actual_callable->call(param...); } 

    struct callable_interface
    {
        virtual Ret call(Param ...) = 0; // pure virtual
        virtual ~callable_interface() = default;
        // finish rule of 5
    }; // renc, why need this callable_interface ? is it because need to put into unique_ptr 

    template <typename CallableT>
    struct callable_impl : public callable_interface
    {
        callable_impl(CallableT _callable) : m_callable{std::move(_callable)} {}

        Ret call(Param... param) override { return m_callable(param...); }
        CallableT m_callable;
    };
    std::unique_ptr<callable_interface> m_actual_callable; // renc, dynamic memory, (law latency no)
};

// a more generic, not limit to function pointer say int (int, int) , to support lambda also
namespace v2 {
template <typename T>
struct function; 
template <typename Ret, typename ... Param> 
struct function<Ret (Param ... )>
{
    template <typename FunctionObject>
    function(FunctionObject fo)
        : m_actual_callable { std::make_unique<callable_impl<FunctionObject>>(fo)}
    {} // need sth to store the func , small func, big fuc, type erasure, 
    Ret operator() (Param ... param) { return m_actual_callable->call(param...); } 

    struct callable_interface
    {
        virtual Ret call(Param ...) = 0; // pure virtual
        virtual ~callable_interface() = default;
        // finish rule of 5
    };

    template <typename CallableT>
    struct callable_impl : public callable_interface
    {
        callable_impl(CallableT _callable) : m_callable{std::move(_callable)} {}

        Ret call(Param... param) override { return m_callable(param...); } // std::invoke(callable, param...); #inlcude <functional>
        CallableT m_callable;
    };
    std::unique_ptr<callable_interface> m_actual_callable; 
};
} // end of namespace v2;

int f(int x, int y) { return x+y; }

void test()
{
    
    //function<int (int, int)> func;
    //func(1, 2);  // error: undefined reference to `cj::function<int (int, int)>::operator()(int, int)' 

    function<int (int, int)> func{f};
    std::cout << func(1, 2) <<"\n";

    v2::function<int (int, int)> func2{[z=1](int x, int y) { return x+y+z; }};
    std::cout << func(1, 2) + func2(3, 4) << "\n";

    v2::function<bool (int)> func3{[](int x) { return (bool)x; }};
    std::cout << func3(0) << " and " << func3(2) << "\n";
}
}

namespace ms {
namespace part1 {
// https://devblogs.microsoft.com/oldnewthing/20200513-00/?p=103745 
// very similar (on top of) to C++ Weekly version 
// to implement std::function<bool(int, char*)> 
struct callable_base
{
    callable_base() = default;
    virtual ~callable_base() {}
    virtual bool invoke(int, char*) = 0; 
    virtual std::unique_ptr<callable_base> clone() = 0; 
};
template <typename T>
struct callable : public callable_base
{
    T m_t;
    callable(T const& t) : m_t(t) {}
    callable(T&& t) : m_t(std::move(t)) {} // RValue reference , not forward/universe reference

    bool invoke(int a, char* b) override { return m_t(a, b); }
    std::unique_ptr<callable_base> clone() override { return std::make_unique<callable>(m_t); }
}; // end of class 

struct function
{
    std::unique_ptr<callable_base> m_callable;

    template<typename T>
    function(T&& t) : 
        m_callable(new callable<std::decay_t<T>>(std::forward<T>(t)) ) {}

    function(const function& rhs) :
        m_callable(rhs.m_callable ? rhs.m_callable->clone() : nullptr) {}
    function(function&& rhs) = default;

    bool operator()(int a, char* b) { return m_callable->invoke(a, b); }

}; // end of class 

} // end of namespace part1
namespace part2 {
// https://devblogs.microsoft.com/oldnewthing/20200514-00/?p=103749    how to optimize small memory 
} // end of namespace part2 
} // end namespace ms


void test()
{
    std::function<int (void)> f;
}

int main()
{
    cw::test();
    return 0; 
}
