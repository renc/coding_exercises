/*
https://www.youtube.com/watch?v=XV5sRaSVtXQ
step 1. malloc example, 
    check the malloc example first, which will use strace to find brk 
step 2. brk example.  
step 3. mmap example 
*/

#include <stdio.h>
#include <cstdint>          // for uint8_t 
#include <stdlib.h>         // for malloc 
#include <string_view>
#include <unistd.h>         // for brk
#include <sys/mman.h>       // for mmap 
#include <bits/mman-linux.h> // for mmap 

int main(int argc, char* argv[])
{
    if (argc != 2)
        printf("Usage: app malloc/brk/mmap\n");
    else {
        auto cmd = std::string_view(argv[1]); // no std::string here 
        if (cmd == "malloc") {
            //for (int i=0; i < 5; ++i)
            {
                char *ptr = (char*)malloc(50000);
                printf("Got memory! Address=%p\n", ptr);
            }
        } 
        else if (cmd == "brk") {
            void *first = sbrk(0); // get the current program break addr
            void *second = sbrk(4096);
            void *third = sbrk(0);

            printf("First: %p\n", first);
            printf("Secon: %p\n", second);
            printf("Third: %p\n", third);
        } 
        else if (cmd == "mmap") {
            #define PAGESIZE 4096
            uint8_t* first = (uint8_t*)mmap(NULL, PAGESIZE, PROT_READ|PROT_WRITE, 
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
            uint8_t* second = (uint8_t*)mmap(NULL, PAGESIZE, PROT_READ|PROT_WRITE, 
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            
            printf("First: %p\n", first);
            printf("Secon: %p\n", second);
        } 
    }
    
    return 0;
}
/* case of malloc 
[renc@renc-laptop cpp]$ g++ -o a -Wall -ggdb -O3 ex1_malloc.cpp
[renc@renc-laptop cpp]$ strace ./a
...
brk(NULL)                               = 0x879000
brk(0x89a000)                           = 0x89a000
newfstatat(1, "", {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0x3), ...}, AT_EMPTY_PATH) = 0
write(1, "Got memory! Address=0x88b2b0\n", 29Got memory! Address=0x88b2b0
) = 29
exit_group(0)                           = ?
+++ exited with 0 +++

// it uses brk when the asked size is small, it use mmap when the asked size is big
so what is brk ? check man brk, program break , system call brk, or use sbrk(size_t) to change the program break/
*/
/* case of brk
[renc@renc-laptop cpp]$ g++ -Wall -ggdb -O3 ex1_brk.cpp
[renc@renc-laptop cpp]$ ./a.out
First: 0x582000
Secon: 0x582000     // return the prev addr 
Third: 0x583000     // 

my explain: 
    0x     5    8    2    0    0    0 
    bin 0101,1000,0010,0000,0000,0000 
                        100,0000,0000 <= +1k, which is 1024=pow(2,10)
                       1000,0000,0000 <= +1k, totally add 2k now 
                       1100,0000,0000 <= +1k, totally add 3k now 
                     1,0000,0000,0000 <= +1k, totally add 4k now 
    0x     5    8    3    0    0    0   
*/

