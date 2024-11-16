/*
https://leetcode.com/discuss/interview-question/3349116/Squarepoint-Capital-or-Bangalore-or-OA-or-Cpp
i love this interview question, it invokes so many topics
*/
#include <type_traits>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <future>

template <typename T, typename Op>
class accumulator {
public:
    accumulator(T init, Op op);
    template <typename U>
    void generic_push(U&& value);
    void push(T value);
    T finalize();
private:
    struct pimpl;
    std::unique_ptr<pimpl> pimpl_;    
};
// you can toggle these boolens to enable only some checks
// this can be useful if some of them do not compie
constexpr bool check_001_enabled = true;
constexpr bool check_002_enabled = true;
constexpr bool check_003_enabled = true; // turns out need to enabe one-by-one, it does not check the the code when false;
constexpr bool check_004_enabled = true;
constexpr bool check_005_enabled = true;
constexpr bool check_006_enabled = true;
constexpr bool check_007_enabled = true;
constexpr bool check_008_enabled = true;
constexpr bool check_009_enabled = true;

template <typename T>
struct strong { T value; };
class latch {
public:
    explicit latch(int count) : count_(count) {}
    void arrive_and_wait() {
        std::unique_lock lock(mutex_);
        --count_;
        cv_.notify_all();
        cv_.wait(lock, [this] { return count_ == 0; });// wait if not ture, wait until it is true
    }
private:
    int count_{};
    std::mutex mutex_;
    std::condition_variable cv_;
};
template <typename T>
struct move_only_plus : std::plus<T> {
    using std::plus<T>::plus;
    move_only_plus(move_only_plus const &) = delete;
    move_only_plus& operator=(move_only_plus const &) = delete;
    move_only_plus(move_only_plus&&) = default;
    move_only_plus& operator=(move_only_plus &&) = default;
    using std::plus<T>::operator(); 
};
// basic usage
bool check_001() {
    if constexpr (check_001_enabled) {
        auto acc = accumulator(10, std::plus<int>());
        acc.push(1);
        acc.push(2);
        acc.push(3);
        return acc.finalize() == 16; // renc, compare 002, this push can do lazy eval
    } else return false;
}
// eager evaluation 
bool check_002() {
    if constexpr (check_002_enabled) {
        auto invoke = 0; 
        auto acc= accumulator(10, [&](auto lhs, auto rhs) {
            ++invoke;
            return lhs + rhs;
        });
        acc.push(1); if (invoke != 1) return false; // renc, mean each push need to eval
        acc.push(2); if (invoke != 2) return false;
        acc.push(3); if (invoke != 3) return false;
        return acc.finalize() == 16;
    } else return false;
}
// T is move-only
bool check_003() {
    if constexpr(check_003_enabled) {
        auto acc = accumulator(std::make_unique<int>(10), 
            [](auto lhs, auto rhs) { return std::make_unique<int>(*lhs + *rhs);});
        acc.push(std::make_unique<int>(1));
        acc.push(std::make_unique<int>(2));
        acc.push(std::make_unique<int>(3));
        return *acc.finalize() == 16;
    } else return false;
}
// Op is move-only
bool check_004() {
    if constexpr (check_004_enabled) {
        auto acc = accumulator(10, move_only_plus<int>());
        acc.push(1); acc.push(2); acc.push(3);
        return acc.finalize() == 16;
    } else return false;
}
// stateful Op
bool check_005() {
    if constexpr (check_005_enabled) {
        constexpr double eps = 1e-6;
        auto acc = accumulator(0.0, [count=0, sum=0.0](auto _, auto rhs) mutable {
            sum += rhs;
            count ++;
            return sum/count;
        });
        acc.push(0); acc.push(2); acc.push(4); acc.push(4);
        return std::abs(acc.finalize() - 2.5) < eps;
    } else return false;
}
// thread safety
bool check_006() {
    if constexpr (check_006_enabled) {
        constexpr auto nthread = 10; 
        constexpr auto op_per_thread = 100000;
        latch latch(nthread);
        std::vector<std::future<void>> futures;
        auto acc = accumulator(0, std::plus<int>());
        for (auto i = 0; i < nthread; ++i) {
            futures.push_back(std::async(std::launch::async, [&] {
                latch.arrive_and_wait();
                for (auto j = 0; j < op_per_thread; ++j) {
                    acc.push(1);
                }
            }));
        }
        futures.clear();
        return acc.finalize() == op_per_thread * nthread;
    } else return false;
}
// generic_push 
bool check_007() {
    if constexpr (check_007_enabled) {
        auto acc = accumulator(10, [](auto lhs,auto rhs) { return lhs + rhs.value; });
        acc.generic_push(strong<int>{1});
        acc.generic_push(strong<int>{2});
        acc.generic_push(strong<int>{3});
        return acc.finalize() == 16;
    } else return false;
}
// generic_push with move-only U
bool check_008() {
    if constexpr (check_008_enabled) {
        auto acc = accumulator(10, [](auto lhs, auto rhs) { return lhs + *rhs; });
        acc.generic_push(std::make_unique<int>(1));
        acc.generic_push(std::make_unique<int>(2));
        acc.generic_push(std::make_unique<int>(3));
        return acc.finalize() == 16; 
    } else return false;
}
// generic_push does not steal ownership
bool check_009() {
    if constexpr (check_009_enabled) {
        auto acc = accumulator(10, [](auto lhs, auto rhs) { return lhs + *rhs; });
        auto v1 = std::make_shared<int>(1);
        auto v2 = std::make_shared<int>(2);
        auto v3 = std::make_shared<int>(3);
        acc.generic_push(v1);
        acc.generic_push(v2);
        acc.generic_push(v3);
        return acc.finalize() == 16 && v1 && v2 && v3;
    } else return false;
}
int main()
{
    std::unordered_map<std::string, std::function<bool()>> checks = {
        {"check_001", [] { return check_001(); }},
        {"check_002", [] { return check_002(); }},
        {"check_003", [] { return check_003(); }},
        {"check_004", [] { return check_004(); }},
        {"check_005", [] { return check_005(); }},
        {"check_006", [] { return check_006(); }},
        {"check_007", [] { return check_007(); }},
        {"check_008", [] { return check_008(); }},
        {"check_009", [] { return check_009(); }}
    };
    std::string check;
    std::cin >> check;
    auto success = checks.at(check)();
    std::cout << check << " - " << (success ? "OK" : "KO") << std::endl;
    return 0;
}

// Get this error, when pimp is not defined, even defined as empty {} will fix it.
// ex9_accum.cpp:17:7:   required from here
// /usr/include/c++/13/bits/unique_ptr.h:97:23: error: invalid application of ‘sizeof’ to incomplete type ‘accumulator<int, std::plus<int> >::pimpl’
//    97 |         static_assert(sizeof(_Tp)>0,
//       |                       ^~~~~~~~~~~

template <typename T>
struct is_smart_pointer : std::false_type {};

template <typename T>
struct is_smart_pointer<std::unique_ptr<T> > : public std::false_type {};
template <typename T>
struct is_smart_pointer<std::shared_ptr<T> > : public std::true_type {};
template <typename T>
const bool is_smart_pointer_v = is_smart_pointer<T>::value;

template<typename T, typename Op>
struct accumulator<T, Op>::pimpl {
    pimpl(T init, Op op) : result_(std::move(init)), op_(std::move(op)) 
    {
        //if constexpr (std::is_scalar_v<T> == true)
        //   std::cout << "pimp  ctr, r=" << result_ << "\n";
        //else std::cout << "pimp  ctr, r=" << *result_ << "\n";
    }
    void push(T value) {
        std::scoped_lock lck{mtx_};
        result_ = std::move( op_(std::move(result_), std::move(value)) );
        //if constexpr (std::is_scalar_v<T> == true)
        //    std::cout << "pimp push, r=" << result_ << "\n";
        //else std::cout << "pimp push, r=" << *result_ << "\n";
    }
    template <typename U>
    void generic_push(U&& u) {
        auto temp =  op_(std::move(result_), std::forward<U>(u));
        result_ = temp;
        //if constexpr (std::is_scalar_v<T> == true)
        //    std::cout << "pimp push, r=" << result_ << "\n";
        //else std::cout << "pimp push, r=" << *result_ << "\n";
    }
    T finalize() {
        if constexpr (std::is_scalar_v<T> == true)
            std::cout << "pimp finl, r=" << result_ << "\n";
        else std::cout << "pimp finl, r=" << *result_ << "\n";
        return std::move(result_);
    }
    T result_{};
    Op op_;
    std::mutex mtx_;
};

template<typename T, typename Op>
accumulator<T, Op>::accumulator(T init, Op op)
{
    pimpl_ = std::make_unique<pimpl>(std::move(init), std::move(op));
}

template<typename T, typename Op>
template <typename U>
void accumulator<T, Op>::generic_push(U&& value)
{
    pimpl_->generic_push(std::forward<U>(value));
}

template<typename T, typename Op>
void accumulator<T, Op>::push(T value)
{
    pimpl_->push(std::move(value));
}

template<typename T, typename Op>
T accumulator<T, Op>::finalize()
{
    return pimpl_->finalize();
}