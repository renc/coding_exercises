/*
https://www.youtube.com/watch?v=XV5sRaSVtXQ
step 1. malloc example, 
    check the malloc example first, which will use strace to find brk 
step 2. this brk example. 
step 3. mmap example 
*/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <bits/mman-linux.h>

#define PAGESIZE 4096

int main()
{
    uint8_t* first = (uint8_t*)mmap(NULL, PAGESIZE, PROT_READ|PROT_WRITE, 
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    uint8_t* second = (uint8_t*)mmap(NULL, PAGESIZE, PROT_READ|PROT_WRITE, 
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    printf("First: %p\n", first);
    printf("Secon: %p\n", second);
    return 0;
}
/*
[renc@renc-laptop cpp]$ g++ -Wall -ggdb -O3 ex1_mmap.cpp 
[renc@renc-laptop cpp]$ ./a.out
First: 0x7f025e1fa000
Secon: 0x7f025e1f9000

why the addr decrease in this mmap example result? 
while the addr increase in brk example 
*/