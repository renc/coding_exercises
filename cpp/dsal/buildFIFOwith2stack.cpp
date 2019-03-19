#include <stack>
#include <iostream>

template <typename T>
class FIFO {
public:
    void push_back(const T &v){
        // move elements in s1 to s2
        while (s1.empty() != false) {
            T ele = s1.top(); // return reference, copy constructor
            s1.pop();
            s2.push(ele);
        }
        s2.push(v);
    }
    bool pop_front(T &v) {
        // move elements in s2 to s1
        while(s2.empty() != false) {
            s1.push(s2.top());
            s2.pop();
        }
        if (s1.empty()) {
            return false;
        }
        v = s1.top();
        s1.pop();
        return true;
    }
private:
    std::stack<T> s1;
    std::stack<T> s2;
};