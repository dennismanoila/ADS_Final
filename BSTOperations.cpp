#include <iostream>
#include <list>
#include <vector>
#include "IODialog.h"
#include "BSTOperations.h"

using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    Node* parent;

    Node(int k, Node* l = nullptr, Node* r = nullptr, Node* p = nullptr)
            : key(k), left(l), right(r), parent(p) {}

    string toString() {
        return to_string(key);
    }
};

struct BSTree {
    Node* root;

    BSTree() : root(nullptr) {}
    ~BSTree() { deleteSubtree(root); }

    Node* createNode(int key) { return new Node(key); }

    void insert(Node* z) {
        Node* y = nullptr;
        Node* x = root;
        while (x != nullptr) {
            y = x;
            x = (z->key < x->key) ? x->left : x->right;
        }
        z->parent = y;
        if (y == nullptr)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;
    }

    Node* search(Node* x, int key) {
        if (x == nullptr || key == x->key)
            return x;
        return search((key < x->key) ? x->left : x->right, key);
    }

    Node* minimum(Node* x) {
        while (x && x->left != nullptr)
            x = x->left;
        return x;
    }

    Node* maximum(Node* x) {
        while (x && x->right != nullptr)
            x = x->right;
        return x;
    }

    Node* successor(Node* x) {
        if (x->right != nullptr)
            return minimum(x->right);
        Node* y = x->parent;
        while (y != nullptr && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    Node* predecessor(Node* x) {
        if (x->left != nullptr)
            return maximum(x->left);
        Node* y = x->parent;
        while (y != nullptr && x == y->left) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    void del(Node* z) {
        if (z == nullptr) return;
        Node* y = (z->left == nullptr || z->right == nullptr) ? z : successor(z);
        Node* x = (y->left != nullptr) ? y->left : y->right;
        if (x != nullptr)
            x->parent = y->parent;
        if (y->parent == nullptr)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        if (y != z)
            z->key = y->key;
        delete y;
    }

    void inorder(Node* x) {
        if (x != nullptr) {
            inorder(x->left);
            cout << x->toString() << " ";
            inorder(x->right);
        }
    }

    void inorder() { inorder(root); }

    void indentedDisplay(Node* x, int indent) {
        if (x != nullptr) {
            indentedDisplay(x->right, indent + 4);
            if (indent > 0)
                cout << string(indent, ' ');
            cout << x->toString() << endl;
            indentedDisplay(x->left, indent + 4);
        }
    }

    void indentedDisplay() {
        cout << "Binary Search Tree:\n";
        indentedDisplay(root, 0);
    }

    int depth(Node* x) {
        if (x == nullptr) return 0;
        return 1 + max(depth(x->left), depth(x->right));
    }

    int depth() { return depth(root); }

    bool isBalanced(Node* x) {
        if (x == nullptr) return true;

        int leftHeight = depth(x->left);
        int rightHeight = depth(x->right);

        return abs(leftHeight - rightHeight) <= 1 &&
               isBalanced(x->left) &&
               isBalanced(x->right);
    }

    Node* findLCA(Node* root, int key1, int key2) {
        if (root == nullptr) return nullptr;

        if (root->key > key1 && root->key > key2)
            return findLCA(root->left, key1, key2);

        if (root->key < key1 && root->key < key2)
            return findLCA(root->right, key1, key2);

        return root;
    }

    void kthSmallestHelper(Node* x, int& k, int& result) {
        if (x == nullptr || k <= 0) return;

        kthSmallestHelper(x->left, k, result);

        if (--k == 0) {
            result = x->key;
            return;
        }

        kthSmallestHelper(x->right, k, result);
    }

    int kthSmallest(int k) {
        int result = -1;
        kthSmallestHelper(root, k, result);
        return result;
    }

    Node* kthLargest(Node* root, int& k) {
        if (root == nullptr) return nullptr;

        Node* result = kthLargest(root->right, k);
        if (result != nullptr) return result;

        k--;
        if (k == 0) return root;

        return kthLargest(root->left, k);
    }

    Node* kthLargest(int k) {
        return kthLargest(root, k);
    }

    void rangeQuery(Node* root, int low, int high, std::list<int>& result) {
        if (root == nullptr) return;

        if (low < root->key)
            rangeQuery(root->left, low, high, result);

        if (low <= root->key && root->key <= high)
            result.push_back(root->key);

        if (high > root->key)
            rangeQuery(root->right, low, high, result);
    }

    std::list<int> rangeQuery(int low, int high) {
        std::list<int> result;
        rangeQuery(root, low, high, result);
        return result;
    }


private:
    void deleteSubtree(Node* x) {
        if (x != nullptr) {
            deleteSubtree(x->left);
            deleteSubtree(x->right);
            delete x;
        }
    }
};

void bstMenu() {
    BSTree tree;
    int choice = 0;

    while (choice != 15) {
        cout << "\n--- Binary Search Tree (BST) Menu ---\n";
        cout << "1. Add nodes\n";
        cout << "2. Delete a node\n";
        cout << "3. Get the minimum node\n";
        cout << "4. Get the maximum node\n";
        cout << "5. Get the successor of a node\n";
        cout << "6. Get the predecessor of a node\n";
        cout << "7. Show tree (indented format)\n";
        cout << "8. Show inorder traversal\n";
        cout << "9. Show tree depth\n";
        cout << "10. Check if the tree is balanced *NEW*\n";
        cout << "11. Find the lowest common ancestor of two nodes *NEW*\n";
        cout << "12. Find the kth smallest element *NEW*\n";
        cout << "13. Find the kth greatest element *NEW*\n";
        cout << "14. Find the elements in a certain range *NEW*\n";
        cout << "15. Back to main menu\n";
        cout << "Enter your choice (1-15): ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input. Please try again.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        list<int> nodeKeys;
        Node* node;
        int key;

        switch (choice) {
            case 1:
                IODialog::getNodeKeys(nodeKeys);
                for (int k : nodeKeys) {
                    tree.insert(tree.createNode(k));
                }
                cout << "Nodes added successfully.\n";
                break;
            case 2:
                key = IODialog::getNodeKey();
                node = tree.search(tree.root, key);
                if (node)
                    tree.del(node);
                else
                    cout << "Node not found.\n";
                break;
            case 3:
                node = tree.minimum(tree.root);
                if (node)
                    cout << "Minimum node: " << node->toString() << endl;
                else
                    cout << "Tree is empty.\n";
                break;
            case 4:
                node = tree.maximum(tree.root);
                if (node)
                    cout << "Maximum node: " << node->toString() << endl;
                else
                    cout << "Tree is empty.\n";
                break;
            case 5:
                key = IODialog::getNodeKey();
                node = tree.search(tree.root, key);
                if (node) {
                    Node* succ = tree.successor(node);
                    if (succ)
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
                if (node) {
                    Node* pred = tree.predecessor(node);
                    if (pred)
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
                cout << "Tree depth: " << tree.depth() << endl;
                break;
            case 10:
                cout << "Tree is " << (tree.isBalanced(tree.root) ? "balanced." : "not balanced.") << endl;
                break;
            case 11: {
                cout << "Finding LCA...\n";
                auto keys = IODialog::getMultipleKeys(2);
                if (keys.size() != 2) {
                    cout << "You must provide exactly two keys.\n";
                    break;
                }

                int key1 = keys.front();
                int key2 = keys.back();

                Node* lca = tree.findLCA(tree.root, key1, key2);
                if (lca)
                    cout << "LCA of " << key1 << " and " << key2 << " is: " << lca->key << endl;
                else
                    cout << "One or both keys are not in the tree.\n";
                break;
            }
            case 12: {
                cout << "Enter k: ";
                cin >> key;
                int result = tree.kthSmallest(key);
                if (result != -1)
                    cout << "The " << key << "th smallest element is: " << result << endl;
                else
                    cout << "k is out of range.\n";
                break;
            }
            case 13:{
                cout << "Enter the value of k to find the kth largest element: ";
                int k;
                cin >> k;

                Node* result = tree.kthLargest(k);
                if (result != nullptr)
                    cout << "The " << k << "th largest element is: " << result->key << endl;
                else
                    cout << "The tree has fewer than " << k << " elements.\n";
                break;
            }
            case 14: {
                auto range = IODialog::getRange();
                int low = range.first;
                int high = range.second;

                std::list<int> result = tree.rangeQuery(low, high);

                std::cout << "Keys in range [" << low << ", " << high << "]: ";
                if (result.empty()) {
                    std::cout << "None";
                } else {
                    for (int key : result) {
                        std::cout << key << " ";
                    }
                }
                std::cout << std::endl;
                break;
            }
            case 15:
                cout << "Returning to main menu...\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}
