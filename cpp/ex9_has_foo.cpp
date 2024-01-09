#include <iostream>
#include <type_traits>


namespace di {
// ref: tartanllama blog detection-idiom
struct has_special_support { 
    int get_foo() const { return 123; }
};
struct will_need_generic_calculation {}; 

namespace v1 {
// first version, C++11, will be replaced by newer/better version
namespace detail {
template <class T>
auto calculate_foo_factor(const T& t, int) -> decltype(std::declval<T>().get_foo()) {
    return t.get_foo();
}
template <class T>
int calculate_foo_factor(const T&t, ...) { return 43; } // generic calculation version here
} // end namespace detail

template <class T>
int calculate_foo_factor(const T&t)
{
    return detail::calculate_foo_factor(t, 0);
}

// what is std::declval trickery , which does not requrie T with default-constructible
// struct foo { 
//  foo() = delete; foo(int); void do_something();    
// };
// decltype(foo{}.do_something())               // invalid
// decltype(std::declval<foo>().do_something()) // find 

} // end namespace v1


namespace v2 {
// detection idiom
template <class ... Ts>
using void_t = void; // void_t is since c++17, my favious, in cppref, it used to detect SFINAE has type

template <class T, class=void>
struct supports_foo : std::false_type {};

template <class T>
struct supports_foo<T, void_t<decltype(std::declval<T>().get_foo())>> : std::true_type{};

template <class T, std::enable_if_t<supports_foo<T>::value>* = nullptr>
auto calculate_foo_factor(const T&t ) { return t.get_foo(); }

template <class T, std::enable_if_t<!supports_foo<T>::value>* = nullptr>
auto calculate_foo_factor(const T&t) { return 42; } // generic calculation here 
} // end namespace v2
// the author propose a better way to do it using is_detected ... 

void test()
{
    has_special_support h1;
    will_need_generic_calculation g1;

    std::cout << v1::calculate_foo_factor(h1) << std::endl;
    std::cout << v1::calculate_foo_factor(g1) << std::endl;

    std::cout << v2::calculate_foo_factor(g1) << std::endl;
    std::cout << v2::calculate_foo_factor(h1) << std::endl;
}
} // end namespace di

int main()
{
    di::test();
    return 0;
}
