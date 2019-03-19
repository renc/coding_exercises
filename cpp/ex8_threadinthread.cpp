
// g++ ex8_threadinthread.cpp -o ./build/ex8_threadinthread -std=c++11 -pthread -g
// 
#include <thread>
#include <iostream>
#include <chrono>


class Foo 
{
public:
    Foo() {
        m_thread = std::thread(&Foo::func4Thread, this);
    }
    ~Foo() {
        if (m_thread.joinable())
            m_thread.join();
    }
    void func4Thread() {
        for (int i = 0; i < 10; ++i)
        {
            std::cout << "func4Thread: tid " << std::this_thread::get_id() << ", i" << i << std::endl;
            //std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
    std::thread m_thread; 

    void onDoSth(int x) {
        int a = x; 
        int b = a; ++b;
        std::cout << "onDoSth: tid " << std::this_thread::get_id() << ", " << b << std::endl;
    }
};

void func()
{
    Foo foo;
    std::cout << "func: tid " << std::this_thread::get_id() << ", " << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));// i add this to prevent foo.onDoSth has done before func4Thread start.
    for (int i = 0; i < 10; ++i)
        foo.onDoSth(i);
}

int main()
{
    std::thread t(func);
    t.join();
    return 0; 
}

