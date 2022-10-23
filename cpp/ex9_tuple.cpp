#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <atomic>
#include <tuple> 

namespace tuple_ex 
{
  
// std::tuple_cat which contatenate the values. 
// to contatenate the types: 
template <typename T1, typename T2> struct tuple_cat_helper;
template <typename... T1, typename... T2>
struct tuple_cat_helper<std::tuple<T1...>, std::tuple<T2...>>
{
    using type = std::tuple<T1..., T2...>;
};

//template<typename T1, typename T2>
//using tuple_cat_t = typename tuple_cat_helper<T1, T2>::type;
// another way to do the same:
//template<typename T1, typename T2>
//using tuple_cat_t = decltype(std::tuple_cat(std::declval<T1>(), std::declval<T2>()));
// even:
template<typename... Tuples>
using tuple_cat_t = decltype(std::tuple_cat(std::declval<Tuples>()...)); 

// select elements from the tuple
//https://devblogs.microsoft.com/oldnewthing/20200624-00/?p=103902
template<typename Tuple, std::size_t... Ints>
auto select_tuple(Tuple&& tuple, std::index_sequence<Ints...>)
{
    return std::tuple<std::tuple_element_t<Ints, Tuple>...> (
        std::get<Ints>(std::forward<Tuple>(tuple))...
    );
}
// or alternatively
//template<typename Tuple, std::size_t... Ints>
//std::tuple<std::tuple_element_t<Ints, Tuple>...> 
//select_tuple(Tuple&& tuple, std::index_sequence<Ints...>)
//{
//    return { std::get<Ints>(std::forward<Tuple>(tuple))... };
//}

// remove the last element (not remove the last type)
template<typename Tuple>
auto remove_last(Tuple&& tuple)
{
    constexpr auto size = std::tuple_size_v<Tuple>;
    using indices = std::make_index_sequence<size - 1>;
    return select_tuple(std::forward<Tuple>(tuple), indices{});
}

// to build offset sequence
template<std::size_t N, typename Seq> struct offset_sequence;
template<std::size_t N, std::size_t... Ints>
struct offset_sequence<N, std::index_sequence<Ints...>>
{
    using type = std::index_sequence<Ints + N...>;
};
template<std::size_t N, typename Seq>
using offset_sequence_t = typename offset_sequence<N, Seq>::type;
// example = index_sequence<3, 4, 5, 6>
// using example = offset_sequence_t<3, std::make_index_sequence<4>>;

template<typename Tuple>
auto remove_first(Tuple&& tuple)
{
    constexpr auto size = std::tuple_size_v<Tuple>;
    using indices = offset_sequence_t<1, 
                        std::make_index_sequence<size-1>>;
    return select_tuple(std::forward<Tuple>(tuple), indices{});
}
// N >= 1
template<std::size_t N, typename Tuple>
auto remove_Nth(Tuple&& tuple)
{
    constexpr auto size = std::tuple_size_v<Tuple>;
    using first = std::make_index_sequence<N>;// 0,1,2, .., n-2, n-1; 
    using rest = offset_sequence_t<N+1, std::make_index_sequence<size - N-1>>;
    return std::tuple_cat(select_tuple(tuple, first{}), select_tuple(tuple, rest{}));
}

template<typename Seq1, typename Seq> struct cat_sequence;

template<std::size_t... Ints1, std::size_t... Ints2>
struct cat_sequence<std::index_sequence<Ints1...>,
                    std::index_sequence<Ints2...>>
{
 using type = std::index_sequence<Ints1..., Ints2...>;
};
template<typename Seq1, typename Seq2>
using cat_sequence_t = typename cat_sequence<Seq1, Seq2>::type;

// example = index_sequence<3, 1, 4, 1, 5, 9>
//using example = cat_sequence_t<std::index_sequence<3, 1, 4>,
//                               std::index_sequence<1, 5, 9>>;

template<std::size_t N, typename Tuple>
auto remove_Nth_v2(Tuple&& tuple)
{
  constexpr auto size = std::tuple_size_v<Tuple>;
  using first = std::make_index_sequence<N>;
  using rest = offset_sequence_t<N+1,
                std::make_index_sequence<size-N-1>>;
  using indices = cat_sequence_t<first, rest>;
  return select_tuple(std::forward<Tuple>(tuple), indices{});
}

// https://devblogs.microsoft.com/oldnewthing/20200626-00/?p=103904

template<std::size_t F(std::size_t), typename Seq>
struct modify_sequence;

template<std::size_t F(std::size_t), std::size_t... Ints>
struct modify_sequence<F, std::index_sequence<Ints...>>
{
 using type = std::index_sequence<F(Ints)...>;
};

template<std::size_t F(std::size_t), typename Seq>
using modify_sequence_t = typename modify_sequence<F, Seq>::type;

template<int N>
constexpr std::size_t add_N(std::size_t v) { return v + N; }

// example = std::index_sequence<4, 2, 5>
//using example = modify_sequence_t<add_N<1>, std::index_sequence<3, 1, 4>>;

// example2 = std::index_sequence<2, 0, 3>
//using example2 = modify_sequence_t<add_N<-1>, std::index_sequence<3, 1, 4>>;



// Given a index, get the type of element from a tuple,  
// example = char
// using example = std::tuple_element_t<1, std::tuple<int, char, float>>;
// Given a index, get the value of element from a tuple, 

// Given a type, find the index for the element in a tuple 
template<typename T, typename Tuple>
struct tuple_element_index_helper;

template<typename T>
struct tuple_element_index_helper<T, std::tuple<>>
{
  static constexpr std::size_t value = 0;
};

template<typename T, typename... Rest>
struct tuple_element_index_helper<T, std::tuple<T, Rest...>>
{
  static constexpr std::size_t value = 0;
  using RestTuple = std::tuple<Rest...>;
  static_assert(
    tuple_element_index_helper<T, RestTuple>::value == 
    std::tuple_size_v<RestTuple>,
    "type appears more than once in tuple");
};

template<typename T, typename First, typename... Rest>
struct tuple_element_index_helper<T, std::tuple<First, Rest...>>
{
  using RestTuple = std::tuple<Rest...>;
  static constexpr std::size_t value = 1 +
       tuple_element_index_helper<T, RestTuple>::value;
};

template<typename T, typename Tuple>
struct tuple_element_index
{
  static constexpr std::size_t value =
    tuple_element_index_helper<T, Tuple>::value;
  static_assert(value < std::tuple_size_v<Tuple>,
                "type does not appear in tuple");
};

template<typename T, typename Tuple>
inline constexpr std::size_t tuple_element_index_v
 = tuple_element_index<T, Tuple>::value;

// index = 1
//constexpr std::size_t index = tuple_element_index_v<int, std::tuple<char, int, float>>;

// error: type does not appear in tuple
//constexpr std::size_t index = tuple_element_index_v<double, std::tuple<char, int, float>>;

// error: type appears more than once in tuple
//constexpr std::size_t index = tuple_element_index_v<int, std::tuple<char, int, int>>;

// https://www.cppstories.com/2022/tuple-iteration-basics/
namespace iter_basics
{
template <typename TupleT, std::size_t... Is>
std::ostream& printTupleImp(std::ostream& os, const TupleT& tp, std::index_sequence<Is...>) {
	auto printElem = [&os](const auto& x, std::size_t id) {
    	if (id > 0) os << ", ";
        os << id << ": " << x;
    }; 
    
    os << "(";
    (printElem(std::get<Is>(tp), Is), ...);
    os << ")";
    
    return os;
}

template<typename TupleT, std::size_t TupleSize = std::tuple_size_v<TupleT>>
std::ostream& operator<<(std::ostream& os, const TupleT& tp) {
	return printTupleImp(os, tp, std::make_index_sequence<TupleSize>{});
}
void test()
{
  std::tuple tp{'c', 1, 2.3, "hello", std::string("world")};
  std::cout << tp << std::endl;
}
} // end of namespace iter_basics 

namespace iter_apply {
// https://www.cppstories.com/2022/tuple-iteration-apply/

} // end of namespace iter_apply

namespace unique {
// method 1.  https://stackoverflow.com/questions/55941964/how-to-filter-duplicate-types-from-tuple-c 
// decompose the tuple into the first and the rest, 
template < typename T, typename ...Rest >
constexpr auto make_unique_tuple( std::tuple< T, Rest... > )
{
    if constexpr ( ( std::is_same_v< T, Rest > || ... ) )
    {
        return make_unique_tuple( std::tuple< Rest... >{} );
    }
    else
    {
        if constexpr ( sizeof...( Rest ) > 0 )
        {
            using remaining = decltype( make_unique_tuple( std::tuple< Rest... >{} ) );
            return std::tuple_cat( std::tuple< T >{}, remaining{} );
        }
        else
        {
            return std::tuple< T >{};
        }
    }
}
// method 2
template <typename T, typename... Ts>
struct unique : std::type_identity<T> {};

template <typename... Ts, typename U, typename... Us>
struct unique<std::tuple<Ts...>, U, Us...>
    : std::conditional_t<(std::is_same_v<U, Ts> || ...)
                       , unique<std::tuple<Ts...>, Us...>
                       , unique<std::tuple<Ts..., U>, Us...>> {};

template <typename... Ts>
using unique_tuple = typename unique<std::tuple<>, Ts...>::type;
// start with the empty one, and add one by one 

template <typename T> void TD() { std::cout << __PRETTY_FUNCTION__ << std::endl;  }

void test()
{
    TD< decltype( make_unique_tuple( std::tuple< int, char, int,double >{} ) ) >();
    TD<unique_tuple<int, char, int, double>>();
}
	
} // end of namespace unique 

} // end of namespace tuple_ex
  
int main()
{
	tuple_ex::iter_basics::test();
    return 0; 
}
