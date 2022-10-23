#include <iostream>
#include <string>
#include <string.h> // for strcpy
#include <vector>


void testCharArr()
{
    char abc[27]; // abs is a name of an array, sizeof(abc) return 27;
    char *ptr = abc; // ptr to a char, read the book of [understanding of C points]
    strcpy(abc, "abcdefgxyz");

    /* waht are the types and values of expressions: 

    */ 
   std::cout << "abc:" << abc << std::endl;
   std::cout << "*abc:" << *abc << std::endl;
   std::cout << "abc[2]:" << abc[2] << std::endl;
   std::cout << "&abc[3]:" << &abc[3] << std::endl;
   std::cout << "abc+4:" << abc+4 << std::endl;
   std::cout << "*(abc+5)+2:" << *(abc+5)+2 << std::endl;
   std::cout << "abc[10]:" << abc[10] << std::endl;
   std::cout << "abc[12]:" << abc[12] << std::endl;
   std::cout << "ptr:" << ptr << std::endl;
   std::cout << "&ptr:" << &ptr << std::endl; 

   {
       char a[4] = { 'a', 'b', 'c', 'd' };
       std::cout << "a: sizeof " << sizeof(a) << std::endl;
   }
}

namespace test6 {
    struct A {
        int _i;
        ~A() { std::cout << _i << std::endl; }
    };
}
namespace test7 {
    class A {
    private: 
        int data;
    public:
        A() : data{ 0 } {}
        A(const A&) = delete;
        A& operator=(const A&) = delete;
    };
}
int main(int argc, char *argv[])
{
    //testCharArr();

    {
        //auto a = new A[3]{ {0}, {1}, {2} };
        //delete a;
        //delete[] a;
    }
    {
        // test7
        //std::vector<test7::A> aVec;
        //aVec.resize(5);// do not compile
    }

    {
        unsigned i = 1;
        auto l = [i](int j) { i -= j;  };
        l(2);
        std::cout << i << std::endl;
    }
    return 0; // exist status, 0 success. 
}