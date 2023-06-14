#include <string>
#include <vector>
#include <iostream>
#include <cctype>
#include <type_traits>
 
namespace ss {
template <int ... Is> struct sequence
{
  static constexpr auto size = sizeof ...(Is); // added by me just for try
};// as a container

// auxiliary meta-function for making N+1 sized sequence from an N sized sequence
template <typename T> struct append;
template <int... Is> 
struct append<sequence<Is...>>
{
    using type = sequence<Is..., sizeof...(Is)>;
};

// recursive implementation of make_sequence
template <int I> struct make_sequence_;
template <int I> using make_sequence = typename make_sequence_<I>::type; 
template <> struct make_sequence_<0> { using type = sequence<>; }; // recursion end
template <int I> struct make_sequence_ : append< make_sequence<I-1> >
{
    static_assert(I >= 0, "negative size");
};
void test()
{
    //static_assert( std::is_same_v<make_sequence<0>, sequence<> >, "" );
    //static_assert( std::is_same_v<make_sequence<1>, sequence<0> >, "" );
    static_assert( std::is_same_v<make_sequence<2>, sequence<0, 1> >, "");
    //static_assert( std::is_same_v<make_sequence<3>, sequence<0, 1, 2> >, "");
  
  	static_assert( make_sequence<2>::size == 2, "");
}
}

int main() {
    ss::test();
	return 0;
}
