#include <iostream>
#include <vector>
#include "BTreeOperations.h"
#include "IODialog.h"

using namespace std;

struct BTreeNode {
    vector<int> keys;
    vector<BTreeNode*> children;
    bool isLeaf;
    int t;

    BTreeNode(int t, bool isLeaf);

    void traverse();
    BTreeNode* search(int key);
    void insertNonFull(int key);
    void splitChild(int i, BTreeNode* y);
    void removeKey(int key);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    int getPred(int idx);
    int getSucc(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
    void displayIndented(int depth);
    int depth();

};

BTreeNode::BTreeNode(int t, bool isLeaf) : t(t), isLeaf(isLeaf) {}

void BTreeNode::traverse() {
    int i;
    for (i = 0; i < keys.size(); i++) {
        if (!isLeaf) {
            children[i]->traverse();
        }
        cout << " " << keys[i];
    }
    if (!isLeaf) {
        children[i]->traverse();
    }
}

BTreeNode* BTreeNode::search(int key) {
    int i = 0;
    while (i < keys.size() && key > keys[i]) {
        i++;
    }
    if (i < keys.size() && keys[i] == key) {
        return this;
    }
    if (isLeaf) {
        return nullptr;
    }
    return children[i]->search(key);
}

void BTreeNode::insertNonFull(int key) {
    int i = keys.size() - 1;
    if (isLeaf) {
        keys.resize(keys.size() + 1);
        while (i >= 0 && keys[i] > key) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = key;
    } else {
        while (i >= 0 && keys[i] > key) {
            i--;
        }
        i++;
        if (children[i]->keys.size() == 2 * t - 1) {
            splitChild(i, children[i]);
            if (keys[i] < key) {
                i++;
            }
        }
        children[i]->insertNonFull(key);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->t, y->isLeaf);
    z->keys.resize(t - 1);
    for (int j = 0; j < t - 1; j++) {
        z->keys[j] = y->keys[j + t];
    }
    if (!y->isLeaf) {
        z->children.resize(t);
        for (int j = 0; j < t; j++) {
            z->children[j] = y->children[j + t];
        }
    }
    y->keys.resize(t - 1);
    children.resize(children.size() + 1);
    for (int j = children.size() - 1; j > i + 1; j--) {
        children[j] = children[j - 1];
    }
    children[i + 1] = z;
    keys.resize(keys.size() + 1);
    for (int j = keys.size() - 1; j > i; j--) {
        keys[j] = keys[j - 1];
    }
    keys[i] = y->keys[t - 1];
}

void BTreeNode::removeKey(int key) {
    int idx = 0;
    while (idx < keys.size() && keys[idx] < key) {
        idx++;
    }

    if (idx < keys.size() && keys[idx] == key) {
        if (isLeaf) {
            removeFromLeaf(idx);
        } else {
            removeFromNonLeaf(idx);
        }
    } else {
        if (isLeaf) {
            cout << "The key " << key << " is not present in the tree.\n";
            return;
        }

        bool flag = (idx == keys.size());
        if (children[idx]->keys.size() < t) {
            fill(idx);
        }
        if (flag && idx > keys.size()) {
            children[idx - 1]->removeKey(key);
        } else {
            children[idx]->removeKey(key);
        }
    }
}

void BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < keys.size(); ++i) {
        keys[i - 1] = keys[i];
    }
    keys.pop_back();
}

void BTreeNode::removeFromNonLeaf(int idx) {
    int key = keys[idx];

    if (children[idx]->keys.size() >= t) {
        int pred = getPred(idx);
        keys[idx] = pred;
        children[idx]->removeKey(pred);
    } else if (children[idx + 1]->keys.size() >= t) {
        int succ = getSucc(idx);
        keys[idx] = succ;
        children[idx + 1]->removeKey(succ);
    } else {
        merge(idx);
        children[idx]->removeKey(key);
    }
}

int BTreeNode::getPred(int idx) {
    BTreeNode* cur = children[idx];
    while (!cur->isLeaf) {
        cur = cur->children[cur->keys.size()];
    }
    return cur->keys.back();
}

int BTreeNode::getSucc(int idx) {
    BTreeNode* cur = children[idx + 1];
    while (!cur->isLeaf) {
        cur = cur->children[0];
    }
    return cur->keys[0];
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && children[idx - 1]->keys.size() >= t) {
        borrowFromPrev(idx);
    } else if (idx != keys.size() && children[idx + 1]->keys.size() >= t) {
        borrowFromNext(idx);
    } else {
        if (idx != keys.size()) {
            merge(idx);
        } else {
            merge(idx - 1);
        }
    }
}

void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx - 1];

    for (int i = child->keys.size() - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
    }
    if (!child->isLeaf) {
        for (int i = child->children.size() - 1; i >= 0; --i) {
            child->children[i + 1] = child->children[i];
        }
    }
    child->keys[0] = keys[idx - 1];
    if (!child->isLeaf) {
        child->children[0] = sibling->children[sibling->keys.size()];
    }
    keys[idx - 1] = sibling->keys.back();
    sibling->keys.pop_back();
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];

    child->keys.push_back(keys[idx]);
    if (!child->isLeaf) {
        child->children.push_back(sibling->children[0]);
    }
    keys[idx] = sibling->keys[0];
    sibling->keys.erase(sibling->keys.begin());
    if (!sibling->isLeaf) {
        sibling->children.erase(sibling->children.begin());
    }
}

void BTreeNode::merge(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];

    child->keys.push_back(keys[idx]);
    for (int i = 0; i < sibling->keys.size(); ++i) {
        child->keys.push_back(sibling->keys[i]);
    }
    if (!child->isLeaf) {
        for (int i = 0; i < sibling->children.size(); ++i) {
            child->children.push_back(sibling->children[i]);
        }
    }
    keys.erase(keys.begin() + idx);
    children.erase(children.begin() + idx + 1);
    delete sibling;
}

void BTreeNode::displayIndented(int depth) {
    for (int i = depth; i > 0; i--) {
        cout << "    ";
    }
    for (int key : keys) {
        cout << key << " ";
    }
    cout << endl;
    if (!isLeaf) {
        for (BTreeNode* child : children) {
            child->displayIndented(depth + 1);
        }
    }
}

struct BTree {
    BTreeNode* root;
    int t;

    BTree(int t);

    void traverse() { if (root != nullptr) root->traverse(); }
    BTreeNode* search(int key) { return (root == nullptr) ? nullptr : root->search(key); }
    void insert(int key);
    void deleteKey(int key);
    void displayIndented();
    int depth();
    int keyCount();
    int countLeafNodes();
    int findMinimumKey();
    int findMaximumKey();

};

int BTree::findMaximumKey() {
    if (root == nullptr) {
        cout << "The tree is empty.\n";
        return -1;
    }

    BTreeNode* current = root;
    while (!current->isLeaf) {
        current = current->children[current->keys.size()];
    }

    return current->keys.back();
}


int BTree::findMinimumKey() {
    if (root == nullptr) {
        cout << "The tree is empty.\n";
        return -1;
    }

    BTreeNode* current = root;
    while (!current->isLeaf) {
        current = current->children[0];
    }

    return current->keys[0];
}


int calculateLeafNodes(BTreeNode* node) {
    if (node == nullptr) return 0;

    if (node->isLeaf) return 1;

    int count = 0;
    for (BTreeNode* child : node->children) {
        count += calculateLeafNodes(child);
    }

    return count;
}

int BTree::countLeafNodes() {
    return calculateLeafNodes(root);
}


int calculateKeyCount(BTreeNode* node) {
    if (node == nullptr) return 0;

    int count = node->keys.size();

    for (BTreeNode* child : node->children) {
        count += calculateKeyCount(child);
    }

    return count;
}

int BTree::keyCount() {
    return calculateKeyCount(root);
}

int calculateDepth(BTreeNode* node) {
    if (node == nullptr) return 0;

    int maxChildDepth = 0;

    for (BTreeNode* child : node->children) {
        maxChildDepth = max(maxChildDepth, calculateDepth(child));
    }

    return 1 + maxChildDepth;
}

int BTree::depth() {
    return calculateDepth(root);
}

BTree::BTree(int t) : t(t), root(nullptr) {}

void BTree::insert(int key) {
    if (root == nullptr) {
        root = new BTreeNode(t, true);
        root->keys.push_back(key);
    } else {
        if (root->keys.size() == 2 * t - 1) {
            BTreeNode* s = new BTreeNode(t, false);
            s->children.push_back(root);
            s->splitChild(0, root);
            int i = (s->keys[0] < key) ? 1 : 0;
            s->children[i]->insertNonFull(key);
            root = s;
        } else {
            root->insertNonFull(key);
        }
    }
}

void BTree::deleteKey(int key) {
    if (!root) {
        cout << "The tree is empty.\n";
        return;
    }
    root->removeKey(key);
    if (root->keys.empty()) {
        BTreeNode* oldRoot = root;
        root = root->isLeaf ? nullptr : root->children[0];
        delete oldRoot;
    }
}

void BTree::displayIndented() {
    if (root != nullptr) root->displayIndented(0);
}

void bTreeMenu() {
    int t;
    cout << "Enter the minimum degree of the B-Tree: ";
    cin >> t;
    BTree tree(t);
    int choice;

    while (true) {
        cout << "\n--- B-Tree Menu ---\n";
        cout << "1. Insert a key\n";
        cout << "2. Search for a key\n";
        cout << "3. Delete a key\n";
        cout << "4. Traverse the B-Tree\n";
        cout << "5. Display B-Tree (Indented)\n";
        cout << "6. Show tree depth *NEW*\n";
        cout << "7. Count total keys in tree *NEW*\n";
        cout << "8. Count Total Leaf Nodes in the Tree\n";
        cout << "9. Find minimum key in the Tree *NEW*\n";
        cout << "10. Find Maximum Key in the Tree\n";
        cout << "11. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        int key;
        switch (choice) {
            case 1:
                cout << "Enter key to insert: ";
                cin >> key;
                tree.insert(key);
                break;
            case 2:
                cout << "Enter key to search: ";
                cin >> key;
                if (tree.search(key))
                    cout << "Key found.\n";
                else
                    cout << "Key not found.\n";
                break;
            case 3:
                cout << "Enter key to delete: ";
                cin >> key;
                tree.deleteKey(key);
                break;
            case 4:
                cout << "Traversing B-Tree: \n";
                tree.traverse();
                cout << endl;
                break;
            case 5:
                cout << "B-Tree (Indented):\n";
                tree.displayIndented();
                break;
            case 6:
                cout << "Depth of the B-Tree: " << tree.depth() << endl;
                break;
            case 7:
                cout << "Total number of keys in the B-Tree: " << tree.keyCount() << endl;
                break;
            case 8:
                cout << "Total number of leaf nodes in the B-Tree: " << tree.countLeafNodes() << endl;
                break;
            case 9:
            {
                int minKey = tree.findMinimumKey();
                if (minKey != -1)
                    cout << "Minimum key in the B-Tree: " << minKey << endl;
            }
                break;
            case 10:
            {
                int maxKey = tree.findMaximumKey();
                if (maxKey != -1)
                    cout << "Maximum key in the B-Tree: " << maxKey << endl;
            }
                break;

            case 11:
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}
