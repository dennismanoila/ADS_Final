#include "IODialog.h"

namespace IODialog {
    void getNodeKeys(std::list<int>& nodeKeys) {
        std::cout << "Enter node keys (space-separated): ";
        int key;
        std::string line;
        std::cin.ignore();
        std::getline(std::cin, line);
        std::istringstream iss(line);
        while (iss >> key) {
            nodeKeys.push_back(key);
        }
    }

    int getNodeKey() {
        std::cout << "Enter the key: ";
        int key;
        std::cin >> key;
        return key;
    }

    std::list<int> getMultipleKeys(int count) {
        std::list<int> keys;
        std::cout << "Enter " << count << " keys (space-separated): ";
        int key;
        for (int i = 0; i < count; ++i) {
            std::cin >> key;
            keys.push_back(key);
        }
        return keys;
    }

    std::pair<int, int> getRange() {
        int low, high;
        std::cout << "Enter the lower bound: ";
        std::cin >> low;
        std::cout << "Enter the higher bound: ";
        std::cin >> high;
        return {low, high};
    }
}
