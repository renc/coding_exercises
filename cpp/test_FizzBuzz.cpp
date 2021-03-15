#include <iostream>
#include <string>
#include <map>
using namespace std;
void version1() {
    for (int i = 1; i <= 100; i++) {
        if ((i % 3 == 0) && (i % 5 == 0))
            cout << "FizzBuzz\n";
        else if (i % 3 == 0)
            cout << "Fizz\n";
        else if (i % 5 == 0)
            cout << "Buzz\n";
        else
            cout << i << "\n";
    }

}
void version2() {
    std::map<int, std::string> m{ {3, "Fizz"}, {5, "Buzz"} };
    for (int i = 1; i <= 100; ++i) {
        std::string s; 
        for (auto [k,v] : m) {
            if (i % k == 0) {
                s += v;
            }
        }
        std::cout << (s.empty() ? std::to_string(i) : s) << std::endl;
    }
}
int main()
{
    version2();
    return 0;
}