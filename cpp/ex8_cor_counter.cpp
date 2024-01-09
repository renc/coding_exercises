#include <iostream>
#include <coroutine>
#include <utility> // std::exchange()

namespace pc20::generator {
// 2023/11, example from book: programming with c++20, Andreas Fertig, chapter 2, coroutines 

template <typename T, typename G>
struct promise_type_base {
    T mValue; // the value yielded or returned from a corountne

    auto yield_value(T value) // Invoked by co_yield or co_return
    {
        mValue = std::move(value); // store the yielded value for access outside the corountine
        return std::suspend_always{}; // suspend the coroutine here
    }
    G get_return_object() { return G{this}; } // return generator
    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void                return_void() {}
    void                unhandled_exception() { std::terminate(); }
    static auto         get_return_object_on_allocation_failure() { return G{nullptr}; }
};

namespace coro_iterator {
template <typename PT>
struct iterator {
    std::coroutine_handle<PT> mCoroHdl{nullptr};
    iterator() = default;
    iterator(std::coroutine_handle<PT> hco) : mCoroHdl{hco} { resume(); }
    void resume() { if (not mCoroHdl.done()) { mCoroHdl.resume(); } }
    void operator++() { resume(); }
    bool operator==(const iterator&) const { return mCoroHdl.done(); }
    const auto& operator*() const { return mCoroHdl.promise().mValue; }    
}; // iterator is only allowed to have a temporary view of the data
}; // 

template <typename T>
struct generator {
    using promise_type = promise_type_base<T, generator>; // compiler looks for this, as a state controller of a coroutine 
    using PromiseTypeHandle = std::coroutine_handle<promise_type>; // wrapper around a ptr, pointing to the coroutine frame

    // make the generator iterable 
    using iterator = coro_iterator::iterator<promise_type>;
    iterator begin() { return {mCoroHdl}; }
    iterator end()   { return {}; }

    generator(generator const&) = delete;
    generator(generator&& rhs) : mCoroHdl(std::exchange(rhs.mCoroHdl, nullptr)) {} // move only, since coroutine frame has memory resource
    ~generator() { if (mCoroHdl) { mCoroHdl.destroy(); } } // we have to maintain the life-time of this coroutine, release the coroutine frame memory resource

private:
    friend promise_type; // as the default ctor is private, promise_type needs to be a friend
    explicit generator(promise_type* p) : mCoroHdl{PromiseTypeHandle::from_promise(*p)} {}

    PromiseTypeHandle mCoroHdl; // the coroutine handle
};

using IntGenerator = generator<int>;
void UseCounterValue(int a) { std::cout << "UseCounterValue a=" << a << "\n"; }

// returning a coroutine object 
IntGenerator counter(int start, int end)
{
    while (start < end) {
        co_yield start; // suspending and yielding a value and giving control back to the caller
        ++ start;
    }
} // whenever we use (co_yield, co_wait, co_return) keyword in a func, this func automatically becomes a coroutine.

void UseCounter()
{
    auto g = counter(0, 5);
    // this sequence runs from 1 to 5
    for (auto i: g) { UseCounterValue(i); }
}
} // end of namespace 
namespace pc20::parser {
// https://andreasfertig.blog/2021/02/cpp20-a-coroutine-based-stream-parser/    
static const std::byte ESC{'H'};
static const std::byte SOF{0x10};

template <typename T>
void ProcessNextByte(std::byte b, T&& frameCompleted)
{
    static std::string frame{};
    static bool inHeader{false};
    static bool wasESC{false};
    static bool lookingForSOF{false};

    if (inHeader) {
        x 
    }
}

} // end of namespace 