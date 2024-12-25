/*
Code::dive  Mathieu Ropert - Heaps Don't Lie Guidelines for Memory Allocation in C++ 

Guildline #1. use stack when possible, it is quick;
Guildline #2. malloc() is usually fast except when it isn't.
             do not use malloc when u are in the plant.
            malloc() process
                Try to get a free block of size N from free list;             1x
                Grab a free page, initialize it as page of size objects;      10x
                Use an OS system call to get more pages;                      10x 
                Swap some physical pages to disk.                             10x 
            and u cannot predict which step it will success. 

Exploring neighbours 
    std::vector<>::reserve(n);

Guildline #4. reserve final container sizewhen known rather than rely on geometric growth (0, 1, 2, x2, x2,...)
Guildline #5. avoid using reserve() with constant delta in loops. 
            because these small constant delta accumuated small which may not need allocate, while each reserve() will allocate. 


Zooming out
    while (!search_queue.empty()) {
        const Point current = search_queue.top();
        search_queue.pop();
        for (const Point p : find_neighbours(current)) {  // <-- allocate everytime         -> change api to  void find_neighbours(Point p, std::vector<Point>& neighbours) const ; 
            ... 
        }
    }

Guildline #6. prefer output parameters to returns for containers.
Guildline #7. reuse previous allocations when possible.
Guildline #8. prefer memberwise .clear() over assignment to empty struct. 


General purpose allocator 
    - has to be "good enough" for everyone ;
    - can't make assumptions about usage patterns;
    - compromises have to be made;

What about alloca() ?
    - not standard (but very common);
    - Dynamic stack allocation
    - C API, need wrapper to add RAII back; 
    - Fast
    Drawbacks 
        - bound by stack size ( several MB )
        - limited space, especially on threads, especially on macOS, 
        - not all threading libs suppport setting stack size.
Guideline #9. avoid using alloca() because it's likely to overflow the stack. 

Recommendation: Taming Dynamic Memory - An Introduction to Custom Allocators - Andreas Weis - ACCU 2019.

Monotonic allocators
    - allocates by increasing a pointeruntil it runs out
    - as fast as stack alloc
    - cannot deallocate one specific object. 
    - free all or nothing 
    sturct monotonic_buffer {
        std::unique_ptr<std::byte[]> buffer_;
        std::byte *current_;
        explicit monotonic_buffer(std::size_t size) : buffer_(std::make_unique<std::byte[]>(size)), current_(buffer_.get()) {}
        ...

        void* allocate(std:;size_t size) {
            constexpr auto align_mask = sizeof(std::max_align_t) -1; 
            const auto alloc_size = (size + align+mask) & ~align_mask;
            void* allocated = current_;
            current_ += alloc_size;
            return allocated;                // -------------------------------------->
        }                                    //        | alloc      | curr_
        void deallocate( void* ptr) {} // nice, do nothing
        void deallocate_all() { current_ = buffer_.get(); }      // free all or nothing 
    };

Guideline #10. consider monotonic allocators when resources can be scoped and budgeted.

*/