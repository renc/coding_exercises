// https://accu.org/index.php/journals/389#ftn.d0e82
// https://accu.org/index.php/journals/383 
// http://goodliffe.blogspot.com/2008/11/c-stl-like-circular-buffer-part-10.html
// https://codereview.stackexchange.com/questions/208293/ring-buffer-implementation-in-c14
// try to implement one version without iterator first, and second version with iterator.

template <typename T>
class circular_buffer {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef size_t difference_type; 

    explicit circular_buffer(size_type capacity = 100)
    : array(new T[capacity]), 
    array_size_(capacity), 
    head_(0), tail_(0), contents_size_(0)
    {

    }
    ~circular_buffer() {
        delete[] array;
    }
    size_t size() const { return contents_size_;}
    bool empty() const { return contents_size_ == 0; }
    value_type top() const {
        return array_[head_];
    }
    void pop_front() {
        increment_head();
    }
    void push_back(const value_type &item) {
        if (contents_size_ == 0) {
            array_[head_] = item; 
            tail_ = head_;
            ++contents_size_;
        } else if (contents_size_ != array_size_ ) {
            // i think it should be contents_size_ < array_size_ 
            increment_tail();
            array_[tail] = item;
        } else {
            // contents_size_ == array_size_ 
            // We always accept data when full and lose the front()
            increment_head();
            increment_tail();
            array_[tail_] = item;
        }
    }
    reference front() { return array_[head_]; }
    const_reference front() { return array_[head_]; }
    reference back() { return array_[tail]; }
    const_reference back() { return array_[tail]; }
    void clear() {
        head_ = tail_ = contents_size_ = 0; 
    }
    reference operator[](size_t i);
    const_reference operator[](size_t i) const;
private:
    void increment_tail() {
        ++tail_;
        ++contents_size_;
        if (tail_ == array_size_) tail_ = 0;
    }
    void increment_head() {
        ++head_;
        --contents_size_;
        if (head_ == array_size_) head_ = 0; 
    }
    T   *array_; // why use dynamic array, not std::vector<T> ?
    size_type  array_size_;
    size_t  head_;
    size_t  tail_;
    size_type  contents_size_;
};