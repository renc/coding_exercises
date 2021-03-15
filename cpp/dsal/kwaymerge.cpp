
// reference:
// https://www.geeksforgeeks.org/merge-k-sorted-arrays-set-2-different-sized-arrays/ 
// 

#include <vector>
#include <iostream>
#include <queue> // for std::priority_queue

void print(const std::vector<int> &a)
{
    for (auto e: a)
    {
        std::cout << e << ", ";
    }
    std::cout << std::endl;
}
struct Element {
    Element(int v, int ia, int ie) 
        : iValue(v), iArrayIdx(ia), iIdxInArray(ie) {}

    int iValue;
    int iArrayIdx;
    int iIdxInArray;
};
class ElementCompare {
    public:
    bool operator()(Element a, Element b) {
        if (a.iValue <= b.iValue)
            return false;
        return true;
    } // to simulate the std::greater<T>, to get the smallest element at top().
};
// https://en.cppreference.com/w/cpp/container/priority_queue
// by default, the Compare is std::less<T>, the result top() is the largest element.
// 
// A user-provided Compare can be supplied to change the ordering,
//  e.g. using std::greater<T> would cause the smallest element to appear as the top().
// 
// Note that the Compare parameter is defined such that it returns true 
// if its first argument comes before its second argument in a weak ordering. 
// But because the priority queue outputs largest elements first, 
// the elements that "come before" are actually output last. 
// That is, the front of the queue contains the "last" element according to the weak ordering imposed by Compare. 

std::vector<int> mergeKArrays(const std::vector<std::vector<int> > &arr)
{
    std::vector<int> ret;

    // renc, it should be myself to implement the min heap, but directly use stl here.
    std::priority_queue<Element, std::vector<Element>, ElementCompare > pq;

    // push the first element of each array into heap
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr[i].size()) {
            Element ele(arr[i][0], i, 0);
            pq.push(ele);
        } 
    }

    while (pq.size())
    {
        Element ele = pq.top();
        ret.push_back(ele.iValue);
        pq.pop();
        print(ret);

        if (ele.iIdxInArray + 1 < arr[ele.iArrayIdx].size()) {
            Element nextEle( arr[ele.iArrayIdx][ele.iIdxInArray+1], ele.iArrayIdx, ele.iIdxInArray+1);
            pq.push(nextEle);
        }
    }
    return ret;
}

// Driver program to test above functions 
int main() 
{ 
    // Change n at the top to change number 
    // of elements in an array 
    std::vector<std::vector<int> > arr{ { 2, 6, 12 }, 
                              { 1, 9 }, 
                              { 23, 34, 90, 2000 } }; 
  
    std::vector<int> output = mergeKArrays(arr); 
  
    std::cout << "Merged array is " << std::endl; 
    for (auto x : output) 
        std::cout << x << " "; 
    std::cout << std::endl;
    return 0; 
} 