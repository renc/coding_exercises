/*
https://www.youtube.com/watch?v=XV5sRaSVtXQ
step 1. malloc example, 
    check the malloc example first, which will use strace to find brk 
step 2. this brk example. 
step 3. mmap example 
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    //for (int i=0; i < 5; ++i)
    {
        char *ptr = (char*)malloc(50000);
        printf("Got memory! Address=%p\n", ptr);
    }
    return 0;
}
/* 
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
