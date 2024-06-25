/*
https://www.youtube.com/watch?v=XV5sRaSVtXQ
step 1. malloc example, 
    check the malloc example first, which will use strace to find brk 
step 2. this brk example. 
step 3. mmap example 
*/
#include <stdio.h>
#include <unistd.h>

int main()
{
    void *first = sbrk(0); // get the current program break addr
    void *second = sbrk(4096);
    void *third = sbrk(0);

    printf("First: %p\n", first);
    printf("Secon: %p\n", second);
    printf("Third: %p\n", third);
    return 0;
}
/*
[renc@renc-laptop cpp]$ g++ -Wall -ggdb -O3 ex1_brk.cpp
[renc@renc-laptop cpp]$ ./a.out
First: 0x582000
Secon: 0x582000     // return the prev addr 
Third: 0x583000     // 

my explain: 
    0x     5    8    2    0    0    0 
    bin 0101,1000,0010,0000,0000,0000 
                        100,0000,0000 <= +1k,
                       1000,0000,0000 <= +1k, totally add 2k now 
                       1100,0000,0000 <= +1k, totally add 3k now 
                     1,0000,0000,0000 <= +1k, totally add 4k now 
    0x     5    8    3    0    0    0   
*/