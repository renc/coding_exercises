#include <type_traits>


namespace ns20241126 {
// https://www.club.cc.cmu.edu/~ajo/disseminate/2017-02-15-Optional-From-Scratch.pdf
namespace native {

template <typename T>
struct  optional 
{
    T   m_val; 
    bool m_engaged; 
};
// this won't work, the whoe point is we don't want its default ctor to construct a T object,
// how can we get the size and aligned block of memory of T without actually constructing that T ?
} // end of namespace native 

namespace native2 {

template <typename T>
struct optional {
    std::aligned_storage_t<sizeof(T), alignof(T)> m_buf;
    bool m_engaged; 

    optional(): m_engaged(false) {}
    optional(const T& t) : m_engaged(true) { ::new ((void *)&m_buf) T(t); }

    ~optional() {
        if (m_engaged) { reinterpret_cast<T&>(m_buf).~T(); }
    }
}; // messy 
} // end of namespace native2

namespace native3 {

template <typename T>
struct optional {
    union { char m_dummy; T m_val; };
    bool m_engaged; 

    optional() : m_dummy(0), m_engaged(false) {}
    optional(const T& t) : m_val(t), m_engaged(true) {}
    ~optional() { // too bad, this is also no a trivaial descructor!
        if constexpr(!std::is_trivially_destructible_v<T>) {
            if (m_engaged) m_val.~T();
        }
    } 
}; 
} // end of namespace native3 

namespace native4 {

template <typename T, typename E = void>
struct optional_storage {
    union { char m_dummy; T m_val; }
    bool m_engaged;
    ~optional_storage() { 
        if (m_engaged) m_val.~(); // this destructor is not trival
    }
};

template <typename T> // partial specialization
struct optional_storage<T, std::enable_if_t<std::is_trivially_destructible_v<T>> {
    union { char m_dummy; T m_val; };
    bool m_engaged;

    ~optional_storage() = default; // this descructor is trivial  
};

template <typename T>
struct optional {
    optional_storage<T> m_storage;
    ~optional() = default; // this destructor is sometimes trivial;
};

} // end of namespace native4

} // end of namespace ns20241126