#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>
#include <assert.h>
#include <zl/debug.h>
using namespace zl;
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

	TreeNode* minValue(TreeNode* tn) {
		while (tn->left) tn = tn->left;
		return tn;
	}

    TreeNode* leftRotate(TreeNode* parent) {
        TreeNode* child = parent->right;
        parent->right = child->left;
        child->left = parent;

        if (parent->parent) {
            if (parent == parent->parent->left) 
                parent->parent->left = child;
            else 
                parent->parent->right = child;
        } 
		else {
            root = child;
			child->parent = nullptr;
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
        } 
		else {
            root = child;
			child->parent = nullptr;
        }
        child->parent = parent->parent;
        parent->parent = child;
        if (parent->left) parent->left->parent = parent;

        return child;
    }

    void insertModifyColor(TreeNode* hot) {
		cout << "Insert: " << hot->val << " " << (hot->parent ? hot->parent->val : -1) << endl;
		show();
        if (hot == root) {
            hot->color = TreeNode::BLACK;
            return;
        }
        TreeNode* parent = hot->parent;
        if (hot->color == TreeNode::RED && parent->color == TreeNode::RED) {
            TreeNode* grandparent = parent->parent;
            TreeNode* uncle = parent == grandparent->left ? grandparent->right : grandparent->left;
            if (uncle && uncle->color == TreeNode::RED) {
                parent->color = uncle->color = TreeNode::BLACK;
                grandparent->color = TreeNode::RED;
                insertModifyColor(grandparent);
            } 
			else {
                if (parent == grandparent->left) {
                    if (hot == parent->right) leftRotate(parent);
                    grandparent = rightRotate(grandparent);
                    grandparent->color = TreeNode::BLACK;
                    grandparent->right->color = TreeNode::RED;
                } 
				else {
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
        } 
		else if (hot->val > tn->val) {
            tn->right = insert(tn->right, hot);
            tn->right->parent = tn;
        }
		else {
			TreeNode* tmp = tn;
			hot->color = tn->color;
			tn = hot;
			delete tmp;
		}
        return tn;
    }

	void removeModifyColor(TreeNode* hot) {
		cout << "modify: " << hot->val << endl;
		show();
		if (hot == root) return;	
		if (hot->color == TreeNode::RED) {
			hot->color = TreeNode::BLACK;
			return;
		}
		// hot->color == TreeNode::BLACK
		TreeNode* parent = hot->parent;
		if (hot == parent->left) { // RR, RL
			TreeNode* brother = parent->right;
			if (brother->color == TreeNode::RED) {
				brother->color = parent->color;
				parent->color = TreeNode::RED;
				leftRotate(parent);
				removeModifyColor(hot); // traceback
			}	
			else { // brother->color == TreeNode::BLACK
				TreeNode* lc = brother->left;
				TreeNode* rc = brother->right;
				if (rc && rc->color == TreeNode::RED) { // RR
					brother->color = parent->color;
					parent->color = rc->color = TreeNode::BLACK;
					leftRotate(parent);
				}
				else if (lc && lc->color == TreeNode::RED) { // RL
					lc->color = parent->color;
					parent->color = TreeNode::BLACK;
					rightRotate(brother);
					leftRotate(parent);
				}
				else { // traceback 兄弟节点子节点都为黑色
					brother->color = TreeNode::RED;
					removeModifyColor(parent);
				}
			}
		} 
		else { // hot == parent->right LL, LR
			TreeNode* brother = parent->left;
			if (brother->color == TreeNode::RED) {
				brother->color = parent->color;
				parent->color = TreeNode::RED;
				rightRotate(parent);
				removeModifyColor(hot);
			}
			else { // brother->color == TreeNode::BLACK
				TreeNode* lc = brother->left;
				TreeNode* rc = brother->right;
				if (lc && lc->color == TreeNode::RED) {
					brother->color = parent->color;
					parent->color = lc->color = TreeNode::BLACK;
					rightRotate(parent);
				}
				else if (rc && rc->color == TreeNode::RED) {
					rc->color = parent->color;
					parent->color = TreeNode::BLACK;
					leftRotate(brother);
					cout << "leftRotate: " << endl;
					show();
					rightRotate(parent);
					cout << "rightRotate: " << endl;
					show();
				}
				else {
					brother->color = TreeNode::RED;
					removeModifyColor(parent);
				}
			}
		}
	}
	
    void remove(TreeNode* tn, int val) {
		if (!tn) return;
		if (val < tn->val) remove(tn->left, val);
		else if (val > tn->val) remove(tn->right, val);
		else {
			if (tn->left && tn->right) {
				TreeNode* succ = minValue(tn->right);
				tn->val = succ->val;
				remove(tn->right, succ->val);
			}
			else { // tn->left == nullptr || tn->right == nullptr
				TreeNode* child = tn->left ? tn->left : tn->right;
				if (child && child->color == TreeNode::RED) // BLACK, RED
					child->color = TreeNode::BLACK; 
				else if (tn->color == TreeNode::BLACK) {// BLACK, BLACK
					removeModifyColor(tn); 
					cout << "Modify successed" << endl;
					show();
				}
				if (tn->parent) {
					if (child) child->parent = tn->parent;
					if (tn == tn->parent->left) tn->parent->left = child;
					else tn->parent->right = child;	
				}
				else {
					root = child;
					if (child) child->parent = nullptr;
				}
				delete tn;	
			}
		}
    }

	int size(TreeNode* tn) {
		if (!tn) return 0;
		return 1 + size(tn->left) + size(tn->right);
	}

    void show(TreeNode* tn, string prefix = "", bool isLeft = true) {
		if (!tn) return;
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

public:

    void insert(int val) {
        TreeNode* hot = new TreeNode(val, TreeNode::RED);
        root = insert(root, hot);
        insertModifyColor(hot);
    }

    void remove(int val) {
        remove(root, val);
    }

	int size() {
		return size(root);
	}

    void show() {
		cout << size() << endl;
        show(root);
    }
};

int main() {

    RBTree rbt;

	vector<int> v;
    for (int i = 0; i < 2000; ++i) {
        int val = rand() % 1000;
		v.push_back(val);
        cout << val << " ";
        rbt.insert(val);
    }
    cout << endl;

    cout << "\n" << endl;
    rbt.show();
    cout << "\n" << endl;

    string op, num;
    cout << ">>> ";
    //while (cin >> op >> num) {
	for (int i = 0; i < 10000; ++i) {
		 op = rand() % 2 ? "-" : "+";
		int x = rand() % 1000;
	//for (int x: v) {
		op = "-";
		num = to_string(x);
		cout << op << " " << num << endl;
        if (op == "+") {
            rbt.insert(stoi(num));
        } 
		else if (op == "-") {
            rbt.remove(stoi(num));
        } 
		else {
            break;
        }

        cout << "\n" << endl;
        rbt.show();
        cout << "\n" << endl;
        cout << ">>> ";
    }
	cout << "done !" << endl;

    return 0;
}

