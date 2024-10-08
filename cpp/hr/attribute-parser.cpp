#include <cmath>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
using namespace std;

std::string accumulateTagNames(const std::vector<std::string>&tagStack)
{
    if (tagStack.size() > 0) {
        std::string tag = tagStack[0];
        for (int i = 1; i < tagStack.size(); ++i) {
            tag += "." + tagStack[i];
        }
        return tag;
    }
    return "";
}
int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
    int n, q;
    std::cin >> n >> q;
    std::cin.ignore();// wrong without this
    // HRML 
    std::vector<std::string> tagStack;
    std::unordered_map<std::string, std::string> mAtt2Val;
    std::string line;
    for (int i =0; i < n; ++i) {
        std::getline(std::cin, line);
//        std::cout << "line=" << line << std::endl;
        if (line[1] == '/') {
            // </tagname> 
            tagStack.pop_back();
        } else {
            // remove the > at the end of line
            // remove-erase idiom to get rid of quotation marks
line.erase(std::remove(line.begin(),line.end(),'\"'),line.end()); 
// remove-erase idiom to get rid of greater-than sign
line.erase(std::remove(line.begin(),line.end(),'>'),line.end()); 

            std::stringstream ss(line);
            char cLess, cEqual;
            std::string tagName, attName, attVal;
            ss >> cLess >> tagName >> attName >> cEqual >> attVal;
            //std::cout << "cLess=" << cLess << " tagName=" << tagName << " attName=" << attName << " attVal=" << attVal << std::endl;
            
            tagStack.push_back(tagName);
            std::string tags = accumulateTagNames(tagStack);  
            mAtt2Val[tags+"~"+attName] = attVal;
            //std::cout << "Item " << tags+"~"+attName << std::endl;
            while (ss) {
                ss >> attName >> cEqual >> attVal;
                mAtt2Val[tags+"~"+attName] = attVal;
            }
        }
    }
    // Queries 
    for (int i = 0; i < q; ++i) {
        std::string line;
        std::getline(std::cin, line);
        if (mAtt2Val.find(line) != mAtt2Val.end())
            std::cout << mAtt2Val[line] << std::endl;
        else 
            std::cout << "Not Found!\n";
    }
    return 0;
}
