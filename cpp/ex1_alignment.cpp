#include <cstdint>
#include <iostream>
uint32_t testNotAlign()
{
    long long x = 0;
    char *y = reinterpret_cast<char*>(&x);
    int* ptr = reinterpret_cast<int*>(y+1); // making this non alignment 
    std::cout << "x=" << x << ", z=" << *ptr << "\n";
    return *ptr;
}

int main()
{
    testNotAlign();
}

/**
[renc@renc-laptop cpp]$ g++ -o ex1_align ex1_alignment.cpp  -Wall -ggdb -O2  -fsanitize=address -fsanitize=undefined 
[renc@renc-laptop cpp]$ ./ex1_align 
ex1_alignment.cpp:8:49: runtime error: load of misaligned address 0x7f4690400021 for type 'int', which requires 4 byte alignment
0x7f4690400021: note: pointer points here
 7f 00 00  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  00 80 3f 90 46
              ^ 
x=0, z=0
ex1_alignment.cpp:9:12: runtime error: load of misaligned address 0x7f4690400021 for type 'int', which requires 4 byte alignment
0x7f4690400021: note: pointer points here
 7f 00 00  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  00 80 3f 90 46
              ^ 
[renc@renc-laptop cpp]$

*/