/*
g++ ex3_std_algorithm.cpp -o ex3_std_algorithm -std=c++11
*/
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
 
void test_std_transform()
{
    // manipulate directly on the orginal string 
    std::string s("hello");
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) -> unsigned char { return std::toupper(c); });
    
    // append the result to new location 
    std::vector<std::size_t> ordinals;
    std::transform(s.begin(), s.end(), std::back_inserter(ordinals),
                   [](unsigned char c) -> std::size_t { return c; });
    
    std::cout << s << ':';
    for (auto ord : ordinals) {
       std::cout << ' ' << ord;
    }
    std::cout << std::endl;
}


// https://en.cppreference.com/w/cpp/algorithm/for_each
struct Sum 
{
    Sum() : sum(0) {}
    void operator() (int n) { sum += n;}
    int sum;
};
void test_std_for_each()
{
    std::vector<int> nums{3, 4, 2, 8, 15, 267};
    
    auto print = [] (const int &n){ std::cout << " " << n;};
    std::cout << "before:";
    std::for_each(nums.begin(), nums.end(), print);
    std::cout << std::endl; // in this case, the value is not modified.

    std::cout << "after:";
    std::for_each(nums.begin(), nums.end(), [](int &n){++n;} ); // value modified.
    std::for_each(nums.begin(), nums.end(), print); 
    std::cout << std::endl;

    // calls Sum::operator() for each number
    // All elements use the same sum ?!
    Sum s = std::for_each(nums.begin(), nums.end(), Sum());
    std::cout << "Sum: " << s.sum << std::endl;
}

int main()
{
    test_std_transform();
    test_std_for_each();
    return 0; // exist status, 0 success. 
}