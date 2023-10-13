#include <array>
#include <type_traits>
#include <iostream>
#include <cstdint> // for std::uint32_t 

// compile time string 

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

int main(int argc, char* argv[])
{
    std::cout << hash("hello") << "\n";
    std::cout << hash("world") << "\n";
}
