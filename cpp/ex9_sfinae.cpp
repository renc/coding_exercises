#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
    
namespace c11 {
    template <typename ...>
    using void_t = void;
}

// primary template
template<typename, typename = void>
struct has_type_member: std::false_type {};
// partial specialization
template<typename T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};

// primary template 
template <typename T,  typename = void>
struct has_foo : std::false_type {};
// partial specialization 
template <typename T>
struct has_foo<T, std::void_t<decltype(
    std::declval<T&>().foo()
)>> : std::true_type {};

// primary template 
template <typename T, typename = void>
struct can_reference : std::false_type {};
// partial specialization
template <typename T>
struct can_reference<T, std::void_t<T&>> : std::true_type{};

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


