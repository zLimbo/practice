#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>
#include <assert.h>
#include <zl/debug.h>
using namespace std;


struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int val_,
             TreeNode* left_ = nullptr,
             TreeNode* right_ = nullptr):
        val(val_), left(left_), right(right_) {}
};


class AVLTree {

private:

    TreeNode* root = nullptr;

    int height(TreeNode* tn) {
        if (!tn) return 0;
        return 1 + max(height(tn->left), height(tn->right));
    }

    int getBalance(TreeNode* tn) {
        return height(tn->left) - height(tn->right);
    }

    TreeNode* leftRotate(TreeNode* parent) {
        TreeNode* child = parent->right;
        parent->right = child->left;
        child->left = parent;
        return child;
    }

    TreeNode* rightRotate(TreeNode* parent) {
        TreeNode* child = parent->left;
        parent->left = child->right;
        child->right = parent;
        return child;
    }

    TreeNode* insert(TreeNode* tn, int val) {
        if (!tn) return new TreeNode(val);
        if (val < tn->val) tn->left = insert(tn->left, val);
        else if (val > tn->val) tn->right = insert(tn->right, val);

        int balance = getBalance(tn);

        if (balance > 1) {
            if (val > tn->left->val) tn->left = leftRotate(tn->left);
            return rightRotate(tn);
        }
        if (balance < -1) {
            if (val < tn->right->val) tn->right = rightRotate(tn->right);
            return leftRotate(tn);
        }

        return tn;
    }

    TreeNode* remove(TreeNode* tn, int val) {
        if (!tn) return nullptr;
        if (val < tn->val) tn->left = remove(tn->left, val);
        else if (val > tn->val) tn->right = remove(tn->right, val);

        return tn;
    }

public:

    int height() {
        return height(root);
    }

    void insert(int val) {
        root = insert(root, val);
    }

    void remove(int val) {
        root = remove(root, val);
    }

    void show(TreeNode* tn, string prefix = "", bool isLeft = true) {
        if (tn->right) {
            show(tn->right, prefix + (isLeft ? "┃    " : "     "), false);
        }
        cout << prefix + (isLeft ? "┗━━━ " : "┏━━━ ") << tn->val << endl;
        if (tn->left) {
            show(tn->left, prefix + (isLeft ? "     " : "┃    "), true);
        }
    }

    void show() {
        show(root);
    }
};

int main() {

    AVLTree avl;

    for (int i = 0; i < 10; ++i) {
        int val = rand() % 100;
        cout << val << " ";
        avl.insert(val);
    }
    cout << endl;
	print(RED, "height: ", avl.height());

    cout << "\n" << endl;
    avl.show();
    cout << "\n" << endl;

    return 0;
}

