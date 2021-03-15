
/* 


http://www.cs.rpi.edu/~musser/design/blitz/meta-art.html

*/ 

#include <cstdio>
#include <cmath>
#include <algorithm>
#include <type_traits>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <deque>

namespace meta {

    template <int n> 
    struct Square { static constexpr int value = n * n;  };

    template<int N>
    class Factorial {
    public:
        enum { value = N * Factorial<N - 1>::value };
    };
    template <>
    class Factorial<0> {
    public:
        enum { value = 1 };
    };
    // !!template specialisation, required for terminating condition
    template<int N>
    constexpr int fibonacci()
    {
        if constexpr (N >= 2)
            return fibonacci<N - 1>() + fibonacci<N - 2>();
        else
            return N;
    } // using the constexpr c++17 

    // template variable (variable template)
    template <uint32_t n>
    constexpr auto fib = fib<n - 1> + fib<n - 2>;
    template <>
    constexpr auto fib<1> = 1;
    template <>
    constexpr auto fib<0> = 0;

    template <int n> struct Pow2 { 
        static constexpr int value = 2 * Pow2<n - 1>::value; 
    };
    template <> struct Pow2<0> {
        static constexpr int value = 1; 
    };

    // use SFINAE to restrict the input types 
    // ? how to write an function only accept floating point as variables? 
    template <typename T> struct restrictor {};
    template <> struct restrictor<float> { typedef float result; };
    template <> struct restrictor<double> { typedef double result;  };

    template <typename T>
    typename restrictor<T>::result distance(T a1, T a2, T b1, T b2) {
        T temp1 = a1 - b1;
        T temp2 = a2 - b2; 
        return std::sqrt(temp1 * temp1 + temp2 * temp2);
    }

    // ? how to rewrite this using enable_if 

    // -------------------------------------------------
    // 
    // function pointer as template parameter 
    template <double (*mfunction) (size_t)>
    void tabulateFunPtr(size_t n) {
        std::cout << std::fixed << std::setprecision(3);
        for (size_t i = 0; i < n; ++i) {
            std::cout << std::setw(5) << i
                << std::setw(10) << mfunction(i)
                << std::endl;
        }
    }
    template <double mfunction (size_t)>
    void tabulateFunPtr2(size_t n) {
        std::cout << std::fixed << std::setprecision(3);
        for (size_t i = 0; i < n; ++i) {
            std::cout << std::setw(5) << i
                << std::setw(10) << mfunction(i)
                << std::endl;
        }
    }

    // class template as template parameters
    template <  
        typename Element,
        // Every STL container has two type parameters Container<Element, Allocator>
        // The allocator has the type parameter Allocator<Element>
        template<typename, typename> class Container = std::deque
    >
        class Stack {
        private:
            Container <Element, std::allocator<Element>> _stack;
        public:
            void push(const Element& e) { _stack.push_back(e); }
            Element pop() {
                auto x = _stack.back();
                _stack.pop_back();
                return x;
            }
            size_t size() { return _stack.size(); }
            bool empty() { return _stack.empty(); }
            void clear() { _stack.clear(); }
            void print() {
                std::cout << "stack: ";
                for (const auto& e : _stack) std::cout << e << " ";
                std::cout << "\n";
            }
            // ? how to support the for auto loop
    };

    // function pointer + container as template parameters 
    // to  perform fold-operation 
    template< typename Acc, typename Func, 
        typename Element, template<typename> class Allocator, 
        template<typename, typename> class Container>
    Acc folderContainer(Acc init, const Container<Element, Allocator<Element>>& cont, Func func) {
        Acc acc{ init };
        for (const auto& e : cont) acc = func(acc, e);
        return acc;
    }
    
    // -------------------------------------------------
    // 
    template <typename A, typename B, typename C>
    struct tuple3 {
        tuple3() {} // empty ctor, nessarty to story by value the tuple in STL containers.
        tuple3(const A& a, const B& b, const C& c) : a(a), b(b), c(c) {}
        A a; B b; C c;
    };

    template <typename A, typename B, typename C>
    auto printTuple(const tuple3<A, B, C>& t) -> void {
        std::cout << "tuple3{" << t.a << ", " << t.b << ", " << t.c << "} ";
    }
     /*
    template <typename Iterator>
    auto printContainer(const Iterator& b, const Iterator& e, const std::string& sep = ", ") -> void {
        for (auto it = begin; it != end; ++it) std::cout << *it << sep;
    }

    template <typename Container>
    auto printContainer2(const Container& container, const std::string& sep = ", ") -> void {
        for (const auto& e : container) std::cout << e << sep;
    }  */
       /*
    template<typename KEY, typename VALUE>
    void printMap(std::map<KEY, VALUE>& mp, int w1 = 10, int w2 = 10) {
        for (const auto& p : mp)
            std::cout << std::setw(w1) << p.first
            << std::setw(w2) << p.second
            << "\n";
        std::cout << "\n";
    } */

    template<typename Container>
    void printAssoc(const Container& mp, int w1 = 10, int w2 = 10) {
        for (const auto& it : mp)
            std::cout << std::setw(w1) << it.first
            << std::setw(w2) << it.second
            << "\n";
        std::cout << "\n";
    }

    template < template <typename, typename> class C, 
        typename T, typename Allocator 
    >
        void print_container(C<T, Allocator> container) {
        for (const T& v : container)
            std::cout << v << ", ";
        std::endl;
    }

    template <typename First, typename ... Args>
    void print(const First& firstarg, const Args&... args) {
        std::cout << firstarg; // since the firstarg does not need " " ahead.
        auto spaceBefore = [](const auto& arg) -> const auto& {
            std::cout << " ";
            return arg;
        };
        (std::cout << ... << spaceBefore(args)) << "\n"; // parenthese () is part of fold expression. 
    }
    template <typename First, typename ... Args>
    void print_foldedFuncCall(const First& firstarg, const Args&... args) {
        std::cout << firstarg; // since the firstarg does not need " " ahead.
        auto outWithSpace = [](const auto& arg) -> const auto& {
            std::cout << " " << arg;
        };
        (... , outWithSpace(args)); // using the comma operator for fold expression
        std::cout << "\n"; 
    } // it turns out: outWithSpace(arg1), outWithSpace(arg2), outWithSpace(arg3), and the others

    namespace variadic {
        // variadic templates (variadic class templates) 
        template <typename... T>
        struct Tuple { }; // basic (empty) tuple, i.e. Tuple<> object;
        template <typename T, typename ...Ts>
        struct Tuple<T, Ts...> {
            T first;
            Ts rest;
            Tuple(const T& f, const Ts& ... r) : first(f), rest(r...) {}
        };
        void testTuple() {
            Tuple<> t0;
            Tuple<bool> t1(false);
            Tuple<int, char, std::string> t2(1, 'a', "ABC");
        }

        // variadic templates (variadic function templates) (cpp11, using recursion) 
        bool isAllTrue() { return true;  } // basic and default value.
        template <typename T, typename ... Ts> 
        bool isAllTrue(T t, Ts ... ts) { return t && isAllTrue(ts); } // using recursion 

        template <typename  ... Args >
        constexpr bool isAll(Args... args) { return ( true && ...&& args); } 
        // cpp17 Flod expression, all with default value. this simply the cpp11 which require recursion. 

        void test_isAll() {
            isAll(); // this mean variadic template can support zero argument. 
            static_assert(isAll()); // this will error if remove the constexpr from isAll.
            isAll(true);
            isAll(false);
            isAll(true, true);
        }

        
        template <typename H, typename ... Ts>
        auto add(H head, Ts ... tail) { return head + add(tail...); } // recursion 
        template <typename H> auto add(H head) { return head;  }
        // at cpp17, with fold expressions
        template <typename H, typename ... Ts> 
        auto add2(H head, Ts ... tail) { return (head + ... + tail);  }
        // expands to: head + tail[0] + tail[1] + ... 
        
        //template <typename ...Args> auto sum(Args ... args) { return (args + ...); } // this does not have value for sum();
        template <typename ...Args> auto sum(Args ... args) { return (args + ... + 0); } // init value, support sum()
        
        // // https://foonathan.net/2020/05/fold-tricks // 
        // 
        // call a function with each element 
        // for (auto elem : ts) f(elem);  
        // ( f(ts), ... ); 
        // 
        // call a function with each element until a predicate matches
        // for (auto elem : ts) {
        //   if (pred(elem)) break;
        //   f(elem);
        // } 
        // ( (pred(ts) ? false : (f(ts), true) ) && ... ); 
        // expands to (pred(ts[0]) ? false : (f(ts[0], true))
        //         && (pred(ts[1]) ? false : (f(ts[1], true)) && ... 

        // check whether passed types are homogeneous
        template< typename T1, typename... TN>
        struct IsHomogeneous {
            static constexpr bool value = (std::is_same_v<T1, TN> && ...);
        };
        // check whether passed arguments have the same type
        template<typename T1, typename ...TN>
        constexpr bool isHomogeneous(T1, TN...) {
            return (std::is_same_v<T1, TN> && ...);
        }
    } // namespace variadic 
    namespace ctv {
        // compile time value list from book <C++ template> version2.
        template<typename T, T ... Values>
        struct ValueList {};

        template<typename List>
        struct IsEmpty {
            static constexpr bool value = false;
        };
        template<typename T, T ... Values>
        struct IsEmpty<ValueList<T, Values...>> {
            static constexpr bool value = sizeof ... (Values) == 0;
        };

        template <typename T, T Value>
        struct CTValue {
            static constexpr T value = Value;
        };

        template <typename List> struct FrontT;
        // compile error without this, the following FrontT<ValueList > error: 
        // explicit specialization of undeclared template struct 'FrontT'

        template <typename T, T Head, T... Tail>
        struct FrontT<ValueList<T, Head, Tail...>> {
            using Type = CTValue<T, Head>;
            static constexpr T value = Head;
        };

        template <typename List> struct PopFrontT;
        template <typename T, T Head, T... Tail>
        struct PopFrontT<ValueList<T, Head, Tail...>> {
            using Type = ValueList<T, Tail...>;
        };


        void test_ValueList() {
            ValueList<int> a;
            static_assert(IsEmpty< ValueList<int> >::value == true);
            static_assert(IsEmpty< ValueList<int, 4, 5> >::value == false);
            static_assert(FrontT< ValueList<int, 3, 4, 5> >::value == 3);
            PopFrontT< ValueList<int, 4, 5> >::Type b;
            static_assert(FrontT< PopFrontT< ValueList<int, 4, 5> >::Type >::value == 5);
        }
    }
    namespace enable_if {
        // think about how these template instantiation, how to implement the std::enable_if_t;
        // http://www.vishalchovatiya.com/c-template-a-quick-uptodate-look/
        template <typename T, typename std::enable_if_t<!std::is_pointer<T>::value>* = nullptr>
        void func(T val) {
            cout << "Value" << endl;
        }

        template <typename T, typename std::enable_if_t<std::is_pointer<T>::value>* = nullptr>
        void func(T val) {  // NOTE: function signature is NOT-MODIFIED
            cout << "Pointer" << endl;
        }
        
        void test() {
            int a = 0;
            func(a);
            func(&a);
        }
#ifdef INSIGHTS_USE_TEMPLATE
        template<>
        void func<int, void*>(int val)
        {
            std::operator<<(std::cout, "Value").operator<<(std::endl);
        }
        template<>
        void func<int*, void*>(int* val)
        {
            std::operator<<(std::cout, "Pointer").operator<<(std::endl);
        }
#endif

    }
} // end of namespace meta

using namespace meta;

int main()
{

    printf("Square<3> is : %d.\n", Square<3>::value);

    printf("Factorial<0> is : %d.\n", Factorial<0>::value);
    printf("Factorial<1> is : %d.\n", Factorial<1>::value);
    printf("Factorial<3> is : %d.\n", Factorial<3>::value);
    printf("Factorial<10> is : %d.\n", Factorial<10>::value);

    std::cout << "Pow2<4>=" << meta::Pow2<4>::value << std::endl;
    std::cout << "Pow2<0>=" << meta::Pow2<0>::value << std::endl;

    std::cout << "\n";
    {
        tabulateFunPtr<std::sqrt>(9);
    }
    std::cout << "\n";
    {
        auto tdata = std::vector<tuple3<int, char, std::string>>{
            {11, 'x', "hello"}, {23, 'y', "tuple"}
        };
        std::for_each(tdata.begin(), tdata.end(), printTuple<int, char, std::string>);
        std::for_each(tdata.begin(), tdata.end(), &printTuple<int, char, std::string>);
    }

    std::cout << "\n";
    {
        Stack<std::string, std::vector> s;
        s.push("C++"); s.push("hello");
        s.print();
    }
    {
        std::vector<int> arr{ 1, 2, 3, 4, 5 };
        std::cout << folderContainer<int>(0, arr, [](int acc, int e) {return 10 * acc + e; })
            << std::endl;
        std::cout << folderContainer<int>(1, arr, std::plus<int>()) << std::endl;
    }
    {
        
    }
    return 0; 
}
/*
 g++ metaart.cpp -o metaart

*/