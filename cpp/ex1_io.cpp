/*
To build: 
g++ ex1_num.cpp -o ./build/ex1_num -std=c++11 -g 
// -g is need for debug
*/ 
#include <climits> // for CHAR_BIT 
#include <vector>
#include <string> 
#include <algorithm>
#include <cassert>  // for assert
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <type_traits>

using namespace std;

vector<string> split_string(string, char delimiter = ' ');
// Complete the jumpingOnClouds function below.
// https://www.hackerrank.com/challenges/jumping-on-the-clouds/problem
int jumpingOnClouds(vector<int> c) {
    int iCount = 0;
    for (int i = 0; i < c.size(); ) {
        if (i + 1 <= c.size() - 1) {
            if (c[i + 1] == 1) { // have to jump 
                ++iCount;
                i += 2;
            }
            else {
                // c[i+1] == 0
                if (i + 2 <= c.size() - 1) {
                    if (c[i + 2] == 0) {
                        ++iCount;
                        i += 2;
                    }
                    else {
                        ++iCount;
                        i++;
                    }
                }
                else {
                    // i+1 == c.size() -1
                    ++iCount;
                    i++;
                }
            }
        }
        else {
            assert(i == c.size() - 1); // last element
            ++i; // to break
        }
    }
    return iCount;

} 
int main4jumpingOnClouds()
{
    //ofstream fout(getenv("OUTPUT_PATH"));
    
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string c_temp_temp;
    getline(cin, c_temp_temp);

    vector<string> c_temp = split_string(c_temp_temp);

    vector<int> c(n);

    for (int i = 0; i < n; i++) {
        int c_item = stoi(c_temp[i]);

        c[i] = c_item;
    }

    int result = jumpingOnClouds(c);
    std::cout << result << "\n";
    //fout << result << "\n";
    //fout.close();

    return 0;
}

vector<string> split_string(string input_string, char delimiter) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [](const char& x, const char& y) {
        return x == y && x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits; 

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}

using namespace std;
namespace e11 {
    template<typename T> void adl(T) { cout << "T"; }
    struct S {};
    template<typename T> void call_adl(T t) {
        adl(S());
        adl(t);
    }
    void adl(S) { cout << "S"; }

    template <typename T> class Foo {
        T tVar; 
    public: 
        Foo() : tVar("-") {}
        Foo(T t) : tVar(t) {}
    };
    class FooDerived : public Foo<std::string> {};
}
namespace e12 {
    class A {
    public: 
        int data;
        ///virtual void f1() {}
        //virtual void f2() {}
        //virtual void f3() {}
    };

    template <template<typename> class>
    struct X { X() { std::cout << "1"; } };

    template< typename> struct Y {};
    template <typename T> using Z = Y<T>;

    template <>
    struct X<Y> {
        X() { std::cout << "2"; }
    };
}
int main()
{
    {
        e12::X<e12::Y> x1;
        e12::X<e12::Z> x2;
    }
    { // 16
        std::cout << sizeof(e12::A) << std::endl;
    }
    {
        e11::FooDerived d; 
        int x = 0; 
        ++x; 
        return 0;
    }
    {
        e11::call_adl(e11::S());
    }
    {
        int a = 0;
        decltype((a)) b = a;
        b++;
        std::cout << a << b; 
    }
    {
        int i, & j = i;
        [=] {
            cout << std::is_same<decltype((j)), int>::value
                << std::is_same<decltype((j)), int&>::value
                << std::is_same<decltype((j)), int const&>::value
                << std::is_same<decltype((j)), int&&>::value
                << std::is_same<decltype((j)), int const&&>::value
                << std::endl;
        } ();
    }


    {
        main4jumpingOnClouds();
    }
    {
        std::string s;
        getline(std::cin, s);
        std::cout << "s=[" << s << "].\n"; // this will let the spaces at the begin and last.
    }
    {
        // To consume the whitespace or newline between the end of a token and the beginning of the next line
        std::string s;
        getline(std::cin >> std::ws, s);
        std::cout << "s=[" << s << "].\n"; // eat the space at the begin, but not at the last/end.
        std::cout << "s=[" << s << "].\n"; // 
    } // i think the safest way is to left trim and right trim 
    {
        double pi = 3.14159;

        // Let's say we wanted to scale this to 2 decimal places:
        std::cout << std::fixed << std::setprecision(2) << pi << std::endl;
        printf("%.2f", pi);
    }
    return 0; // exist status, 0 success. 
}
