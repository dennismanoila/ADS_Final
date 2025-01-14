//
// Created by Denis on 1/13/2025.
//

#ifndef FINALPROJECTV2_IODIALOG_H
#define FINALPROJECTV2_IODIALOG_H

#include <iostream>
#include <list>
#include <sstream>
#include <string>

namespace IODialog {
    void getNodeKeys(std::list<int>& nodeKeys);
    int getNodeKey();
    std::list<int> getMultipleKeys(int count);
    std::pair<int, int> getRange();
}


#endif //FINALPROJECTV2_IODIALOG_H
