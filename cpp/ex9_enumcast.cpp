#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
    
enum E1 { apple = 10, banana };
enum class E2 : std::uint32_t { cat = 20, dog };

namespace c17 {
template <typename T>
void SetValue(const T& value)
{
    if constexpr(true == std::is_enum_v<T>) {
        std::cout << "E.value=" << static_cast<std::underlying_type_t<T>>(value) << std::endl;
    }
    else {
        std::cout << "value=" << value << std::endl;
    }
}
}

namespace c11a {  
template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
void SetValue(const T &t)
{
    std::cout << "E.value=" << static_cast<std::underlying_type_t<T>>(t) << std::endl;
} 
template <typename T, typename = std::enable_if_t<!std::is_enum_v<T>>
    , typename = int>
void SetValue(const T &t)
{
    std::cout << "value=" << static_cast<T>(t) << std::endl;
}
// why need typename =int, p473 of <C++ templates>
}

namespace c11b {
template<typename T, std::enable_if_t<std::is_enum_v<T> == false, int> = 0>
void SetValue(const T& value)
{
    std::cout << "value=" << value << std::endl;
}
template<typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
void SetValue(const T& value)
{
    std::cout << "E.value=" << static_cast<std::underlying_type_t<T>>(value) << std::endl;
}
}

namespace c11c {
template <typename T, typename = void>
struct native_type
{
    using type = T;
};
template <typename T>
struct native_type<T, std::enable_if_t<std::is_enum_v<T>>>
{
    using type = std::underlying_type<T>::type;
};
template <typename T>
using native_type_t = native_type<T>::type;

template <typename T>
auto Enum_Cast(const T& value)
{
    return static_cast<native_type_t<T>>(value);
}

template <typename T>
void SetValue(const T& value)
{
    std::cout << Enum_Cast(value) << std::endl;
}
}

int main(int argc, char **argv)
{
    c17::SetValue(99);
    c17::SetValue(E1::banana);
    c17::SetValue(E2::cat);
    c11a::SetValue(99);
    c11a::SetValue(E1::banana);
    c11a::SetValue(E2::cat);
    c11b::SetValue(99);
    c11b::SetValue(E1::banana);
    c11b::SetValue(E2::cat);
    c11c::SetValue(99);
    c11c::SetValue(E1::banana);
    c11c::SetValue(E2::cat);
    return 1;
}


