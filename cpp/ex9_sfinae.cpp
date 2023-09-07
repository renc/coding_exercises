#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

// SFINAE from C++11
// std::enable_if  , std::is_same_v
// https://eli.thegreenplace.net/2014/sfinae-and-enable_if/
template <typename T, typename std::enable_if_t<std::is_integral_v<T>, T>* = nullptr>
void do_stuff(const T& t)
{
    std::cout << "do_stuff integral type\n";
}

template <typename T, typename std::enable_if_t<std::is_class_v<T>, T>* = nullptr>
void do_stuff(const T& t)
{
    std::cout << "do_stuff class type\n";
}

template <typename T>
void do_stuff17(const T& t)
{
    if constexpr (std::is_integral_v<T>)
    {
        std::cout << "do_stuff17 integral type\n";
    }
    else if constexpr (std::is_class_v<T>)
    {
        std::cout << "do_stuff17 class type\n";
    }
}

// C++20 using concept
//template <template T>
//void do_stuff20

int main(int argc, char* argv[])
{
    int a = 10;
    std::string ss("abc");
    do_stuff<decltype(10)>(10); // (1)
    do_stuff<std::string>(std::string("abc")); // (2)
    // when (1), first overload works because std::enable_if<true, int>::type 
    // while second overload  std::enable_if<false, int> the Primary template does not have ::type, so fail

    do_stuff17(ss);
    do_stuff17(a);
    return 0;
}
