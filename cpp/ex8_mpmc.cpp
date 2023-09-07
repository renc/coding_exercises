#include <array>
#include <thread> // std::mutex
#include <mutex>
#include <optional> 
// #include <semaphore>// C++20

namespace cpphp {
// page 360
template <typename T, size_t N>
class BoundedBuffer_Mtx 
{
private:
    std::array<T, N> buffer_{};
    size_t read_pos_ = {0}; // point to the element can be read;
    sizt_t write_pos_= {0}; // point to the empty slot can be write;
    std::mutex m_; 

    void do_push(auto&& item)
    {
        // missing: should block (or return false) if buffer is full
        std::unique_lock<std::mutex> lc(m_);
        buffer_[write_pos_] = std::forward<decltype(item)>(item);
        write_pos_ = (write_pos_ + 1) %N; 
    }
public:
    void push(const T& item) { do_push(item); };
    void push(T&& item) { do_push(item); }
    std::optional<T> pop() 
    {
        // missing: should block (or return false) if buffer is empty
        std::optional<T> ret;
        {
            std::unique_lock<std::mutex> lc(m_);
            ret = buffer_[read_pos_];
            read_pos_ = (read_pos_ + 1) % N; 
        }
        return ret; 
    }
}; // end of 

// page 361, using two std::counting_semaphone<N> 
template <typename T, size_t N>
class BoundedBuffer_Sem {

}; 
}
