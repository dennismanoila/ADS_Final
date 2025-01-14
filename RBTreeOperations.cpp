#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include "IODialog.h"
#include "RBTreeOperations.h"

using namespace std;

struct RBNode {
    int key;
    RBNode* parent;
    RBNode* left;
    RBNode* right;
    enum Color { RED, BLACK } color;

    RBNode(int k = 0, RBNode* p = nullptr, RBNode* l = nullptr, RBNode* r = nullptr, Color c = BLACK)
            : key(k), parent(p), left(l), right(r), color(c) {}

    string toString() {
        return to_string(key) + (color == RED ? ":r" : ":b");
    }
};

RBNode* NIL = new RBNode(0);

struct RBTree {
    RBNode* root;

    RBTree() : root(NIL) {}
    ~RBTree() { deleteSubtree(root); }

    void RBInsert(int key);
    void RBDelete(RBNode* z);
    RBNode* search(RBNode* x, int key);
    RBNode* minimum(RBNode* x);
    RBNode* maximum(RBNode* x);
    RBNode* successor(RBNode* x);
    RBNode* predecessor(RBNode* x);
    void inorder(RBNode* x);
    void indentedDisplay(RBNode* x, int indent);
    int blackHeight(RBNode* x);
    int maxBlackKey();
    int maxRedKey();
    int depth();
    int maxBlackKeyHelper(RBNode* node, int& maxKey);
    int maxRedKeyHelper(RBNode* node, int& maxKey);
    int calculateDepth(RBNode* node);
    double blackNodePercentage();
    RBNode* minimumRed();
    RBNode* minimumBlack();
    std::vector<int> pathToKey(int key);

    int countRedNodes() {return countRedNodesHelper(root);}
    void inorder() { inorder(root); }
    void indentedDisplay() { indentedDisplay(root, 0); }
    int blackHeight() { return blackHeight(root); }
    int countBlackNodes() { return countBlackNodesHelper(root); }


private:
    void deleteSubtree(RBNode* x);
    void RBInsertFixup(RBNode* z);
    void RBDeleteFixup(RBNode* x);
    void leftRotate(RBNode* x);
    void rightRotate(RBNode* x);
    int countRedNodesHelper(RBNode* node);
    int countBlackNodesHelper(RBNode* node);
    int totalNodesHelper(RBNode* node);

};

bool findPathToKeyHelper(RBNode* node, int key, std::vector<int>& path) {
    if (node == NIL) return false;

    path.push_back(node->key);

    if (node->key == key) return true;

    if (key < node->key) {
        if (findPathToKeyHelper(node->left, key, path)) return true;
    } else {
        if (findPathToKeyHelper(node->right, key, path)) return true;
    }

    path.pop_back();
    return false;
}

std::vector<int> RBTree::pathToKey(int key) {
    std::vector<int> path;
    if (!findPathToKeyHelper(root, key, path)) {
        std::cout << "Key " << key << " not found in the tree.\n";
        return {};
    }
    return path;
}

RBNode* findMinimumRedHelper(RBNode* node, RBNode*& minRedNode) {
    if (node == NIL) return minRedNode;
        findMinimumRedHelper(node->left, minRedNode);

    if (node->color == RBNode::RED && (minRedNode == nullptr || node->key < minRedNode->key)) {
        minRedNode = node;
    }

    findMinimumRedHelper(node->right, minRedNode);

    return minRedNode;
}

RBNode* RBTree::minimumRed() {
    RBNode* minRedNode = nullptr;
    return findMinimumRedHelper(root, minRedNode);
}

RBNode* findMinimumBlackHelper(RBNode* node, RBNode*& minBlackNode) {
    if (node == NIL) return minBlackNode;

    findMinimumBlackHelper(node->left, minBlackNode);

    if (node->color == RBNode::BLACK && (minBlackNode == nullptr || node->key < minBlackNode->key)) {
        minBlackNode = node;
    }

    findMinimumBlackHelper(node->right, minBlackNode);

    return minBlackNode;
}

RBNode* RBTree::minimumBlack() {
    RBNode* minBlackNode = nullptr;
    return findMinimumBlackHelper(root, minBlackNode);
}

double RBTree::blackNodePercentage() {
    int totalBlackNodes = countBlackNodes();
    int totalNodes = totalNodesHelper(root);

    if (totalNodes == 0) return 0.0;

    return (static_cast<double>(totalBlackNodes) / totalNodes) * 100.0;
}

int RBTree::totalNodesHelper(RBNode* node) {
    if (node == NIL) return 0;

    return 1 + totalNodesHelper(node->left) + totalNodesHelper(node->right);
}

int RBTree::countBlackNodesHelper(RBNode* node) {
    if (node == NIL) return 0;

    int count = (node->color == RBNode::BLACK) ? 1 : 0;
    return count + countBlackNodesHelper(node->left) + countBlackNodesHelper(node->right);
}

int RBTree::countRedNodesHelper(RBNode* node) {
    if (node == NIL) return 0;

    int count = (node->color == RBNode::RED) ? 1 : 0;
    return count + countRedNodesHelper(node->left) + countRedNodesHelper(node->right);
}

void RBTree::RBInsert(int key) {
    RBNode* z = new RBNode(key, nullptr, NIL, NIL, RBNode::RED);
    RBNode* y = NIL;
    RBNode* x = root;

    while (x != NIL) {
        y = x;
        x = (z->key < x->key) ? x->left : x->right;
    }
    z->parent = y;
    if (y == NIL)
        root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = z->right = NIL;
    RBInsertFixup(z);
}

void RBTree::RBDelete(RBNode* z){
    RBNode* y = z;
    RBNode* x;
    RBNode::Color yOriginalColor = y->color;

    if (z->left == NIL) {
        x = z->right;
        if (z->parent == NIL)
            root = x;
        else if (z == z->parent->left)
            z->parent->left = x;
        else
            z->parent->right = x;
        x->parent = z->parent;
    } else if (z->right == NIL) {
        x = z->left;
        if (z->parent == NIL)
            root = x;
        else if (z == z->parent->left)
            z->parent->left = x;
        else
            z->parent->right = x;
        x->parent = z->parent;
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            if (y->parent != NIL)
                y->parent->left = x;
            x->parent = y->parent;
            y->right = z->right;
            y->right->parent = y;
        }

        if (z->parent == NIL)
            root = y;
        else if (z == z->parent->left)
            z->parent->left = y;
        else
            z->parent->right = y;

        y->parent = z->parent;
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (yOriginalColor == RBNode::BLACK) {
        RBDeleteFixup(x);
    }
}

void RBTree::RBInsertFixup(RBNode* z) {
    while (z->parent->color == RBNode::RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* y = z->parent->parent->right;
            if (y->color == RBNode::RED) {
                z->parent->color = RBNode::BLACK;
                y->color = RBNode::BLACK;
                z->parent->parent->color = RBNode::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = RBNode::BLACK;
                z->parent->parent->color = RBNode::RED;
                rightRotate(z->parent->parent);
            }
        } else {
            RBNode* y = z->parent->parent->left;
            if (y->color == RBNode::RED) {
                z->parent->color = RBNode::BLACK;
                y->color = RBNode::BLACK;
                z->parent->parent->color = RBNode::RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = RBNode::BLACK;
                z->parent->parent->color = RBNode::RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = RBNode::BLACK;
}

RBNode* RBTree::search(RBNode* x, int key) {
    if (x == NIL || key == x->key)
        return x;
    return search((key < x->key) ? x->left : x->right, key);
}

RBNode* RBTree::minimum(RBNode* x) {
    while (x->left != NIL)
        x = x->left;
    return x;
}

RBNode* RBTree::maximum(RBNode* x) {
    while (x->right != NIL)
        x = x->right;
    return x;
}

RBNode* RBTree::successor(RBNode* x) {
    if (x->right != NIL)
        return minimum(x->right);
    RBNode* y = x->parent;
    while (y != NIL && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y;
}

RBNode* RBTree::predecessor(RBNode* x){
    if (x->left != NIL) {
        return maximum(x->left);
    }
    RBNode* y = x->parent;
    while (y != NIL && x == y->left) {
        x = y;
        y = y->parent;
    }
    return y;
}

void RBTree::RBDeleteFixup(RBNode* x){
    while (x != root && x->color == RBNode::BLACK) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;
            if (w->color == RBNode::RED) {
                w->color = RBNode::BLACK;
                x->parent->color = RBNode::RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RBNode::BLACK && w->right->color == RBNode::BLACK) {
                w->color = RBNode::RED;
                x = x->parent;
            } else {
                if (w->right->color == RBNode::BLACK) {
                    w->left->color = RBNode::BLACK;
                    w->color = RBNode::RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = RBNode::BLACK;
                w->right->color = RBNode::BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            RBNode* w = x->parent->left;
            if (w->color == RBNode::RED) {
                w->color = RBNode::BLACK;
                x->parent->color = RBNode::RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == RBNode::BLACK && w->left->color == RBNode::BLACK) {
                w->color = RBNode::RED;
                x = x->parent;
            } else {
                if (w->left->color == RBNode::BLACK) {
                    w->right->color = RBNode::BLACK;
                    w->color = RBNode::RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBNode::BLACK;
                w->left->color = RBNode::BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = RBNode::BLACK;
}

void RBTree::leftRotate(RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left != NIL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void RBTree::rightRotate(RBNode* x) {
    RBNode* y = x->left;
    x->left = y->right;
    if (y->right != NIL)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void RBTree::deleteSubtree(RBNode* x) {
    if (x != NIL) {
        deleteSubtree(x->left);
        deleteSubtree(x->right);
        delete x;
    }
}

int RBTree::maxBlackKeyHelper(RBNode* node, int& maxKey) {
    if (node == NIL) return maxKey;

    maxBlackKeyHelper(node->left, maxKey);

    if (node->color == RBNode::BLACK && node->key > maxKey) {
        maxKey = node->key;
    }

    maxBlackKeyHelper(node->right, maxKey);

    return maxKey;
}


int RBTree::maxRedKeyHelper(RBNode* node, int& maxKey) {
    if (node == NIL) return maxKey;

    maxRedKeyHelper(node->left, maxKey);

    if (node->color == RBNode::RED && node->key > maxKey) {
        maxKey = node->key;
    }

    maxRedKeyHelper(node->right, maxKey);

    return maxKey;
}

int RBTree::maxRedKey() {
    if (root == NIL) return -1;

    int maxKey = INT_MIN;
    maxRedKeyHelper(root, maxKey);

    return (maxKey == INT_MIN) ? -1 : maxKey;
}

int RBTree::calculateDepth(RBNode* node) {
    if (node == NIL) return 0;

    int leftDepth = calculateDepth(node->left);
    int rightDepth = calculateDepth(node->right);

    return 1 + max(leftDepth, rightDepth);
}

int RBTree::depth() {
    return calculateDepth(root);
}


int RBTree::maxBlackKey() {
    if (root == NIL) return -1;

    int maxKey = INT_MIN;
    maxBlackKeyHelper(root, maxKey);

    return (maxKey == INT_MIN) ? -1 : maxKey;
}


void RBTree::inorder(RBNode* x) {
    if (x != NIL) {
        inorder(x->left);
        cout << x->toString() << " ";
        inorder(x->right);
    }
}

void RBTree::indentedDisplay(RBNode* x, int indent) {
    if (x != NIL) {
        indentedDisplay(x->right, indent + 4);
        if (indent > 0)
            cout << string(indent, ' ');
        cout << x->toString() << endl;
        indentedDisplay(x->left, indent + 4);
    }
}

int RBTree::blackHeight(RBNode* x) {
    if (x == NIL)
        return 0;
    int leftHeight = blackHeight(x->left);
    int rightHeight = blackHeight(x->right);
    return (x->color == RBNode::BLACK ? 1 : 0) + max(leftHeight, rightHeight);
}

void rbTreeMenu() {
    RBTree tree;
    int choice = 0;

    while (choice != 19) {
        cout << "\n--- Red-Black Tree (RBTree) Menu ---\n";
        cout << "1. Add nodes\n";
        cout << "2. Delete a node\n";
        cout << "3. Get the minimum node\n";
        cout << "4. Get the maximum node\n";
        cout << "5. Get the successor of a node\n";
        cout << "6. Get the predecessor of a node\n";
        cout << "7. Show tree (indented format)\n";
        cout << "8. Show inorder traversal\n";
        cout << "9. Show black-height of the tree\n";
        cout << "10. Show the maximum key of black nodes\n";
        cout << "11. Show the maximum key of red nodes\n";
        cout << "12. Show the depth of the tree\n";
        cout << "13. Count red nodes *NEW*\n";
        cout << "14. Count black nodes *NEW*\n";
        cout << "15. Show the percentage of black nodes *NEW*\n";
        cout << "16. Find the minimum red node *NEW*\n";
        cout << "17. Find the minimum black node *NEW*\n";
        cout << "18. Find the path to a key *NEW*\n";
        cout << "19. Back to main menu\n";
        cout << "Enter your choice (1-19): ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input. Please try again." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        list<int> nodeKeys;
        RBNode* node;
        int key;

        switch (choice) {
            case 1:
                IODialog::getNodeKeys(nodeKeys);
                for (int k : nodeKeys) {
                    tree.RBInsert(k);
                }
                cout << "Nodes added successfully.\n";
                break;
            case 2:
                key = IODialog::getNodeKey();
                node = tree.search(tree.root, key);
                if (node != NIL) {
                    tree.RBDelete(node);
                    cout << "Node deleted successfully.\n";
                } else {
                    cout << "Node not found.\n";
                }
                break;
            case 3:
                node = tree.minimum(tree.root);
                if (node != NIL)
                    cout << "Minimum node: " << node->toString() << endl;
                else
                    cout << "Tree is empty.\n";
                break;
            case 4:
                node = tree.maximum(tree.root);
                if (node != NIL)
                    cout << "Maximum node: " << node->toString() << endl;
                else
                    cout << "Tree is empty.\n";
                break;
            case 5:
                key = IODialog::getNodeKey();
                node = tree.search(tree.root, key);
                if (node != NIL) {
                    RBNode* succ = tree.successor(node);
                    if (succ != NIL)
                        cout << "Successor: " << succ->toString() << endl;
                    else
                        cout << "No successor found.\n";
                } else {
                    cout << "Node not found.\n";
                }
                break;
            case 6:
                key = IODialog::getNodeKey();
                node = tree.search(tree.root, key);
                if (node != NIL) {
                    RBNode* pred = tree.predecessor(node);
                    if (pred != NIL)
                        cout << "Predecessor: " << pred->toString() << endl;
                    else
                        cout << "No predecessor found.\n";
                } else {
                    cout << "Node not found.\n";
                }
                break;
            case 7:
                tree.indentedDisplay();
                break;
            case 8:
                tree.inorder();
                cout << endl;
                break;
            case 9:
                cout << "Black-height of the tree: " << tree.blackHeight() << endl;
                break;
            case 10:
                cout << "Maximum key of black nodes: " << tree.maxBlackKey() << endl;
                break;
            case 11:
                cout << "Maximum key of red nodes: " << tree.maxRedKey() << endl;
                break;
            case 12:
                cout << "Depth of the tree: " << tree.depth() << endl;
                break;
            case 13:
                cout << "Number of red nodes: " << tree.countRedNodes() << endl;
                break;
            case 14:
                cout << "Number of black nodes: " << tree.countBlackNodes() << endl;
                break;
            case 15:
                cout << "Percentage of black nodes: " << tree.blackNodePercentage() << "%\n";
                break;
            case 16: {
                RBNode* minRed = tree.minimumRed();
                if (minRed)
                    cout << "Minimum red node: " << minRed->key << endl;
                else
                    cout << "No red nodes found in the tree.\n";
                break;
            }
            case 17: {
                RBNode* minBlack = tree.minimumBlack();
                if (minBlack)
                    cout << "Minimum black node: " << minBlack->key << endl;
                else
                    cout << "No black nodes found in the tree.\n";
                break;
            }
            case 18: {
                key = IODialog::getNodeKey();
                std::vector<int> path = tree.pathToKey(key);
                if (!path.empty()) {
                    std::cout << "Path to key " << key << ": ";
                    for (int p : path) {
                        std::cout << p << " ";
                    }
                    std::cout << std::endl;
                }
                break;
            }
            case 19:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}
