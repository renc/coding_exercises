// custom spsp queue, single producer single consumer
//
// The real trick behind lock less spsc circular queue is
// push_back() is called from producer only, only one producer (not from consumer)
//      so the writer index is only updated by 1 producer(1thread )
// pop_front() is called from consumer only, only one consumer (not from producer)
//      so the reader index is only updated by 1 consumer(the other 1 thread)
// producer: load reader index value for checking whether there is empty slot, store writer index at last;
// consumer: load writer index value for checking whether there is element to read, store reader index at last;
// https://stackoverflow.com/questions/14142023/why-is-a-single-producer-single-consumer-circular-queue-thread-safe-without-lock


#include <atomic>
#include <cstddef>
#include <utility> // std::optional
#include <array>
#include <vector>

namespace cpphp {
// from book 2020 C++ high performance by Bjorn Andrist ... page 378
// [-][-][-][-]  init four empty slots
// r/w    size=0
// [*][-][-][-], after 1 push
// r   w    size=1
// [-][-][-][-], after 1 pop
//    r/w   size=0 
// [-][*][*][*], after 3 push
//  w  r    size=3 
// [*][*][*][*], after 1 push
//    r/w   size=4 full, cann't push/write again 
// as we can see, empty/full are r==w, so it need a size variable to diff
template <class T, size_t N>
class LockFreeQueue {
    std::array<T, N> buffer_{};   // used by both threads
    std::atomic<size_t> size_{0}; // used by both threads
    static_assert(std::atomic<size_t>::is_always_lock_free);
    size_t read_pos_{0}; // used by reader thread
    size_t write_pos_{0}; // used by writer thread

    bool do_push(auto&& t) { // helper func
        if (size_.load() == N) return false; // full , this is sequencial consistent memory order
        buffer_[write_pos_] = std::forward<decltype(t)>(t); // do this writting before updating the index
        write_pos_ = (write_pos_ + 1) % N;
        size_.fetch_add(1); // size_.store(size_ + 1); ok ? do the actually writting ahead
    }
public:
    // write thread
    bool push(const T& t) { return do_push(t);}
    bool push(T&& t) { return do_push(t); }
    // reader thread
    std::optional<T> pop() {
        auto val = std::optional<T>{};
        if (size_t.load() > 0)
        {
            val = std::move(buffer_[read_pos_]); // renc: no need of that elem in the buf 
            read_pos_ = (read_pos_+1)%N;
            size_t.fetch_sub(1); // do the actually reading ahead.
        }
        return val; 
    } // size_ .fetch_add/fetch_sub/load func use the default memory order(std::memory_order_seq_cst), 
    // it is too complex for me to use other memory order explicitly, so let me stick with the sequentially consistent

    // both threads can call this
    auto size() const noexcept { return size_.load(); }
}; 
} // end of namespace cpphp 

namespace timur {
// ACCC 2017 lock-free programming modern c++ from Timur Doumler who works on audio processing
// [-][-][-][-]  init four empty slots
// r/w     
// [*][-][-][-], after 1 push
// r   w    size=1
// [-][-][-][-], after 1 pop
//    r/w   size=0 
// [-][*][*][*], after 3 push
//  w  r    size=3 
// [-][*][*][*], try 1 push, fail since w+1 == r
//  w  r    size=3 here means full, cann't push/write again 
// as we can see, without size variable, waste one slot
template <typename T, size_t size>
class LockFreeQueue
{
public:
    bool push(const T& newElement)
    {
        auto oldWritePos = writePos.load(); 
        auto newWritePos = getPositionAfter(oldWritePos);
        if (newWritePos == readPos.load()) // between write.load() and this, there might be pop already
            return false; // full.         // but we don't care, just return false, let caller try again
        
        ringBuffer[oldWritePos] = newElement;
        // only write thread is updating the writePos
        writePos.store(getPositionAfter(oldWritePos));//why not use newWritePos since only one writer?
        return false;
    }
    bool pop(T& returnedElement)
    {
        auto oldWritePos = writePos.load();
        auto oldReadPos = readPos.load();
        if (oldWritePos == oldReadPos)  // between writePos.load() and this, there might be new write/push already, 
            return false; // empty      // we do not care, just return false let caller try pop again
        // only read thread is updating the readPos 
        returnedElement = std::move(ringBuffer[oldReadPos]);
        readPos.store(getPositionAfter(oldReadPos));
        return true;
    }
private:
  static constexpr size_t getPositionAfter(size_t pos) noexcept
  {
      return ++pos == rightBufferSize ? 0 : pos;
  }
    static_constexpr size_t ringBufferSize = size + 1; // one extra for diff full (w+1==r) from empty (r==w);
    std::array<T, ringBufferSize> ringBuffer;
    std::atomic<size_t> readPos = {0}, writePos = {0};
};

} // end of namespace timer
// folly spsc <-- read this too, it is similar https://github.com/facebook/folly/blob/main/folly/ProducerConsumerQueue.h


namespace kjel {
// renc: // write pos : point to last element +1.  read pos: point to the current elem can be read.
// renc: this implementation use sequential consistent memory order
// there is other memory order (release-acquire, relaxed) but more complexed.
namespace memory_sequential_consistent {
template<typename Element, size_t Size> 
class CircularFifo{
public:
  enum { Capacity = Size+1 };

  CircularFifo() : _tail(0), _head(0){}   
  virtual ~CircularFifo() {}

  bool push(const Element& item); // pushByMOve?
  bool pop(Element& item);

  bool wasEmpty() const;
  bool wasFull() const;
  bool isLockFree() const;

private:
  size_t increment(size_t idx) const; 

  std::atomic <size_t>  _tail; // tail(input) index, writer index 
  Element    _array[Capacity]; // renc, not sure why not Element *_array;  
  std::atomic<size_t>   _head; // head(output) index, reader index 
};


// Here with memory_order_seq_cst for every operation. This is overkill but easy to reason about
//
// Push on tail. TailHead is only changed by producer and can be safely loaded using memory_order_relexed
//         head is updated by consumer and must be loaded using at least memory_order_acquire
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::push(const Element& item)
{	
  const auto current_tail = _tail.load(); 
  const auto next_tail = increment(current_tail); 
  if(next_tail != _head.load())                         
  {
    _array[current_tail] = item;
    _tail.store(next_tail); 
    return true;
  }
  
  return false;  // full queue
}


// Pop by Consumer can only update the head 
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::pop(Element& item)
{
  const auto current_head = _head.load();  
  if(current_head == _tail.load())  
    return false;   // empty queue

  item = _array[current_head]; 
  _head.store(increment(current_head)); 
  return true;
}

// snapshot with acceptance of that this comparison function is not atomic
// (*) Used by clients or test, since pop() avoid double load overhead by not
// using wasEmpty()
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::wasEmpty() const
{
  return (_head.load() == _tail.load());
}

// snapshot with acceptance that this comparison is not atomic
// (*) Used by clients or test, since push() avoid double load overhead by not
// using wasFull()
template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::wasFull() const
{
  const auto next_tail = increment(_tail.load());
  return (next_tail == _head.load());
}


template<typename Element, size_t Size>
bool CircularFifo<Element, Size>::isLockFree() const
{
  return (_tail.is_lock_free() && _head.is_lock_free());
}

template<typename Element, size_t Size>
size_t CircularFifo<Element, Size>::increment(size_t idx) const
{
  return (idx + 1) % Capacity;
}


} // sequential_consistent
// https://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular
}

namespace seva {
// renc: queue_mutex vs queue_spsc 
// renc: write pos : point to last element +1.  read pos: point to the current elem can be read.
template <typename T>
class queue_mutex
{
public:
    queue_mutex(int max_capacity)
        : mBuffer(max_capacity + 1)
        , mReadPos(0)
        , mWritePos(0)
    {
        assert(max_capacity > 0);
        // Protect from integer overflow
        assert(max_capacity + 1 > 0);
    }

    bool push_back(const T &item)
    {
        std::lock_guard<std::mutex> guard(mLock);
        const int r = mReadPos;
        const int w = mWritePos;

        // Assuming we write, where will we move next?
        const int next_w = increment(w);

        // The two pointers colliding means we would have exceeeded the
        // queue size and create an ambiguous state with being empty..
        if (r == next_w)
            return false;

        mBuffer[w] = item;
        mWritePos = next_w;
        return true;
    }

    bool pop_front(T &item)
    {
        std::lock_guard<std::mutex> guard(mLock);
        int r = mReadPos;
        int w = mWritePos;
        // Queue is empty (or was empty when we checked)
        if (r == w)
            return false;

        item = mBuffer[r];
        mReadPos = increment(r);
        return true;
    }

private:
    inline int increment(int pos) const
    {
        return (pos + 1) % int(mBuffer.size());
    }

    std::vector<T> mBuffer;
    int mReadPos;
    int mWritePos;
    std::mutex mLock;
};
template <typename T>
class queue_spsc
{
public:
	queue_spsc(int max_capacity)
		: mBuffer(max_capacity + 1)
		, mReadPos(0), mWritePos(0)
	{
		assert(max_capacity > 0);
		// Protect from integer overflow
		assert(max_capacity + 1 > 0);
	}

	bool push_back(const T &item)
	{
		const int r = mReadPos.load(std::memory_order_consume);
		const int w = mWritePos.load(std::memory_order_consume);

		// Assuming we write, where will we move next?
		const int next_w = increment(w);

		// The two pointers colliding means we would have exceeeded the
		// queue size and create an ambiguous state with being empty..
		if (r == next_w)
			return false;

		mBuffer[w] = item;
		mWritePos.store(next_w, std::memory_order_release);
		return true;
	}

	bool pop_front(T &item)
	{
		int r = mReadPos.load(std::memory_order_consume);
		int w = mWritePos.load(std::memory_order_consume);
		// Queue is empty (or was empty when we checked)
		if (r == w)
			return false;

		item = mBuffer[r];
		mReadPos.store(increment(r), std::memory_order_release);
		return true;
	}

private:
	inline int increment(int pos) const
	{
		return (pos + 1) % int(mBuffer.size());
	}

	std::vector<T> mBuffer;
	std::atomic<int> mReadPos;
	std::atomic<int> mWritePos;
};

// http://www.sevangelatos.com/lockless-single-producer-single-consumer-bounded-queue/
}