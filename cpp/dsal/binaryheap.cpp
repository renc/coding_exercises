
// to build: 
// $ g++ ./dsal/binaryheap.cpp -o ./build/binaryheap -std=c++11 -g
// -g for debug
//
// reference
// https://www.geeksforgeeks.org/heap-sort/


#include <vector>
#include <iostream>
#include <math> // for std::floor std::ceil 

void print(const std::vector<int> &a)
{
    for (auto e: a)
        std::cout << e << ", ";
    std::cout << std::endl;
}
// 0 1 2 3 
//     0
//   1   2 
// 3  
// using array(std::vector) to implement heap,
// compare to use pointer node,  wo using two pointers for left/right child.
class BinaryHeap 
{
    public:
    BinaryHeap(const std::vector<int> &arr, bool bAsMaxHeap = true)
    {
        m_value = arr;
        m_iEnd = m_value.size();
        m_bMax = bAsMaxHeap;

        for (int i = m_value.size() / 2; i >= 0; --i) // renc, difficult point, heapify from child to parent
            heapifyDown(i);
        std::cout << "Ctor: "; print(m_value);
    }
    int topValue() const { return m_value.front(); } // MaxHeap, front() is the max;
    bool popTopValue() 
    {
        // maxheap: move the largest to last, -> ascending sorting.
        // minheap: move the smallest to last, -> decending sorting.
        if (m_iEnd >= 1) {
            int iMaxValue = topValue();
            m_value[0] = m_value[m_iEnd -1]; // move the last to head.
            m_value[m_iEnd -1] = iMaxValue; // put the max to the last 

            --m_iEnd; // do this before heapify, since the heapify use this value to check
            heapifyDown(0);
            std::cout << "After popTopValue:"; print(m_value);
            return true;
        }
        return false;
    }
    void heapSort()
    {
        int iSize = m_value.size();
        for (int i = 0; i < iSize; ++i) {
            popTopValue();
        }
        std::cout << "After heapSort: "; print(m_value);
    }
    void insert(int val)
    {
        m_value.push_back(val);
        ++m_iEnd;
        
        heapifyUp(m_value.size() -1);
    }
    int left(int i) const { return 2*i + 1;}
    int right(int i) const  { return 2*i + 2;}
    int parent(int i) const { return static_cast<int>(std::floor((i - 1)/2)); } 
private:
    std::vector<int> m_value;
    int m_iEnd = -1;    // 
    bool m_bMax = true; // true: maxheap; false: minheap.

    void heapifyDown(int i)
    {
        int iL = left(i);
        int iR = right(i);
        int iMaxIdx = i; // to find the max child for maxheap, 
        if (iL < m_iEnd && (m_bMax ? m_value[iL] > m_value[iMaxIdx] : m_value[iL] < m_value[iMaxIdx]) )
            iMaxIdx = iL;
        if (iR < m_iEnd && (m_bMax ? m_value[iR] > m_value[iMaxIdx] : m_value[iR] < m_value[iMaxIdx]))
            iMaxIdx = iR;
        
        if (iMaxIdx != i) {
            std::swap(m_value[i], m_value[iMaxIdx]);
            heapifyDown(iMaxIdx);
        }
    } 
    void heapifyUp(int i)
    {
        if (i <= 0)
            return;
        const int iParent = parent(i);
        if (iParent >=0) {
            if (m_bMax) {
                if (m_value[iParent] < m_value[i])
                {
                    std::swap(m_value[iParent], m_value[i]);
                    heapifyUp(iParent);
                }
            }
            else {
                if (m_value[iParent] > m_value[i])
                {
                    std::swap(m_value[iParent], m_value[i]);
                    heapifyUp(iParent);
                }
            }
        }
    }
};

int main(int argc, char const *argv[])
{
    {
        std::vector<int> arr {12, 11, 13, 5, 6, 7};
        std::cout << "Init arr:"; print(arr);
        BinaryHeap mh (arr);
        mh.heapSort();
    }
    {
        std::vector<int> arr {0};
        std::cout << "Init arr:"; print(arr);
        BinaryHeap mh (arr);
        mh.heapSort();
    }
    {
        std::vector<int> arr {12, 11, 9, 5, 4,-1};
        std::cout << "Init arr:"; print(arr);
        BinaryHeap mh (arr, false);
        mh.heapSort();
    }

    return 0;
}
