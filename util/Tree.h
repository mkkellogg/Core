#pragma once

#include <vector>
#include <stack>
#include <string>
#include <new>

#include "../common/Exception.h"

namespace Core {

    template <typename T> class Tree {
    public:

        class TreeNode {
            friend class Tree;
            std::vector<TreeNode *> children;
            TreeNode * parent = nullptr;

        public:

            T Data;

            TreeNode() {}
            TreeNode(T data) : Data(data) {}

            TreeNode* getChild(UInt32 index) {
                if (index >= getChildCount())return nullptr;
                return children[index];
            }

            UInt32 getChildCount() {
                return children.size();
            }

            TreeNode* addChild(T data) {
                TreeNode * newNode = TreeNode::createNode(data);
                newNode->parent = this;
                children.push_back(newNode);
                return newNode;
            }

            TreeNode* addChild(TreeNode * node) {
                node->parent = this;
                children.push_back(node);
                return node;
            }

            TreeNode* getParent() {
                return parent;
            }

            static TreeNode* createNode(T data) {
                TreeNode * newNode = new(std::nothrow) TreeNode(data);
                if (newNode == nullptr) {
                    throw AllocationException("TreeNode::createNode() -> Unable to allocate new node!");
                }
                return newNode;
            }
        };

        Tree(): root(nullptr), fullTraversal(false) {}
        ~Tree() {}

        TreeNode* getRoot() {
            return root;
        }

        TreeNode* addRoot(T data) {
            if (!root) {
                root = TreeNode::createNode(data);
            }
            return root;
        }

        void setTraversalCallback(std::function<Bool(TreeNode*)> callback) {
            this->traversalCallback = callback;
        }

        void traverse() {
            if (traversalCallback && root != nullptr) {
                fullTraversal = false;
                internalTraverse();
            }
        }

    private:

        TreeNode * root;
        Bool fullTraversal;
        std::function<Bool(TreeNode*)> traversalCallback;
        std::function<Bool(TreeNode*)> fullTraversalCallback;

        void fullTraverse() {
            if (root != nullptr && fullTraversalCallback) {
                fullTraversal = true;
                internalTraverse();
            }
        }

        // do post-order traversal
        void internalTraverse() {
            std::stack<TreeNode *> iterationStack;
            std::stack<UInt32> childIndexStack;
            if (root != nullptr) {
                iterationStack.push(root);
                childIndexStack.push(0);

                while (iterationStack.size() > 0) {
                    TreeNode * current = iterationStack.top();
                    UInt32 childIndex = childIndexStack.top();

                    if (childIndex < current->children.size()) {
                        TreeNode * next = current->children[childIndex];

                        if (next) {
                            childIndexStack.pop();
                            childIndexStack.push(childIndex + 1);

                            iterationStack.push(next);
                            childIndexStack.push(0);
                        }
                        else {
                            // Ignore null node
                        }
                    }
                    else {
                        Bool continueTraversal = true;
                        if (fullTraversal) {
                            if (fullTraversalCallback) {
                                continueTraversal = fullTraversalCallback(current);
                            }
                        }
                        else {
                            if (traversalCallback) {
                                continueTraversal = traversalCallback(current);
                            }
                        }
                        if (!continueTraversal)break;

                        iterationStack.pop();
                        childIndexStack.pop();
                    }
                }
            }
        }
    };
}
