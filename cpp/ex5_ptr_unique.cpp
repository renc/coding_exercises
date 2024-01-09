#include <memory>
#include <type_traits>
#include <utility>

namespace abc {

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    // new T(std::forward<Args1, Args2, Args3, ...>(args1, args2, args3, ...)), wrong
}

}

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
    static_assert()
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}