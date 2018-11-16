// g++ ./dsal/algoTest1/findIssues.cpp -o ./build/findIssues -std=c++11 -pthread -g

#include <iostream>
#include <cstring> // for memset
#include <list> 

// test 1. 
class A {
    private:
    int n1;
    int n2;
    public:
    A() : n2(0), n1(n2+2) {}
    void print() const { std::cout << "n1: " << n1 << ", n2: " << n2 << std::endl; }

}; 
class A2 {
    private:
    int n1=0;
    int n2=0;
    public:
    A2() : n2(0), n1(n2+2) {}
    void print() const { std::cout << "n1: " << n1 << ", n2: " << n2 << std::endl; }

}; 

// test 2. 
// to calc n factorial, n!
template <int n>
class X {
    public:
    enum { v = n * X<n-1>::v };
};
template <>
class X<0> {
    public:
    enum { v = 1 };
}; 

// test 3. 
class TempB {
    public:
    int n1 = 1;
    int n2 = 2;
    int n3 = 3;
    void print() const {
        std::cout << "n1: " << n1 << ", n2: " << n2 << ", n3: " << n3 << std::endl;
    }
};
class Employee { 
public:
    std::string name, addr; 
    Employee() {};
    Employee(const std::string &_name) : name(_name) {}

    bool operator == (const Employee &_emp) const {
        if (_emp.name == name && _emp.addr == addr)
            return true;
        return false;
    } 
};
// test 4.  to find unnecessary copy of objects
std::string find_addr(const std::list<Employee> &listB, const std::string &name)
{
    for (auto it = listB.cbegin(); it != listB.cend(); ++it)
    {
        if (*it == name)
            return it->addr;
    }
    return "";
} 
// this is tough .....
// i find out the issus of function parameters, const and &,
// but fail to find out (*it) is an object, there is chance the std::string name
// will implicit conversion to object, which create an temp object, but this 
// implicit conversion will not happen if the operator == does not exist. 

int main(int argc, char const *argv[])
{
    {
        A a;
        a.print();
        A2 a2;
        a2.print();
    }
    {
        TempB *pB = new TempB;
        memset(pB, 0, sizeof(pB));
        pB->print();
        memset(pB, 0, sizeof(TempB));
        pB->print();
    }
    std::cout << "X<5>: " << X<5>::v << std::endl;
    return 0;
}

//
// n1 is initialized before n2 as the order during declare, so n1(n2+0) at this time, n2 is not ready.