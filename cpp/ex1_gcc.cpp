
int A, B;

void foo()
{
    A = B + 1;
    B = 0;
}

/*
// without compile optimize 
gcc -S -masm=intel ex1_gcc.cpp -o ./build/ex1_gcc.s
cat ./build/ex1_gcc.s

// with compile optimize 
gcc -S -O2 -masm=intel ex1_gcc.cpp -o ./build/ex1_gcc.s
cat ./build/ex1_gcc.s
*/ 