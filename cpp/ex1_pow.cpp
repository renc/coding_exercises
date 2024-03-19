#include <iostream>
#include <type_traits>

namespace npow {
template <size_t N>
constexpr int pow(int a)
{
    if constexpr (N == 0) { return 1; }
    int x = pow<N/2>(a);
    return (x * x * (N&1 ? a : 1));
}

template <typename T>
concept Number = std::is_floating_point_v<T> || std::is_integral_v<T>; 

template <int N, typename T>  // template<int N, Number T >
requires Number<T> 
constexpr auto const_pow_n(const T& v)
{
    static_assert(N >= 0, "N must be positive");
    auto product = T{1};
    for (int i = 0; i < N; ++i)
    {
        product *= v;
    }
    return product;
}

void test()
{
    static_assert(pow<0>(2) == 1);
    static_assert(pow<3>(2) == 8);
    static_assert(pow<4>(2) == 16);
    static_assert(const_pow_n<4>(2) == 16);
}
}

int main(int argc, char* argv[])
{
    npow::test();    

    return 0;
}