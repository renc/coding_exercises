#include <memory>
#include <type_traits>
#include <utility>

#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <memory>
#include <functional> 
namespace stl_mock {
template <typename _Tp, typename _Dp>
class unique_ptr_imp {
public:
    template<typename _Up, typename _Ep, typename = void>
    struct _Ptr { using type = _Up*; };

    using pointer = typename _Ptr<_Tp, _Dp>::type; // = _Tp* 
private:
    std::tuple<pointer, _Dp> _M_t;  // == std::tuple<_Tp*, _Dp> _M_t; 
}; 
template <typename _Tp, typename _Dp = std::default_delete<_Tp>>
class unique_ptr {
    unique_ptr_imp<_Tp, _Dp> _M_t;
}; 
static_assert(sizeof(std::tuple<int*, std::default_delete<int>>) == 8);
static_assert(sizeof(unique_ptr_imp<int, std::default_delete<int>>) == 8);
static_assert(sizeof(unique_ptr<int>) == 8);
// my test of sizeof functor == lambda > fuction pointer >  std::function 
// https://www.nextptr.com/question/qa1366990479/unique_ptr-with-custom-deleter according to this
// they will result in different size 
// functor as custom deleter
struct Obj {
    void operator()() {};
};
static_assert(sizeof(std::tuple<int*, Obj>) == 8);
struct IntDelete { void operator() (int*p) { delete p; } };
static_assert(sizeof(std::tuple<int*, IntDelete>) == 8);
// lambda type as custom deleter , https://fekir.info/post/decltype-lambda/
auto lambda_deleter = [](int*p) { delete p; };
static_assert(sizeof(std::tuple<int*, decltype(lambda_deleter)> ) == 8);
// function pointer
using func_ptr_del = void(*)(int*); 
static_assert(sizeof(std::tuple<int*, func_ptr_del>) == 16);
static_assert(sizeof(std::tuple<int*, std::function<void(int*)>>) == 40);
} // end namespace stl_mock 

int main()
{
    std::tuple<int *, char> t1 { new int(1), 'a' };
    std::pair <int *, char> t2 { new int(2), 'b' };
    std::cout << "t1 {" << std::get<0>(t1) << ", " << std::get<1>(t1) 
              << "}, sz=" << sizeof(t1) << ".\n";
    std::cout << "t2 {" << std::get<0>(t2) << ", " << std::get<1>(t2) 
              << "}, sz=" << sizeof(t1) << ", raw pointer=" << sizeof(int*) << ".\n";
 
    std::unique_ptr<int> p1 = std::make_unique<int>(5);
    std::cout << "p1 sz=" << sizeof(p1) << ".\n"; 
    return 0;
}
namespace abc {

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    // new T(std::forward<Args1, Args2, Args3, ...>(args1, args2, args3, ...)), wrong
}

} // end namespace abc 

namespace abc2 {
    // renc, tag dispatch ?
template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args)
{ 
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}