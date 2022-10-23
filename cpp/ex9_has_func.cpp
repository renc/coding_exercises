
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


namespace c11 {
    template <typename ...>
    using void_t = void;
}

// primary template
template<typename, typename = void>
struct has_type_member : std::false_type {};
// partial specialization
template<typename T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};

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