//
// Created by diogopaiva21 on 10-03-2024.
//

#ifndef TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODEL_H
#define TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODEL_H

#include <string>
#include <vector>

using namespace std;

class CopyModel {

public:
    CopyModel(int reference = -1);
    void add_anchor(int position);
    void update_reference(bool remove_current = true, string criteria = "last");
    void hit();
    void miss();

    int reference;
    int hits;
    int misses;
private:
    vector<int> anchors;
};


#endif //TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODEL_H
