// custom allocator , memory pool, 
#include <memory> // for 
#include <vector> // when vector reallocat, it is not ask size1, but duplicate 1, 2, 4, 8, 16, ...
#include <list> // when list allocate, it ask for sz=1, 1, 1, ....
#include <set> // for test
#include <array>
#include <limits> // for std::limit_numeric
#include <numeric> // for std::accumulate
#include <assert.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <cstddef> // std::byte
#include <memory_resource> // std::pmr

namespace wwh {
// /home/renc/ws/wwhrt-bookbuilder-workshop/Allocator.h
// renc: this is a most basic one, i think they have a good one but just shart the basic one for us to modify
 
// This memory allocator can be used as a template parameter to
// various C++ stdlib datastructures. We've provided a default
// implementation below that can be modified if desired.
template <typename T> struct Allocator {
    using pointer = T*;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using value_type = T;
    using size_type = size_t;
    using difference_type = ssize_t;

    Allocator() {}

    [[nodiscard]] T* allocate(std::size_t n) {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t) { ::operator delete(p); }
};

template <class T, class U>
bool operator==(const Allocator<T>&, const Allocator<U>&) {
    return true;
}
template <class T, class U>
bool operator!=(const Allocator<T>&, const Allocator<U>&) {
    return false;
}

} // end of namespace wwh

namespace cppref {
// https://en.cppreference.com/w/cpp/named_req/Allocator
// This is also mentioned in book C++ high performance by Bjorn,, this is called stateless allocator
template<class T>
struct Mallocator
{
    typedef T value_type;
 
    Mallocator() = default;
 
    template<class U>
    constexpr Mallocator(const Mallocator <U>&) noexcept {}
 
    [[nodiscard]] T* allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_array_new_length();
 
        if (auto p = static_cast<T*>(std::malloc(n * sizeof(T))))
        {
            //report(p, n);
            return p;
        }
 
        throw std::bad_alloc();
    } // i think this is similar to the above version using ::operation new, because new do malloc and bad_alloc too.
 
    void deallocate(T* p, std::size_t n) noexcept
    {
        //report(p, n, 0);
        std::free(p);
    }
private:
    void report(T* p, std::size_t n, bool alloc = true) const
    {
        std::cout << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }
};
 
template<class T, class U>
bool operator==(const Mallocator <T>&, const Mallocator <U>&) { return true; }
 
template<class T, class U>
bool operator!=(const Mallocator <T>&, const Mallocator <U>&) { return false; }
} // end of namespace cppref

#include <cstddef> // std::max_align_t , std::byte
#include <cstdint> // std::uintptr_t 
namespace cpphp {
// from chapter 7, C++ High Performance by Bjorn ...
// arena == memory pool , continues memory
template <size_t N>
class Arena {
    static constexpr size_t alignment = alignof(std::max_align_t);
public:
    Arena() noexcept : ptr_(buffer_) {}
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    auto reset() noexcept { ptr_ = buffer_; }
    static constexpr auto size() noexcept { return N; }
    auto used() const noexcept { return static_cast<size_t>(ptr_ - buffer_); }
    auto allocate(size_t n) -> std::byte*;
    auto deallocate(std::byte* p, size_t n) noexcept -> void;
private:
    static auto align_up(size_t n) noexcept -> size_t {
        return (n + (alignment-1)) & ~(alignment-1);
        // rounds up the alignment ?
    }
    auto pointer_in_buffer(const std::byte* p) const noexcept -> bool {
        return std::uintptr_t(p) >= std::uintptr_t(buffer_) &&
               std::uintptr_t(p) <  std::uintptr_t(buffer_)+N;
    }
    alignas(alignment) std::byte buffer_[N];
    std::byte* ptr_{};
};  
template <size_t N>
auto Arena<N>::allocate(size_t n) -> std::byte* {
    const auto aligned_n = align_up(n);
    const auto available_bytes = static_cast<decltype(aligned_n)>(buffer_+N-ptr_);
    if (available_bytes >= aligned_n) {
        auto* r = ptr_;
        ptr_ += aligned_n;
        return r;
    }
    return static_cast<std::byte*>(::operator new(n));
}
template <size_t N>
auto Arena<N>::deallocate(std::byte* p, size_t n) noexcept -> void
{
    
}
// page 228, using the Arena to build a custom allocator (a stateful allocator vs stateless allocator Mallocator)
// this is important class, which bind the arena with allocator which can be used in container. 
template <typename T, size_t N>
struct ShortAlloc {
    using value_type = T;
    using arena_type = Arena<N>; // ok, we know Arena is about N size, without knowning type T. 
    ShortAlloc(const ShortAlloc&) = default;
    ShortAlloc& operator=(const ShortAlloc&) = default;
    ShortAlloc(arena_type& arena) noexcept : arena_{&arena} {} // ctro, need input of arena_type, is it better to create it inside wo relay on input from outside? like abm:: did
    
    template <typename U>
    ShortAlloc(const ShortAlloc<U, N>& other) noexcept : arena_{other.arena_} {} 
    template <typename U> struct rebind { using other = ShortAlloc<U, N>; };

    auto allocate(size_t n) -> T* { return reinterpret_cast<T*>(arena_->allocate(n*sizeof(T))); }
    auto deallocate(T* p, size_t n) noexcept -> void { arena_->deallocate(reinterpret_cast<std::byte*>(p), n*sizeof(T)); }

    template <class U, size_t M>
    auto operator==(const ShortAlloc<U, M>& other) const noexcept { return N == M && arena_ == other.arena_; }
    template <class U, size_t M>
    auto operator!=(const ShortAlloc<U, M>& other) const noexcept { return !(*this == other); }
    
    template <class U, size_t M> friend struct ShortAlloc;
private:
    arena_type* arena_;
}; // end ShortAlloc
void testShortAlloc()
{
    using SmallSet = ::std::set<int, std::less<int>, ShortAlloc<int, 512>>;

    auto stack_arena = std::set<int, std::less<int>, ShortAlloc<int, 512>>::allocator_type::arena_type{};
    auto unique_numbers = SmallSet{stack_arena};

    // read num from stdin
    auto n = int{};
    while (::std::cin >> n) unique_numbers.insert(n); 

    for (const auto& num : unique_numbers)
        ::std::cout << num << "\n";
}
// issue is for function: void funcX(std::set<int, std::less<int>& aa) {}; // we cannot pass our SmallSet into this func, because the allocator becomes a part of the type
// This problem of ending up with different types when using different allocators was addressed in C++17 by introducing an extra layer, which is std::pmr::polymorphic_allocator.
// Memory resources:
//  std::pmr::monotonic_buffer_resource , similar to our Arena class, for short lifetime objects.
//  std::pmr::unsynchronized_pool_resource, 
//  std::pmr::synchronized_pool_resource 
//  page 234, class CustomMemoryResource : public std::pmr::memory_resource {}; 
void testPmr()
{
    // similar to testShortAlloc()
    auto buffer = ::std::array<::std::byte, 512>{}; // similar to our stack Aeran 
    auto resource = ::std::pmr::monotonic_buffer_resource{buffer.data(), buffer.size(), ::std::pmr::new_delete_resource()};

    auto unique_numbers = ::std::pmr::set<int>{&resource};

    auto n = int{};
    while (::std::cin >> n) { unique_numbers.insert(n); }

    for (const auto& num : unique_numbers) { ::std::cout << num << "\n"; }
}
} // end of cpphp 

namespace abm {
/////////////////////////////////////////////////////////////////
// 
//          Copyright Vadim Stadnik 2023.
// Distributed under the Code Project Open License (CPOL) 1.02. 
// (See or copy at http://www.codeproject.com/info/cpol10.aspx) 
//
/////////////////////////////////////////////////////////////////

//
//  This file contains demo code for the article 
//  by V. Stadnik "Array-Based Memory Pool and Allocator"; 
//
 
//  - class ArrayPool implements array-based memory pool with   
//    low level management facilities for unitialized memory ; 
//  - it allocates and frees single blocks of fixed size ; 
//  - in client C++ code, one instance of this class can be used by 
//    allocators of different types that satisfy this requirement ; 
template< size_t  num_blocks, size_t  block_size>  
class ArrayPool
{
    //  the storage is a contigous region of memory represented by an array of bytes, 
    //  it is divided into equal size blocks that can store objects of client algorithms 
    unsigned char   arr_bytes[num_blocks*block_size] ; 
    //  this array of pointers <byte*> represents a stack, 
    //  a pointer <byte*> points to the first byte in a proper block ;
    //  the stack can be regarded as a controller or manager for blocks in the storage ; 
    unsigned char*  arr_stack[num_blocks] ; 
    //  index of current top element in the stack
    ptrdiff_t       i_top ; 

public:
    ArrayPool( ) : i_top(-1) 
    {
        unsigned char*  p_block = arr_bytes ; 
        for ( int  i = 0 ; i < num_blocks ; ++i, p_block+=block_size )
            arr_stack[i] = p_block ; 

        i_top = static_cast<ptrdiff_t>(num_blocks) - 1 ; 
    } 

    //  allocates one block of uninitialized memory 
    void *  Allocate( ) 
    {   return reinterpret_cast<void*>( arr_stack[i_top--] ) ; }

    //  returns one block of memory back to pool storage 
    void    Deallocate( void *  p_back ) 
    {   arr_stack[++i_top] = reinterpret_cast<unsigned char*>(p_back) ; }

    bool    IsEmpty( )   const noexcept { return i_top < 0 ; } 
    bool    IsFull ( )   const noexcept { return i_top == (num_blocks-1) ; } 

    //  the total number of blocks in array storage, it is also 
    //  the maximum number of objects that can be stored for client algorithms 
    size_t  Capacity( )  const noexcept { return num_blocks ; } 
    //  the number of free blocks available for client algorithms 
    size_t  NumFree( )   const noexcept { return i_top + 1 ;  }
    //  the number of blocks used in client algorithms
    size_t  NumUsed( )   const noexcept { return Capacity() - NumFree() ; } 
    //  the size of array storage in bytes 
    size_t  MemorySize() const noexcept { return num_blocks*block_size ; }

private:
    //  copy constructor and assignment operator are disabled, since 
    //  in client code this array pool supports stateless C++ allocator 
    ArrayPool ( ArrayPool const & ) ; 
    ArrayPool & operator = ( ArrayPool const & ) ; 
} ; 


//  - class AllocrBaseSTL<T> provides type definitions 
//    for C++ standard compliant allocator ; 
//  - it also includes member functions, which are less relevant to 
//    the explanation of the main methods of the array-based allocator ; 
template < class _Ty >
class AllocrBaseSTL
{	
public:
    typedef _Ty             value_type		;
    typedef size_t          size_type		;
    typedef ptrdiff_t       difference_type	;
    typedef _Ty *           pointer			;
    typedef const _Ty *     const_pointer	;
    typedef _Ty &           reference		;
    typedef const _Ty &     const_reference	;

    pointer       address( reference       val ) const noexcept { return (&val); }
    const_pointer address( const_reference val ) const noexcept { return (&val); }

    void construct( pointer ptr, const_reference  val ) { ::new (ptr) value_type(val) ; }
    void destroy  ( pointer p )                         { p->~_Ty() ; }
} ;	


//  the maximum number of values that can be stored in custom allocator ArrayPool ; 
//const size_t    ALR_CAPACITY = 100000 ;  // renc: terminate called after throwing an instance of 'std::bad_alloc' 
//const size_t    ALR_CAPACITY = 1000000 ; // renc: ok !
const size_t    ALR_CAPACITY = 1000000 * 8 ; // renc: better !

//  - class ArrayAllocatorSTL represents stateless STL compliant allocator 
//    all instances of one allocator type operate on the same memory or resource ; 
//  - implementation of this class uses ArrayPool as a static data member ; 
//  - class ArrayAllocatorSTL can be used with node-based STL containers: 
//    std::list, std::forward_list, std::set, std::multiset, std::map and std::multimap; 
template < class _Ty >
class ArrayAllocatorSTL : public AllocrBaseSTL<_Ty>  
{
private: 
    static ArrayPool<ALR_CAPACITY , sizeof(_Ty)>
                            m_array_pool ; 

public: 
    ArrayAllocatorSTL ( ) noexcept { } 
    ArrayAllocatorSTL ( const ArrayAllocatorSTL& ) noexcept { }    
    template <class U> 
    ArrayAllocatorSTL ( const ArrayAllocatorSTL<U>& ) noexcept { } 

    _Ty* allocate ( size_t  count, const void*  p_hint = 0 ) 
    {
        // std::cout << count << ", "
        // for std::vector, count 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, ....
        // for std::list,   count 1, 1, 1, 1, ...
        // renc, tag, that is why it use std::list in the original article      
        if ( count==1 ) 
        {
            if ( m_array_pool.IsEmpty() ) 
                throw std::bad_alloc() ;

            return reinterpret_cast<_Ty*>( m_array_pool.Allocate() ) ; 
        }
        else 
            return reinterpret_cast<_Ty*>( ::operator new (count*sizeof(_Ty) ) ) ; 
    }

    void deallocate ( _Ty*  p, size_t  count ) 
    {
        if ( count==1 ) 
            m_array_pool.Deallocate( reinterpret_cast<void*>(p) ) ; 
        else 
            ::operator delete ( p ) ; 
    }
} ; 

template< class T1, class T2 >
bool operator==( const ArrayAllocatorSTL<T1>&, const ArrayAllocatorSTL<T2>& ) noexcept { return true ; }
template< class T1, class T2 >
bool operator!=( const ArrayAllocatorSTL<T1>&, const ArrayAllocatorSTL<T2>& ) noexcept { return false; }

template< class _Ty > ArrayPool< ALR_CAPACITY , sizeof(_Ty) >    
        ArrayAllocatorSTL<_Ty>::m_array_pool ; 
} // end of namespace abm

struct TestA {
    std::uint32_t count = 0; 
    std::array<char, 16> arrBuf={0};
    char charBuf[16] = {0};
};

constexpr int iCount = 1000000;

template <typename AllocT>
void test(const std::string& name)
{
    std::list<TestA, AllocT > v;
    const auto start = std::chrono::high_resolution_clock::now();
    for (int repeatCount = 0; repeatCount < 10; ++repeatCount) {
        for (int i = 0; i < iCount; ++i) v.push_back(TestA());
        v.clear();
    }
    const std::chrono::duration<double, std::micro> diff = std::chrono::high_resolution_clock::now() - start;
    std::cout << name <<", diff=" << diff.count() << "\n";
}
int main(int argc, char* argv[])
{
    test<wwh::Allocator<TestA> >        ("wwh::Allo");
    test<cppref::Mallocator<TestA> >    ("cppref::M");
    test<abm::ArrayAllocatorSTL<TestA> >("abm::ArrA");  
    return 0; 
}
/*
g++ -O2 ex3_alloc.cpp -o alloc
*/