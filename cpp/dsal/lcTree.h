#ifndef __leetcode_tree_h__
#define __leetcode_tree_h__

#include <set>
#include <stack>
#include <vector>
#include <algorithm> // std::reverse 


// Definition for a binary tree node.
  struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 };
 
class Solution {
public:
    // leetcode 94. binary tree inorder traversal , medium 
    // https://leetcode.com/problems/binary-tree-inorder-traversal 
    // renc, inorder LPR (only for binary tree, general tree wo inorder).
    // recursive -> iteration with stack, P stay at the top until L visited.
    std::vector<int> inorderTraversal(TreeNode* root) {
        std::vector<int> result;
        
        if (root == nullptr)
            return result; 
        
        std::set<TreeNode *> visited;
        
        std::stack<TreeNode *> s;
        s.push(root);
        
        while (s.size())
        {
            TreeNode *n = s.top();
            TreeNode *l = n->left;
            TreeNode *r = n->right;
            
            if (l == nullptr || (l && visited.count(l)) )
            {
                // left is visited, to visit myself now
                visited.insert(n);
                s.pop();
                result.push_back(n->val);
                
                if (r)
                    s.push(r);
            }
            if (l && visited.count(l) == 0)
            {
                // not visited
                s.push(l);
            } 
        }
        
        return result;
    }

    // leetcode 100, same tree, easy 
    // https://leetcode.com/problems/same-tree/
    bool isSameNode(TreeNode *a, TreeNode *b)
    {
        if (a == nullptr && b == nullptr)
            return true;
        if (a == nullptr && b)
            return false;
        if (a && b == nullptr)
            return false;
            
        
        if (a && b )
            if (a->val != b->val) 
                return false; 
        
        return isSameNode(a->left, b->left) && isSameNode(a->right, b->right);
    }
    bool isSameTree(TreeNode* p, TreeNode* q) {
        return isSameNode(p, q);    
    }
    // leetcode 103, binary tree zigzag level order traversal, medium
    // https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal/
    // renc, do the easy 107 first, 
    std::vector<std::vector<int> > zigzagLevelOrder(TreeNode* root) {
        std::vector<std::vector<int> > result;
        
        if (root == nullptr)
            return result;
            
        std::vector<TreeNode *> layer;
        layer.push_back(root);
        
        int layerIndex = -1; // left to right; 
        while (layer.size())
        {
            std::vector<TreeNode *> newLayer;
            std::vector<int> layerValue; 
            for (unsigned int i = 0; i < layer.size(); ++i)
            {
                TreeNode *c = layer[i];
                layerValue.push_back(c->val);
                
                if (c->left == nullptr && c->right == nullptr)
                {
                    // a leaf  
                }
                else
                {
                    if (c->left)
                    {
                        newLayer.push_back(c->left); 
                    }
                    if (c->right)
                    {
                        newLayer.push_back(c->right); 
                    }
                }
            }
            if (layerIndex == -1) {} // left to right
            else
            {
                std::reverse(layerValue.begin(), layerValue.end());
            }
            
            result.push_back(layerValue);
            
            // ready to next layer         
            layer = newLayer;
            layerIndex *= -1; 
        }
        return result;
    }
    // leetcode 107, binary tree level order traversal 2, inverse order. easy
    // https://leetcode.com/problems/binary-tree-level-order-traversal-ii/
    // renc, from top to bottom, layer to layer, but inverse the result.
    std::vector<std::vector<int> > levelOrderBottom(TreeNode* root) {
        std::vector<std::vector<int> > result;
        
        if (root == nullptr)
            return result;
            
        std::vector<TreeNode *> layer;
        layer.push_back(root);
        
        while (layer.size())
        {
            std::vector<TreeNode *> newLayer;
            std::vector<int> layerValue; 
            for (unsigned int i = 0; i < layer.size(); ++i)
            {
                TreeNode *c = layer[i];
                layerValue.push_back(c->val);
                
                if (c->left == nullptr && c->right == nullptr)
                {
                    // a leaf  
                }
                else
                {
                    if (c->left)
                    {
                        newLayer.push_back(c->left); 
                    }
                    if (c->right)
                    {
                        newLayer.push_back(c->right); 
                    }
                }
            }
            result.insert(result.begin(), layerValue); // insert ahead,
            
            layer = newLayer;
        }
        return result;
    }

    // leetcode 863, All nodes distance k in binary tree, medium , tree, 
    // https://leetcode.com/problems/all-nodes-distance-k-in-binary-tree/
    // renc, 1. start at the seed node to children, BFS find k steps/layers.
    // 2. start at the root dfs to seed node, record the path, 
    // find k-1 steps for seed's parent 
    // find k-2 steps for seed's parent parent. 
    // ... 
};

#endif 