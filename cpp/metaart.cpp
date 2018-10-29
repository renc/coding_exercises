
/* 


http://www.cs.rpi.edu/~musser/design/blitz/meta-art.html

*/ 

#include <cstdio>

template<int N>
class Factorial {
    public:
    enum { value = N * Factorial<N-1>::value };
};
template <>
class Factorial<0> {
    public:
    enum { value = 1 };
};

int main()
{
    printf("Factorial<0> is : %d.\n", Factorial<0>::value);
    printf("Factorial<1> is : %d.\n", Factorial<1>::value);
    printf("Factorial<3> is : %d.\n", Factorial<3>::value);
    printf("Factorial<10> is : %d.\n", Factorial<10>::value);
    return 0; 
}
/*
 g++ metaart.cpp -o /home/oracle/Documents/coding_exercises_build/metaart

*/