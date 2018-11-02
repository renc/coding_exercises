#include <iostream> 
#include <vector> 
#include <memory>

// Definition for singly-linked list.
 struct ListNode {
      int val;
      std::shared_ptr<ListNode> next;
      ListNode(int x) : val(x), next(nullptr) {}
  };
 
 std::shared_ptr<ListNode> buildSinglyLinkedList(const std::vector<int> &vals)
 {
     std::shared_ptr<ListNode> pHead = nullptr;
     std::shared_ptr<ListNode> pCurr = pHead;
     for (auto e : vals) {
         std::shared_ptr<ListNode> pNew = std::make_shared<ListNode>(e);
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
 std::vector<int> getLinkedListValues(std::shared_ptr<ListNode> pHead) 
 {
     std::vector<int> ret;
     while (pHead != nullptr)
     {
         ret.push_back(pHead->val);
         pHead = pHead->next;
     }
     return ret;
 }
 void printLinkedList(std::shared_ptr<ListNode> pHead)
 {
     std::vector<int> vals = getLinkedListValues(pHead);
     std::cout << "LL: ";
     for (auto e : vals) {
         std::cout << e << ", ";
     }
     std::cout << std::endl;
 }

 int main()
 {
     std::vector<int> vals{1, 3, 2, 5, 6, 0};
     std::shared_ptr<ListNode> pL1  = buildSinglyLinkedList(vals);
     printLinkedList(pL1);
     std::vector<int> vals2{0, 3, 2, 15, 1};
     std::shared_ptr<ListNode> pL2  = buildSinglyLinkedList(vals2);
     printLinkedList(pL2);
     return 1;
 }