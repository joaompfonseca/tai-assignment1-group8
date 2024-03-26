#ifndef TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODEL_H
#define TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODEL_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

class CopyModel {

public:
    CopyModel(int reference = -1);
    double getBits();
    int getHits();
    int getMisses();
    int getReference();
    void hit();
    void miss();
    void incrementReference();
    void addBits(double amount);
    void reset();

private:
    int reference;
    int start;
    int hits;
    int misses;
    double partialNumberOfBits;
};

#endif //TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODEL_H