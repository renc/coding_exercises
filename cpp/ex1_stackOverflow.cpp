#include <array>
#include <iostream>
struct Buf1M {
    std::byte buf[1024*1024] = {}; // 1M bytes
};

int main(int argc, char** argv)
{
    std::array<Buf1M, 9> arr;
    std::cout << "OK? size of arr = " << sizeof(arr) / 1024 << "KB \n";
    return 0;
}
/**
[renc@renc-laptop cpp]$ g++ -o ex1_stackOver ex1_stackOverflow.cpp  -Wall -ggdb -O2  -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float
-cast-overflow -fno-sanitize=null -fno-sanitize=alignment 
/usr/bin/ld: cannot find /usr/lib64/libasan.so.8.0.0: No such file or directory
collect2: error: ld returned 1 exit status

// to solve this problem: 
sudo dnf install libasan libubsan

// this intensional by redhat, since they think most of ppl do not use this feature. 

[renc@renc-laptop cpp]$ ./ex1_stackOver 
AddressSanitizer:DEADLYSIGNAL
=================================================================
==32513==ERROR: AddressSanitizer: stack-overflow on address 0x7ffe9161b780 (pc 0x0000004010f6 bp 0x7ffe91f1b950 sp 0x7ffe9161b780 T0)
    #0 0x4010f6 in main /home/renc/repo/coding_exercises/cpp/ex1_stackOverflow.cpp:8
    #1 0x7f3dac5dc149 in __libc_start_call_main (/lib64/libc.so.6+0x28149) (BuildId: 788cdd41a15985bf8e0a48d213a46e07d58822df)
    #2 0x7f3dac5dc20a in __libc_start_main_impl (/lib64/libc.so.6+0x2820a) (BuildId: 788cdd41a15985bf8e0a48d213a46e07d58822df)
    #3 0x4012e4 in _start (/home/renc/repo/coding_exercises/cpp/ex1_stackOver+0x4012e4) (BuildId: 11c51f0478082eacc33e2ba3f1bc2cb173ef49cf)

SUMMARY: AddressSanitizer: stack-overflow /home/renc/repo/coding_exercises/cpp/ex1_stackOverflow.cpp:8 in main
==32513==ABORTING
**/