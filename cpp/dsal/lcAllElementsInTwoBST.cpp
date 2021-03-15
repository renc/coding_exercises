#include <iostream>
#include <vector> 
namespace allelementsintwobst {
// tag: bst, tree, binarysearch, 	
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

using namespace std;
class Solution {
public:
    vector<int> getAllElements(TreeNode* root1, TreeNode* root2) {
        std::vector<int> arr1, arr2;
        treeTraverseLPR(root1, arr1);
        treeTraverseLPR(root2, arr2);
        
        for (auto e: arr1) {std::cout << e << ", "; } std::cout << ".\n";
        for (auto e: arr2) {std::cout << e << ", "; } std::cout << ".\n";
        
        std::vector<int> result = mergeArr(arr1, arr2);
        for (auto e: result) {std::cout << e << ", "; } std::cout << ".\n";
        return result;
    }
    void treeTraverseLPR(TreeNode* node,std::vector<int> &arr) {
        if (node == nullptr) return;
         
        treeTraverseLPR(node->left, arr);
        arr.push_back(node->val);
        treeTraverseLPR(node->right, arr);
         
    }
    std::vector<int> mergeArr(const std::vector<int> &arr1, const std::vector<int> &arr2) {
        std::vector<int> result; 
        int l=0, r=0; 
        for (; l< arr1.size() && r< arr2.size();) {
            if (arr1[l] < arr2[r]) {
                result.push_back(arr1[l]);
                ++l;
            } else {
                result.push_back(arr2[r]);
                ++r; 
            }
        }
        if (l == arr1.size() ) {
            for (; r < arr2.size(); ++r) {
                result.push_back(arr2[r]);
            }
        }        
        if (r == arr2.size() ) {
            for (; l < arr1.size(); ++l) {
                result.push_back(arr1[l]);
            }
        }
        return result;
    }
};	
	
	
}; 	