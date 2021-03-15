#pragma once 
 
#include <iostream>
#include <string>
#include <strstream>
#include <future>
#include <functional>
#include <type_traits>
#include <vector>
#include <thread>
#include <cassert>
 
namespace renc {
    // std::sort(str.begin(), str.end() ); // sort chars in a string 

    char* strcpy1(char* to, const char* from)
    {
        if (from == NULL) return NULL;
        if (to == from) return to;
        std::cout << strlen(to) << ", " << strlen(from) << std::endl;
        char* head = to; // how about not enough memory for to ?
        int i = 0;
        for (; from[i] != '\0'; ++i) // how about not nul terminated?
            to[i] = from[i];
        to[i] = '\0'; // forget ths ?
        return head;
    }

    // function to remove all spaces.
    bool myIsSpace(char ch)
    {
        return std::isspace(static_cast<unsigned char>(ch));
    }
    std::string removeSpace(const std::string& str)
    {
        std::string ret{ "" };
        for (auto e : str) {
            if (!myIsSpace(e)) {
                ret += e;
            }
        }
        return ret;
    }

    // trim from start (in place)
    static void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin, s.end(), [](char c) {
            return !std::isspace(c); }));
    }
    static void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](char c) {
            return !std::isspace(c); }).base(), s.end());
    }
    static void trim(std::string& s) {
        ltrim(s); 
        rtrim(s);
    }
    // (copying)
    std::string ltrim_c(std::string s) { ltrim(s); return s; } 
    std::string rtrim_c(std::string s) { rtrim(s); return s; }
    std::string trim_c(std::string s) { trim(s); return s; }
                                                               
    // function to count the number of words
    void getWordsFromString(const std::string& str)
    {
        std::istringstream input;
        input.str(str);
        int iCount = 0;
        while (input)
        {
            std::string sWord;
            input >> sWord;
            if (sWord.empty() == false)
                std::cout << iCount++ << ":[" << sWord << "]" << std::endl;
            // do not know why the last word is empty
        }
    }

    std::vector<std::string> split(const std::string& s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }
    std::vector < std::string split(const std::string& s, const std::string &delim) {
        std::vector<std::string> > result;
        auto start = 0U;
        auto end = s.find(delim);
        while (end != std::string::npos)
        {
            result.push_back(s.substr(start, end - start)); // could be empty,say s="abc",delim="a"
            start = end + delim.length();
            end = s.find(delim, start);
        }

        result.push_back(s.substr(start, end));
        return result;
    }
    // from https://www.hackerrank.com/
    std::vector<std::string> split_string(std::string input_string) {
        std::string::iterator new_end = std::unique(input_string.begin(), input_string.end(), [](const char& x, const char& y) {
            return x == y and x == ' ';
        });

        input_string.erase(new_end, input_string.end());

        while (input_string[input_string.length() - 1] == ' ') {
            input_string.pop_back();
        }

        std::vector<string> splits;
        char delimiter = ' ';

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

    void test_string() {

        assert(removeSpace(" 1 2 3    \t 4 \n 5 ") == std::string("12345"));
        assert(removeSpace("a b  c def g  hi j") == std::string("abcdefghij"));
        assert(removeSpace("  a b  c def g  hi j  ") == std::string("abcdefghij"));
        assert(removeSpace("a b\tcd\ne") == std::string("abcde"));
        getWordsFromString(" how    are \t you ? ");
    }

}; // namespace renc 