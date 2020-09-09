#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>
#include <assert.h>
#include <zl/debug.h>
using namespace std;


struct TreeNode {
    enum COLOR { RED, BLACK };

    int val;
    COLOR color;
    TreeNode *parent, *left, *right;
    TreeNode(int val_,
             COLOR color_ = RED,
             TreeNode* parent_ = nullptr,
             TreeNode* left_ = nullptr,
             TreeNode* right_ = nullptr):
        val(val_), color(color_), parent(parent_), left(left_), right(right_) {}
};


class RBTree {

private:

    TreeNode* root = nullptr;

    TreeNode* leftRotate(TreeNode* parent) {
        TreeNode* child = parent->right;
        parent->right = child->left;
        child->left = parent;

        if (parent->parent) {
            if (parent == parent->parent->left) 
                parent->parent->left = child;
            else 
                parent->parent->right = child;
        } else {
            root = child;
        }
        child->parent = parent->parent;
        parent->parent = child;
        if (parent->right) parent->right->parent = parent;

        return child;
    }

    TreeNode* rightRotate(TreeNode* parent) {
        TreeNode* child = parent->left;
        parent->left = child->right;
        child->right = parent;

        if (parent->parent) {
            if (parent == parent->parent->left) 
                parent->parent->left = child;
            else 
                parent->parent->right = child;
        } {
            root = child;
        }
        child->parent = parent->parent;
        parent->parent = child;
        if (parent->left) parent->left->parent = parent;

        return child;
    }

    void modifyColor(TreeNode* hot) {
        if (hot == root) {
            hot->color = TreeNode::BLACK;
            show();
            return;
        }
        TreeNode* parent = hot->parent;
        if (hot->color == TreeNode::RED && parent->color == TreeNode::RED) {
            TreeNode* grandparent = parent->parent;
            TreeNode* uncle = parent == grandparent->left ? grandparent->right : grandparent->left;
            if (uncle && uncle->color == TreeNode::RED) {
                parent->color = uncle->color = TreeNode::BLACK;
                grandparent->color = TreeNode::RED;
                modifyColor(grandparent);
            } else {
                if (parent == grandparent->left) {
                    if (hot == parent->right) leftRotate(parent);
                    grandparent = rightRotate(grandparent);
                    grandparent->color = TreeNode::BLACK;
                    grandparent->right->color = TreeNode::RED;
                } else {
                    if (hot == parent->left) rightRotate(parent);
                    grandparent = leftRotate(grandparent);
                    grandparent->color = TreeNode::BLACK;
                    grandparent->left->color = TreeNode::RED;
                }
            }
        }
    }

    TreeNode* insert(TreeNode* tn, TreeNode* hot) {
        if (!tn) return hot;
        if (hot->val < tn->val) {
            tn->left = insert(tn->left, hot);
            tn->left->parent = tn;
        } else if (hot->val > tn->val) {
            tn->right = insert(tn->right, hot);
            tn->right->parent = tn;
        }

        return tn;
    }

    TreeNode* remove(TreeNode* tn, int val) {
        // if (!tn) return nullptr;
        // if (val < tn->val) tn->left = remove(tn->left, val);
        // else if (val > tn->val) tn->right = remove(tn->right, val);
        // else {
        //     if (tn->left == nullptr || tn->right == nullptr) {
        //         TreeNode* child = tn->left ? tn->left : tn->right;
        //         delete tn;
        //         tn = child;
        //     } else {
        //         TreeNode* succ = minValue(tn->right);
        //         tn->val = succ->val;
        //         tn->right = remove(tn->right, succ->val);
        //     }
        // }

        // if (!tn) return nullptr;

        // updateHeight(tn);
        // tn = regainBalance(tn);

        return tn;
    }

public:

    void insert(int val) {
        TreeNode* hot = new TreeNode(val);
        root = insert(root, hot);
        modifyColor(hot);
    }

    void remove(int val) {
        //root = remove(root, nullptr, val);
    }

    void show(TreeNode* tn, string prefix = "", bool isLeft = true) {
        if (tn->right) {
            show(tn->right, prefix + (isLeft ? "┃    " : "     "), false);
        }
        cout << prefix + (isLeft ? "┗━━━ " : "┏━━━ ");
        print(tn->color == TreeNode::RED ? RED : BLACE, 
                to_string(tn->val) + "[" + to_string(tn->parent ? tn->parent->val : -1) + "]");
        if (tn->left) {
            show(tn->left, prefix + (isLeft ? "     " : "┃    "), true);
        }
    }

    void show() {
        show(root);
    }
};

int main() {

    RBTree rbt;

    for (int i = 0; i < 10; ++i) {
        int val = rand() % 1000;
        cout << val << " ";
        rbt.insert(val);
    }
    cout << endl;

    cout << "\n" << endl;
    rbt.show();
    cout << "\n" << endl;

    string op, num;
    cout << ">>> ";
    while (cin >> op >> num) {
        if (op == "+") {
            rbt.insert(stoi(num));
        } else if (op == "-") {
            rbt.remove(stoi(num));
        } else {
            break;
        }

        cout << "\n" << endl;
        rbt.show();
        cout << "\n" << endl;
        cout << ">>> ";
    }

    return 0;
}

