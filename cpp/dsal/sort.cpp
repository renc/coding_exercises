#include <algorithm>
#include <vector>
#include <iostream>

// $ g++ ./dsal/sort.cpp -o ./build/sort -std=c++11 -g
// -g for debug


// http://en.cppreference.com/w/cpp/algorithm/inplace_merge 
// std::inplace_merge: 
// Merges two consecutive sorted ranges [first, middle) and [middle, last) 
// into one sorted range [first, last). (renc, i think especially for vector, )
template <typename Iter>
void print(Iter first, Iter last)
{
    while (first != last)
    {
        std::cout << *first << ",";
        ++first;
    }
    std::cout << *last ; // to print the last one, for debug
    std::cout << std::endl;
}
template <typename Iter>
void inplaceMergeT(Iter first, Iter middle, Iter last)
{
    // renc, my implement of inplace merge
    
}
template <typename Iter>
void mergeSortT(Iter first, Iter last)
{
    // Iter last is the one pass the last valid element.
    std::cout << "Into MST:"; print(first, last);
    std::cout << "[first " << *first << ", last " << *last << ")" << std::endl;
    std::cout << "dis:" << std::distance(first, last) << ", " << last - first << std::endl;
    if (last - first > 1) {
        Iter middle = first + (last - first) / 2;
                                    std::cout << "middle:" << *middle << std::endl;
                                    std::cout << "ms1B:"; print(first, middle);
        mergeSortT(first, middle);  std::cout << "ms1E:"; print(first, middle);
                                    std::cout << "ms2B:"; print(middle, last); 
        mergeSortT(middle, last);   std::cout << "ms2E:"; print(middle, last); 
                                                std::cout << "mgB:"; print(first, last);
        std::inplace_merge(first, middle, last); std::cout << "mgE:"; print(first, last);
    }
    else {
        int a = 0; 
        int b = a;
        ++a;
    }
}
void testMergeSort()
{
    std::vector<int> v{8, 2, -2, 1};//, 11, 11, 1, 7, 3};
    mergeSortT(v.begin(), v.end());
    for(auto n : v) {
        std::cout << n << ' ';
    }

    std::cout << '\n';
}
int main(int argc, char const *argv[])
{
    testMergeSort();
    return 0;
}
