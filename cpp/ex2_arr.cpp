#include <iostream>
#include <string>
#include <string.h> // for strcpy

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
}

int main(int argc, char *argv[])
{
    testCharArr();

    return 0; // exist status, 0 success. 
}