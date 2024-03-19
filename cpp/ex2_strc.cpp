#include <iostream>
#include <type_traits>
#include <array> // std::array
#include <algorithm> // for std::copy_n
#include <cstdint> // for std::uint32_t 

// compile time string 
// concate +, substring, find, 

constexpr std::uint32_t hash(const char* str)
{
    std::uint32_t v = 15;
    while (char c = *str++)
    {
        v = (v << 2) + c; 
    }
    return v;
    // char is 1 byte (8 bits)
}


namespace strtools {
// https://yongweiwu.wordpress.com/2022/06/19/compile-time-strings/
constexpr size_t my_str_len(const char* str)
{
    size_t len = 0; 
    while ( *(str+len) != '\0') 
        ++ len;
    return len;
}
constexpr size_t length(const char* str)
{
    return std::char_traits<char>::length(str);
}
constexpr size_t length(std::string_view sv)
{
    return sv.size();
}

constexpr const char* find(const char*str, char ch)
{
    return std::char_traits<char>::find(str, length(str), ch);
}
constexpr std::string_view::size_type find(std::string_view sv, char ch)
{
    return sv.find(sv);
}

template <size_t Count>
constexpr auto copy_str(const char* str)
{
    std::array<char, Count+1> result{};
    for (size_t i=0; i < Count; ++i) {
        result[i] = str[i];
    }
    return result;
}

// how to turn strings into types for compile time argument passing
// pre C++20, this is extension but not accepted by standards committee
template <char ... Cs>
struct compile_time_string_pre20 {
    static constexpr char value[] {Cs..., '\0'};
};
template <typename T, T... Cs>
constexpr compile_time_string_pre20<Cs...> operator""_ctsPre20()
{
    return {};
}
// the c++20 approach 
template <size_t N>
struct compile_time_string {
    constexpr compile_time_string(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }
    char value[N]{};
};
template <compile_time_string cts>
constexpr auto operator""_cts()
{
    return cts;
}
template <compile_time_string cts>
struct cts_wrapper {
    static constexpr compile_time_string str{cts};
};

void test()
{
    using namespace std::literals;
 
    static_assert(length("hello world") == 11); 
    constexpr auto s1 = "Hello Wrold"sv;
    static_assert(length(s1) == 11);

    {
    constexpr auto a = "Hi"_ctsPre20;
    constexpr auto b = "Hi"_ctsPre20;
    constexpr auto c = "hi"_ctsPre20;
    static_assert(std::is_same_v<decltype(a), decltype(b)>);
    static_assert(std::is_same_v<decltype(a), decltype(c)> == false);
    }
    {
    constexpr auto a = "Hi"_cts;
    constexpr auto b = "Hi"_cts;
    constexpr auto c = "hi"_cts;
    constexpr auto d = "ha"_cts;
    static_assert(std::is_same_v<decltype(a), decltype(b)>);
    static_assert(std::is_same_v<decltype(a), decltype(c)>);// not expected
    static_assert(std::is_same_v<decltype(a), decltype(d)>);// not expected
    // check cppinsighs for why a==d
    }
    {
        auto a = cts_wrapper<"Hi">{};
        auto b = cts_wrapper<"Hi">{};
        auto c = cts_wrapper<"hi">{};
        static_assert(std::is_same_v<decltype(a), decltype(b)>);
        static_assert(std::is_same_v<decltype(a), decltype(c)> == false);
    }
} 
}

int main(int argc, char* argv[])
{
    std::cout << hash("hello") << "\n";
    std::cout << hash("world") << "\n";
}
