#include <iostream> 
#include <vector> 
#include <memory>

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

struct ListNodeSP {
    int val;
    std::shared_ptr<ListNodeSP> next;
    ListNodeSP(int x) : val(x), next(nullptr) {}
};

ListNode* buildSinglyLinkedList(const std::vector<int> &vals)
{
    ListNode* pHead = nullptr;
    ListNode* pCurr = pHead;
    for (auto e : vals) {
        ListNode* pNew = new ListNode(e);
        if (pHead == nullptr) {
            pHead = pNew;
            pCurr = pNew;
        } else {
            pCurr->next = pNew;
            pCurr = pCurr->next;
        }
    }
    return pHead; 
}
std::vector<int> getLinkedListValues(ListNode  *pHead) 
{
    std::vector<int> ret;
    while (pHead != nullptr)
    {
        ret.push_back(pHead->val);
        pHead = pHead->next;
    }
    return ret;
}
void printLinkedList(ListNode* pHead)
{
    std::vector<int> vals = getLinkedListValues(pHead);
    std::cout << "LL: ";
    for (auto e : vals) {
        std::cout << e << ", ";
    }
    std::cout << std::endl;
}

// https://leetcode.com/problems/reverse-linked-list-ii/
ListNode* reverseBetween(ListNode* head, int m, int n) {
    ListNode* newHead = head;

    int idx = 1;
    ListNode* pM_1 = nullptr; // point to m-1
    ListNode* pM{ nullptr };
    ListNode* pPrev{ nullptr }, * pCurr{ nullptr }; // 
    while (head) {
        if (idx < m) {
            pM_1 = head;
            head = head->next;
            ++idx;
        }
        else if (idx == m) { // also handle m==n
            pM = head;
            pPrev = head;

            head = head->next;
            ++idx;
        }
        else if (m < idx && idx < n) {
            ListNode* pNext = head->next;
            head->next = pPrev;
            pPrev = head;
            head = pNext;
            ++idx;
        }
        else if (idx == n) {
            ListNode* pNplus1 = head->next;
            pM->next = pNplus1;
            head->next = pPrev;

            if (pM_1) {
                pM_1->next = head;
            }
            else {
                newHead = head;
            }
            break;
        }
        else if (idx > n) {
            break;
        }
    }
    return newHead;
}

int main()
{
    std::vector<int> vals{1, 3, 2, 5, 6, 0};
    ListNode* pL1  = buildSinglyLinkedList(vals);
    printLinkedList(pL1);
    std::vector<int> vals2{0, 3, 2, 15, 1};
    ListNode* pL2  = buildSinglyLinkedList(vals2);
    printLinkedList(pL2);

    {
        std::vector<int> vals{ 1,2,3,4,5 };
        ListNode* head = buildSinglyLinkedList(vals);
        ListNode* pNewHead = reverseBetween(head, 2, 4);
    }
    {
        std::vector<int> vals{ 3,5 };
        ListNode* head = buildSinglyLinkedList(vals);
        ListNode* pNewHead = reverseBetween(head, 1, 1);
    }
    return 1;
}