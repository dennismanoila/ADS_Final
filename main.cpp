#include <iostream>
#include "BSTOperations.h"
#include "RBTreeOperations.h"
#include "BTreeOperations.h"

using namespace std;

void showMainMenu() {
    cout << "\n--- Data Structure Playground ---\n";
    cout << "1. Binary Search Tree (BST)\n";
    cout << "2. Red-Black Tree (RBTree)\n";
    cout << "3. B-Tree\n";
    cout << "4. Exit\n";
    cout << "Enter your choice (1-4): ";
}

int main() {
    int choice = 0;
    while (choice != 4) {
        showMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                bstMenu();
                break;
            case 2:
                rbTreeMenu();
                break;
            case 3:
                bTreeMenu();
                break;
            case 4:
                cout << "Exiting... Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
                break;
        }
    }
    return 0;
}
