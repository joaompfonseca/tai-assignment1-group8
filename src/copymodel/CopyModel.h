#ifndef TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODEL_H
#define TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODEL_H

#include <string>
#include <vector>

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
    double partial_number_of_bits;
};

#endif //TAI_ASSIGNMENT1_GROUP8_SRC_COPYMODEL_H