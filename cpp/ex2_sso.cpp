#include <iostream>
#include <string>
#include <cstdlib> // malloc
// small string optimization 

void printSize(const std::string& s)
{
    std::cout << "s=[" << s << "], cap=" << s.capacity() << ", len=" << s.length() << ", sizeof=" << sizeof(s) << std::endl;
}

// override the global operator new
void* operator new(std::size_t sz)
{
    void *ptr = malloc(sz);
    if (!ptr)
        throw std::bad_alloc(); 
    std::cout << "op new sz=" << sz << ", ptr=" << ptr << "\n";
    return ptr;
}

int main(int argc, char* argv[])
{
    std::cout << "size of pointer     =" << sizeof(int*) << "\n";
    std::cout << "size of std::string =" << sizeof(std::string) << "\n"; 

                                         printSize(std::string("")); std::cout << "\n";
    std::string s0 = "0123456789";       printSize(s0); std::cout << "\n";
    std::string s1 = "0123456789ABCDEF"; printSize(s1); std::cout << "\n";
    std::string s2 = "0123456789ABCDEF0123456789ABCDEF"; printSize(s2); std::cout << "\n";
    std::string s4 = "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"; printSize(s4); std::cout << "\n";
    std::string s5 = "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF-+"; printSize(s5); std::cout << "\n";

    return 0;
}
/*
g++ ex2_sso.cpp -o ex2_sso; ./ex2_sso 

2023/8/30
when len=0 for emptry string, cap is the _S_local_capacity for the local buf

The results are different using g++ 4.8.5 without sso, g++ 9 with sso  
without sso: sizeof(str) always == sizeof pointer
*/