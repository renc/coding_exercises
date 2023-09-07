
/* 
https://eli.thegreenplace.net/2014/variadic-templates-in-c/
http://www.cs.rpi.edu/~musser/design/blitz/meta-art.html
https://github.com/elliotgoodrich/VariadicTemplateAlgorithms/blob/master/include/vta/algorithms.hpp 

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

// // std::integral_constant, std::false_type, std::true_type, std::is_same_v
namespace c11 {
    template <typename ...>
    using void_t = void;
}
// std::void_t is from c++17

// primary template
template<typename, typename = void>
struct has_type_member : std::false_type {};
// partial specialization
template<typename T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};

// C++11 SFINAE version , https://jguegant.github.io/blogs/tech/images/SFINAE-compile-time-introspection.pdf
template <typename T> struct has_foo11 {
    template <typename C>
    static constexpr decltype(std::declval<C>().foo(), bool()) test(int )
    {
        // we can return values, thanks to constexpr instead of playing with sizeof 
        return true;
    }

    template <typename C>
    static constexpr bool test(...) { return false; }

    // int is used to give the precedence !
    static constexpr bool value = test<T>(int());
}; 

// C++17 version 
// primary template 
template <typename T, typename = void>
struct has_foo : std::false_type {};
// partial specialization 
template <typename T>
struct has_foo<T, std::void_t<decltype(std::declval<T&>().foo())>> : std::true_type {};

struct ClassX {
    using type = std::string;
    void foo() {}
};
struct ClassY {
    using type = int;
    type foo() { return 10; };
};
struct ClassZ {
    void foo(int x) {};
};
template <typename T>
struct ClassZ1 {
    using type = T;
    void foo(T x) {};
};
void test_has()
{
    static_assert(has_type_member<int>::value == false);
    static_assert(has_type_member<std::string>::value == false);
    static_assert(has_type_member<ClassX>::value == true);
    static_assert(has_type_member<ClassY>::value == true);
    static_assert(has_type_member<ClassZ>::value == false);
    static_assert(has_type_member<ClassZ1<std::vector<int>> >::value == true);

    static_assert(has_foo<int>::value == false);
    static_assert(has_foo<std::vector<int>>::value == false);
    static_assert(has_foo<ClassX>::value == true);
    static_assert(has_foo<ClassY>::value == true);// didn't check the return type.
    static_assert(has_foo<ClassZ>::value == false);
    static_assert(has_foo<ClassZ1<int>>::value == false);

    static_assert(has_foo11<int>::value == false);
    static_assert(has_foo11<std::vector<int>>::value == false);
    static_assert(has_foo11<ClassX>::value == true);
    static_assert(has_foo11<ClassY>::value == true);// didn't check the return type.
    static_assert(has_foo11<ClassZ>::value == false);
    static_assert(has_foo11<ClassZ1<int>>::value == false);

    decltype(std::declval<ClassY>().foo()) a = 20;
}

// primary template 
template <typename T, typename = void>
struct can_reference : std::false_type {};
// partial specialization
template <typename T>
struct can_reference<T, std::void_t<T&>> : std::true_type {};

// primary class template
template<typename T>
struct is_void {
    static const bool value = false;
};
// "<>" means a full specialzation
template<>
struct is_void<void> {
    static const bool value = true;
};

// primary class template 
template<typename T>
struct is_pointer {
    static const bool value = false;
};
// "typename T" in "<>" means partial specialization
template <typename T>
struct is_pointer<T*> {
    static const bool value = true;
};


int main()
{
    test_has();
    
    return 0; 
}
/*
 g++ metaart.cpp -o metaart

*/